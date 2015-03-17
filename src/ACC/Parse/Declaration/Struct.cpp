//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS structure-declaration parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Scope.hpp"

#include "AST/Attribute.hpp"
#include "AST/Statement.hpp"

#include "CC/Type/Struct.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // GetDeclStruct
      //
      static CC::Type_Struct::Ref GetDeclStruct(Core::Token name,
         Scope_Global &scope)
      {
         auto lookup = scope.lookupTypeTag(name.str);

         if(!lookup)
         {
            auto type = CC::Type_Struct::Create(name.str);
            scope.addTypeTag(name.str, type);
            return type;
         }

         if(!lookup->isCTypeStruct())
            throw Core::ExceptStr(name.pos, "expected structure type");

         return static_cast<CC::Type_Struct::Ref>(&*lookup);
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // GetDecl_Struct
      //
      AST::Statement::CRef GetDecl_Struct(ParserCtx const &ctx,
         Scope_Global &scope)
      {
         // structure-declaration:
         //    <struct> identifier { structure-member-declaration-sequence }
         //    <struct> identifier ;

         // <struct>
         if(!ctx.in.peek(Core::TOK_KeyWrd, Core::STR_struct))
            throw Core::ParseExceptExpect(ctx.in.peek(), "structure-declaration", false);

         auto pos = ctx.in.get().pos;

         // identifier
         if(!ctx.in.peek(Core::TOK_Identi))
            throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);

         auto name = ctx.in.get();

         auto type = GetDeclStruct(name, scope);

         scope.add(name.str, type);

         // ;
         if(ctx.in.drop(Core::TOK_Semico))
            return AST::StatementCreate_Empty(pos);

         // {
         if(!ctx.in.drop(Core::TOK_BraceO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "{", true);

         // List of members.
         std::vector<CC::Type_Struct::MemberData> memv;

         // Current address in structure.
         Core::FastU addr = 0;

         // Current size of structure.
         Core::FastU size = 0;

         //
         // addrAlign
         //
         auto addrAlign = [&](Core::FastU sizeAlign)
         {
            size = addr = (addr + sizeAlign - 1) / sizeAlign * sizeAlign;
         };

         //
         // addrBytes
         //
         auto addrBytes = [&](Core::FastU sizeBytes)
         {
            size = addr += sizeBytes;
         };

         //
         // findName
         //
         auto findName = [&memv](Core::String memName) -> bool
         {
            if(!memName) return false;

            for(auto const &mem : memv)
               if(mem.name == memName) return true;

            return false;
         };

         // structure-member-declaration-sequence:
         //    structure-member-declaration
         //    structure-member-declaration-sequence structure-member-declaration
         do
         {
            // struct-member-declaration:
            //    specifier-qualifier-sequence structure-member-declarator-list ;

            if(!IsSpecQual(ctx, scope))
               throw Core::ExceptStr(ctx.in.peek().pos,
                  "expected structure-member-declaration");

            // specifier-qualifier-list
            AST::Attribute attrBase;
            ParseSpecQual(ctx, scope, attrBase);

            // structure-member-declarator-list:
            //    structure-member-declarator
            //    structure-member-declarator-list , structure-member-declarator
            do
            {
               // structure-member-declarator:
               //    declarator

               AST::Attribute attr = attrBase;

               ParseDeclarator(ctx, scope, attr);

               if(findName(attr.name))
                  throw Core::ExceptStr(attr.namePos, "name reused");

               if(!attr.type->isTypeComplete())
                  throw Core::ExceptStr(attr.namePos, "incomplete member");

               addrAlign(attr.type->getSizeAlign());
               memv.emplace_back(attr.name, attr.type, addr, false);
               addrBytes(attr.type->getSizeBytes());
            }
            while(ctx.in.drop(Core::TOK_Comma));

            // ;
            if(!ctx.in.drop(Core::TOK_Semico))
               throw Core::ExceptStr(ctx.in.peek().pos, "expected ';'");
         }
         while(!ctx.in.drop(Core::TOK_BraceC));

         if(memv.empty())
            throw Core::ExceptStr(pos, "empty structure");

         if(!type->isTypeComplete())
            type->setMembers(memv.data(), memv.size(), size);

         return AST::StatementCreate_Empty(pos);
      }
   }
}

// EOF

