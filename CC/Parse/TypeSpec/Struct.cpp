//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
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

#include "SR/Attribute.hpp"
#include "SR/Exp.hpp"

#include "Target/Info.hpp"


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
         Core::Error(ctx.in.reget().pos, "multiple type-specifier base");

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
               if(local) Core::Error(name.pos, err);
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
         if(!ctx.in.peek(Core::TOK_BraceO))
            Core::ErrorExpect("identifier", ctx.in.peek());

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
      Core::FastU bitsAlig = Target::GetWordAlign();
      Core::FastU bitsSize = Target::GetWordBytes();
      Core::FastU bitsWord = Target::GetWordBits();

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
      //    <__this>
      //    * <this>
      //    * <__this>
      //    -> identifier
      //    ...
      //    constant-expression
      //
      auto getStructPropArg = [&]() -> StructPropArg
      {
         if(ctx.in.drop(Core::TOK_Identi, Core::STR_this) ||
            ctx.in.drop(Core::TOK_Identi, Core::STR___this))
            return StructPropArg::ThisPtr;

         if(ctx.in.drop(Core::TOK_Mul, Core::TOK_Identi, Core::STR_this) ||
            ctx.in.drop(Core::TOK_Mul, Core::TOK_Identi, Core::STR___this))
            return StructPropArg::ThisRef;

         if(ctx.in.peek(Core::TOK_Mem, Core::TOK_Identi))
            return ctx.in.get(), ctx.in.get().str;

         if(ctx.in.drop(Core::TOK_Dot3))
            return StructPropArg::Value;

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
         ctx.expect(Core::TOK_Colon);

         // primary-expression
         SR::Exp::CRef func = ctx.getExp_Prim(scope);

         // (
         ctx.expect(Core::TOK_ParenO);

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
         ctx.expect(Core::TOK_ParenC);

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
         Type_Struct::PropData prop;

         // <__prop>
         ctx.in.get();

         // identifier
         prop.name = ctx.expectIdenti().str;

         // {
         ctx.expect(Core::TOK_BraceO);

         // struct-property-list
         do
         {
            // struct-property-list:
            //    struct-property
            //    struct-property-list , struct-property

            // struct-property
            if(ctx.in.drop(Core::TOK_Identi, Core::STR___operator) ||
               ctx.in.drop(Core::TOK_Identi, Core::STR_operator))
               switch(ctx.in.peek().tok)
            {
            case Core::TOK_AddEq: prop.propAddEq = getStructProp(); break;
            case Core::TOK_AndEq: prop.propAndEq = getStructProp(); break;
            case Core::TOK_DivEq: prop.propDivEq = getStructProp(); break;
            case Core::TOK_Equal: prop.propEqual = getStructProp(); break;
            case Core::TOK_ModEq: prop.propModEq = getStructProp(); break;
            case Core::TOK_MulEq: prop.propMulEq = getStructProp(); break;
            case Core::TOK_OrIEq: prop.propOrIEq = getStructProp(); break;
            case Core::TOK_OrXEq: prop.propOrXEq = getStructProp(); break;
            case Core::TOK_ShLEq: prop.propShLEq = getStructProp(); break;
            case Core::TOK_ShREq: prop.propShREq = getStructProp(); break;
            case Core::TOK_SubEq: prop.propSubEq = getStructProp(); break;

            case Core::TOK_Add2:
               if(ctx.in.peek(Core::TOK_Add2, Core::TOK_Add))
                  {ctx.in.get(); prop.propAdd3 = getStructProp();}
               else
                  prop.propAdd2 = getStructProp();
               break;

            case Core::TOK_Sub2:
               if(ctx.in.peek(Core::TOK_Sub2, Core::TOK_Sub))
                  {ctx.in.get(); prop.propSub3 = getStructProp();}
               else
                  prop.propSub2 = getStructProp();
               break;

            case Core::TOK_ParenO:
               if(ctx.in.peek(Core::TOK_ParenO, Core::TOK_ParenC))
                  {ctx.in.get(); prop.propParen = getStructProp(); break;}
            default:
               Core::ErrorExpect("struct-property-operator", ctx.in.peek());
            }
            else if(ctx.in.peek(Core::TOK_KeyWrd, Core::STR_default))
               prop.prop = getStructProp();
            else
               Core::ErrorExpect("struct-property-name", ctx.in.peek());
         }
         while(ctx.in.drop(Core::TOK_Comma));

         // }
         ctx.expect(Core::TOK_BraceC);

         return prop;
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
               Core::Error(ctx.in.peek().pos, "member after flexible array");

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
                  Core::Error(ctx.in.reget().pos, "incomplete member");

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
                        Core::Error(pos, "name reused");

                     // Check for too many bits to fit in a word.
                     if(bits > bitsWord)
                        Core::Error(pos, "too many bits");

                     // Check for enough size in the current word.
                     if(bitsAddr + bits > bitsWord)
                        bitsReset();

                     // Align underlying storage.
                     bitsAlign();

                     SR::Type::CRef bitsType{attrTemp.type};

                     // bitsType must be qualified or unqualified signed int,
                     // unsigned int, or _Bool.
                     if(!bitsType->isCTypeInteg())
                        Core::Error(pos, "required integer type");

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
                        Core::Error(pos, "named empty bitfield");

                     bitsReset();
                  }
               }

               // Normal data member.
               else
               {
                  if(findName(attrTemp.name))
                     Core::Error(ctx.in.reget().pos, "name reused");

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
                        Core::Error(ctx.in.reget().pos, "incomplete member");

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
            ctx.expect(Core::TOK_Semico);
         }

         else
            Core::ErrorExpect("struct-declaration", ctx.in.peek());
      }
      while(!ctx.in.drop(Core::TOK_BraceC));

      if(memv.empty())
         Core::Error(ctx.in.reget().pos, "empty structure");

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

