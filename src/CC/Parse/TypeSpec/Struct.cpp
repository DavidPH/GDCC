//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2017 David Hill
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

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "Platform/Platform.hpp"

#include "SR/Attribute.hpp"
#include "SR/Exp.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // ParseTypeSpec_Struct
   //
   // struct-or-union-specifier:
   //    struct-or-union identifier(opt) { struct-declaration-list }
   //    struct-or-union identifier
   //
   static void ParseTypeSpec_struct(Parser &ctx, Scope &scope,
      SR::Attribute &attr, TypeSpec &spec, bool isUnion)
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
      std::vector<Type_Struct::PropData>   propv;

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

         for(auto const &prop : propv)
            if(prop.name == name) return true;

         return false;
      };

      //
      // getStructPropArg
      //
      // struct-property-argument:
      //    <this>
      //    <__arg>
      //    <__this>
      //    * <this>
      //    * <__this>
      //    constant-expression
      //
      auto getStructPropArg = [&]() -> StructPropArg
      {
         if(ctx.in.drop(Core::TOK_Identi, Core::STR_this) ||
            ctx.in.drop(Core::TOK_Identi, Core::STR___this))
            return StructPropArg::ThisPtr;

         if(ctx.in.drop(Core::TOK_Identi, Core::STR___arg))
            return StructPropArg::Value;

         if(ctx.in.drop(Core::TOK_Mul, Core::TOK_Identi, Core::STR_this) ||
            ctx.in.drop(Core::TOK_Mul, Core::TOK_Identi, Core::STR___this))
            return StructPropArg::ThisRef;

         if(ctx.in.peek(Core::TOK_Mem, Core::TOK_Identi))
            return ctx.in.get(), ctx.in.get().str;

         return {ctx.getExp_Cond(scope)};
      };

      //
      // getStructProp
      //
      // struct-property:
      //    struct-property-name : primary-expression (
      //       struct-property-argument-list(opt) )
      //
      auto getStructProp = [&]() -> StructProp
      {
         // struct-property-name
         ctx.in.get();

         // :
         if(!ctx.in.drop(Core::TOK_Colon))
            throw Core::ParseExceptExpect{ctx.in.reget(), ":", true};

         // primary-expression
         SR::Exp::CRef func = ctx.getExp_Prim(scope);

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect{ctx.in.reget(), "(", true};

         // struct-property-argument-list(opt)
         std::vector<StructPropArg> argv;
         if(!ctx.in.peek(Core::TOK_ParenC)) do
         {
            // struct-property-argument-list:
            //    struct-property-argument
            //    struct-property-argument-list , struct-property-argument

            // struct-property-argument
            argv.push_back(getStructPropArg());
         }
         while(ctx.in.drop(Core::TOK_Comma));

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect{ctx.in.reget(), ")", true};

         return {func, {argv.begin(), argv.end()}};
      };

      //
      // getStructPropDecl
      //
      // struct-property-declaration:
      //    <__prop> identifier { struct-property-list } ;
      //
      auto getStructPropDecl = [&]() -> Type_Struct::PropData
      {
         // <__prop>
         ctx.in.get();

         // identifier
         if(!ctx.in.peek(Core::TOK_Identi))
            throw Core::ParseExceptExpect{ctx.in.peek(), "identifier", false};

         Core::String name = ctx.in.get().str;

         StructProp propCall, propGet, propSet;

         // {
         if(!ctx.in.drop(Core::TOK_BraceO))
            throw Core::ParseExceptExpect{ctx.in.reget(), "{", true};

         // struct-property-list
         do
         {
            // struct-property-list:
            //    struct-property
            //    struct-property-list , struct-property

            // struct-property
            if(ctx.in.peek(Core::TOK_Identi, Core::STR___call) ||
               ctx.in.peek(Core::TOK_Identi, Core::STR_call))
               propCall = getStructProp();
            else if(ctx.in.peek(Core::TOK_Identi, Core::STR___get) ||
               ctx.in.peek(Core::TOK_Identi, Core::STR_get))
               propGet = getStructProp();
            else if(ctx.in.peek(Core::TOK_Identi, Core::STR___set) ||
               ctx.in.peek(Core::TOK_Identi, Core::STR_set))
               propSet = getStructProp();
            else
               throw Core::ParseExceptExpect{ctx.in.peek(), "struct-property-name", false};
         }
         while(ctx.in.drop(Core::TOK_Comma));

         // }
         if(!ctx.in.drop(Core::TOK_BraceC))
            throw Core::ParseExceptExpect{ctx.in.reget(), "}", true};

         return {name, static_cast<SR::Type::CRef>(type), 0,
            std::move(propCall), std::move(propGet), std::move(propSet)};
      };

      // struct-declaration-list:
      //    struct-declaration
      //    struct-declaration-list struct-declaration
      do
      {
         // struct-declaration:
         //    specifier-qualifier-list struct-declarator-list(opt) ;
         //    static_assert-declaration
         //    struct-property-declaration

         // static_assert-declaration
         if(ctx.isStaticAssert(scope))
            ctx.parseStaticAssert(scope);

         // struct-property-declaration
         else if(ctx.in.peek(Core::TOK_Identi, Core::STR___prop))
            propv.push_back(getStructPropDecl());

         // specifier-qualifier-list struct-declarator-list(opt) ;
         else if(ctx.isSpecQual(scope))
         {
            if(hasFAM)
               throw Core::ExceptStr(ctx.in.peek().pos,
                  "member after flexible array");

            // Check for possible anonymous struct/union: struct-or-union {
            bool maybeAnon =
               ctx.in.peek(Core::TOK_KeyWrd, Core::STR_struct, Core::TOK_BraceO) ||
               ctx.in.peek(Core::TOK_KeyWrd, Core::STR_union,  Core::TOK_BraceO);

            // specifier-qualifier-list
            SR::Attribute attrBase;
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

               SR::Attribute attrTemp = attrBase;

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

                     SR::Type::CRef bitsType{attrTemp.type};

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

                  bool anon = attrTemp.objAnonymous;

                  // Normally, only allow complete types.
                  if(attrTemp.type->isTypeComplete())
                  {
                     addrAlign(attrTemp.type->getSizeAlign());

                     memv.emplace_back(attrTemp.name, attrTemp.type, addr, anon);

                     addrBytes(attrTemp.type->getSizeBytes());
                  }
                  else
                  {
                     // Unless this is a flexible array member.
                     if(!attrTemp.type->isTypeArray())
                        throw Core::ExceptStr(ctx.in.reget().pos, "incomplete member");

                     addrAlign(attrTemp.type->getSizeAlign());

                     memv.emplace_back(attrTemp.name, attrTemp.type, addr, anon);

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

      type->setMembers(memv.data(), memv.size(), propv.data(), propv.size(), size);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Parser::parseTypeSpec_struct
   //
   void Parser::parseTypeSpec_struct(Scope &scope, SR::Attribute &attr, TypeSpec &spec)
   {
      ParseTypeSpec_struct(*this, scope, attr, spec, false);
   }

   //
   // Parser::parseTypeSpec_union
   //
   void Parser::parseTypeSpec_union(Scope &scope, SR::Attribute &attr, TypeSpec &spec)
   {
      ParseTypeSpec_struct(*this, scope, attr, spec, true);
   }
}

// EOF

