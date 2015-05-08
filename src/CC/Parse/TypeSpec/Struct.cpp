//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C struct-or-union-specifier parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp.hpp"
#include "CC/Scope.hpp"
#include "CC/Type/Struct.hpp"

#include "AST/Attribute.hpp"
#include "AST/Exp.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {

      //
      // ParseTypeSpec_Struct
      //
      // struct-or-union-specifier:
      //    struct-or-union identifier(opt) { struct-declaration-list }
      //    struct-or-union identifier
      //
      static void ParseTypeSpec_struct(Parser &ctx, Scope &scope,
         AST::Attribute &attr, TypeSpec &spec, bool isUnion)
      {
         if(spec.specBase)
            throw Core::ExceptStr(ctx.in.reget().pos, "multiple type-specifier base");

         spec.specBase = TypeSpec::BaseName;

         Type_Struct::Ptr type;

         // Named structure.
         if(ctx.in.peek().tok == Core::TOK_Identi)
         {
            auto name  = ctx.in.get();
            bool defin = ctx.in.peek().tok == Core::TOK_BraceO;

            // Start with a local lookup. If the full lookup type is
            // incompatible, it is only an error if it is incompatible with a
            // type from this scope.
            auto lookup = scope.findTypeTag(name.str);
            bool local  = lookup;

            // If definition, only check for existing in current scope.
            if(!lookup && !defin) lookup = scope.lookupTypeTag(name.str);

            // Existing type, check for compatibility.
            if(lookup)
            {
               char const *err = nullptr;

               if(isUnion && !lookup->isCTypeUnion())
                  err = "tag redefined as union";

               else if(!isUnion && !lookup->isCTypeStruct())
                  err = "tag redefined as struct";

               else if(defin && lookup->isTypeComplete())
                  err = "tag redefined";

               if(err)
               {
                  if(local) throw Core::ExceptStr(name.pos, err);
                  lookup = nullptr;
               }
            }

            // No existing compatible type.
            if(!lookup)
            {
               type = Type_Struct::Create(name.str, isUnion);
               scope.addTypeTag(name.str, type);
            }
            else
               type = static_cast<Type_Struct *>(&*lookup);
         }

         // Anonymous structure.
         else
         {
            // Must be a definition.
            if(ctx.in.peek().tok != Core::TOK_BraceO)
               throw Core::ExceptStr(ctx.in.peek().pos, "expected identifier");

            type = Type_Struct::Create(Core::STRNULL, isUnion);
         }

         attr.type = type;

         // Remainder of this function deals with parsing the definition itself.
         if(!ctx.in.drop(Core::TOK_BraceO))
            return;

         // List of members.
         std::vector<Type_Struct::MemberData> memv;

         // Current address in structure. (Remains 0 for unions.)
         Core::FastU addr = 0;

         // Does this structure have a Flexible Array Member?
         bool hasFAM = false;

         // Current size of structure.
         Core::FastU size = 0;

         // Bit-field information.
         Core::FastU bitsAddr = 0;
         Core::FastU bitsAlig = Platform::GetWordAlign();
         Core::FastU bitsSize = Platform::GetWordBytes();
         Core::FastU bitsWord = Platform::GetWordBits();

         //
         // addrAlign
         //
         auto addrAlign = [&](Core::FastU sizeAlign)
         {
            if(!isUnion)
               size = addr = (addr + sizeAlign - 1) / sizeAlign * sizeAlign;
         };

         //
         // addrBytes
         //
         auto addrBytes = [&](Core::FastU sizeBytes)
         {
            if(isUnion)
               size = std::max(size, sizeBytes);
            else
               size = addr += sizeBytes;
         };

         //
         // bitsAlign
         //
         auto bitsAlign = [&]()
         {
            if(!isUnion && !bitsAddr)
               addr = (addr + bitsAlig - 1) / bitsAlig * bitsAlig;
         };

         //
         // bitsReset
         //
         auto bitsReset = [&]()
         {
            if(bitsAddr)
            {
               bitsAddr = 0;

               addrBytes(bitsSize);
            }
         };

         //
         // findName
         //
         auto findName = [&](Core::String name) -> bool
         {
            if(!name) return false;

            for(auto const &mem : memv)
               if(mem.name == name) return true;

            return false;
         };

         // struct-declaration-list:
         //    struct-declaration
         //    struct-declaration-list struct-declaration
         do
         {
            // struct-declaration:
            //    specifier-qualifier-list struct-declarator-list(opt) ;
            //    static_assert-declaration

            // static_assert-declaration
            if(ctx.isStaticAssert(scope))
            {
               ctx.parseStaticAssert(scope);
               continue;
            }

            // specifier-qualifier-list struct-declarator-list(opt) ;
            if(ctx.isSpecQual(scope))
            {
               if(hasFAM)
                  throw Core::ExceptStr(ctx.in.peek().pos,
                     "member after flexible array");

               // Check for possible anonymous struct/union: struct-or-union {
               bool maybeAnon =
                  ctx.in.peek(Core::TOK_KeyWrd, Core::STR_struct, Core::TOK_BraceO) ||
                  ctx.in.peek(Core::TOK_KeyWrd, Core::STR_union,  Core::TOK_BraceO);

               // specifier-qualifier-list
               AST::Attribute attrBase;
               ctx.parseSpecQual(scope, attrBase);

               // Special handling for non-declarator declaration.
               if(ctx.in.peek().tok == Core::TOK_Semico)
               {
                  if(!attrBase.type->isTypeComplete())
                     throw Core::ExceptStr(ctx.in.reget().pos, "incomplete member");

                  addrAlign(attrBase.type->getSizeAlign());

                  memv.emplace_back(Core::STRNULL, attrBase.type, addr, maybeAnon);

                  addrBytes(attrBase.type->getSizeBytes());
               }

               // struct-declarator-list:
               //    struct-declarator
               //    struct-declarator-list , struct-declarator
               else do
               {
                  // struct-declarator:
                  //    declarator
                  //    declarator(opt) : constant-expression

                  AST::Attribute attrTemp = attrBase;

                  if(ctx.isDeclarator(scope))
                     ctx.parseDeclarator(scope, attrTemp);

                  // Bitfield member.
                  if(ctx.in.drop(Core::TOK_Colon))
                  {
                     auto pos  = ctx.in.reget().pos;
                     auto bits = ExpToFastU(ctx.getExp_Cond(scope));

                     if(bits)
                     {
                        // Check that name is valid.
                        if(findName(attrTemp.name))
                           throw Core::ExceptStr(pos, "name reused");

                        // Check for too many bits to fit in a word.
                        if(bits > bitsWord)
                           throw Core::ExceptStr(pos, "too many bits");

                        // Check for enough size in the current word.
                        if(bitsAddr + bits > bitsWord)
                           bitsReset();

                        // Align underlying storage.
                        bitsAlign();

                        AST::Type::CRef bitsType{attrTemp.type};

                        // bitsType must be qualified or unqualified signed int,
                        // unsigned int, or _Bool.
                        if(!bitsType->isCTypeInteg())
                           throw Core::ExceptStr(pos, "required integer type");

                        // Create bitfield type.
                        bitsType = bitsType
                           ->getTypeBitfield(0, bits - bitsType->getSizeBitsS(), bitsAddr)
                           ->getTypeQual(bitsType->getQual());

                        // Add member.
                        memv.emplace_back(attrTemp.name, bitsType, addr, false);

                        // Increase bit address.
                        if(!isUnion) bitsAddr += bits;
                     }

                     // Zero-length bitfield resets bit position.
                     else
                     {
                        if(attrTemp.name)
                           throw Core::ExceptStr(pos, "named empty bitfield");

                        bitsReset();
                     }
                  }

                  // Normal data member.
                  else
                  {
                     if(findName(attrTemp.name))
                        throw Core::ExceptStr(ctx.in.reget().pos, "name reused");

                     bitsReset();

                     // Normally, only allow complete types.
                     if(attrTemp.type->isTypeComplete())
                     {
                        addrAlign(attrTemp.type->getSizeAlign());

                        memv.emplace_back(attrTemp.name, attrTemp.type, addr, false);

                        addrBytes(attrTemp.type->getSizeBytes());
                     }
                     else
                     {
                        // Unless this is a flexible array member.
                        if(!attrTemp.type->isTypeArray())
                           throw Core::ExceptStr(ctx.in.reget().pos, "incomplete member");

                        addrAlign(attrTemp.type->getSizeAlign());

                        memv.emplace_back(attrTemp.name, attrTemp.type, addr, false);

                        // A flexible array member must be the last member.
                        // Therefore, there must be no further declarators.
                        hasFAM = true;
                        break;
                     }
                  }
               }
               while(ctx.in.drop(Core::TOK_Comma));

               // ;
               if(!ctx.in.drop(Core::TOK_Semico))
                  throw Core::ExceptStr(ctx.in.peek().pos, "expected ';'");
            }

            else
               throw Core::ExceptStr(ctx.in.peek().pos, "expected struct-declaration");
         }
         while(!ctx.in.drop(Core::TOK_BraceC));

         if(memv.empty())
            throw Core::ExceptStr(ctx.in.reget().pos, "empty structure");

         bitsReset();

         type->setMembers(memv.data(), memv.size(), size);
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Parser::parseTypeSpec_struct
      //
      void Parser::parseTypeSpec_struct(Scope &scope, AST::Attribute &attr, TypeSpec &spec)
      {
         ParseTypeSpec_struct(*this, scope, attr, spec, false);
      }

      //
      // Parser::parseTypeSpec_union
      //
      void Parser::parseTypeSpec_union(Scope &scope, AST::Attribute &attr, TypeSpec &spec)
      {
         ParseTypeSpec_struct(*this, scope, attr, spec, true);
      }
   }
}

// EOF

