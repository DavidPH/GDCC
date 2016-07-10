//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS create-translation-declaration parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Scope.hpp"

#include "CC/Exp.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "SR/Statement.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      static void ParseDecl_CreateTransProp(Parser &ctx, CC::Scope &scope,
         CreateTransDecl &decl)
      {
         CreateTransPropName name;

         // create-translation-property:
         //    create-translation-property-prefix(opt) (
         //       create-translation-property-name ) : constant-expression

         // create-translation-property-prefix(opt)
         if(!ctx.in.peek(Core::TOK_ParenO))
            name.prefix = ctx.in.get();
         else
            name.prefix = Core::TokenEOF;

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(ctx.in.peek(), "(", true);

         // create-translation-property-name:
         //    identifier
         //    constant-expression : constant-expression = constant-expression
         //       : constant-expression
         Core::String nameSpecial;
         Core::Origin namePos = ctx.in.peek().pos;
         if(ctx.in.peek(Core::TOK_Identi, Core::TOK_ParenC))
         {
            nameSpecial = ctx.in.get().str;
         }
         else
         {
            nameSpecial = nullptr;

            // constant-expression
            name.argc[0] = CC::ExpToFastU(ctx.getExp_Cond(scope));

            // :
            if(!ctx.in.drop(Core::TOK_Colon))
               throw Core::ParseExceptExpect(ctx.in.peek(), ":", true);

            // constant-expression
            name.argc[1] = CC::ExpToFastU(ctx.getExp_Cond(scope));

            // =
            if(!ctx.in.drop(Core::TOK_Equal))
               throw Core::ParseExceptExpect(ctx.in.peek(), "=", true);

            // constant-expression
            name.argc[2] = CC::ExpToFastU(ctx.getExp_Cond(scope));

            // :
            if(!ctx.in.drop(Core::TOK_Colon))
               throw Core::ParseExceptExpect(ctx.in.peek(), ":", true);

            // constant-expression
            name.argc[3] = CC::ExpToFastU(ctx.getExp_Cond(scope));
         }

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

         // :
         if(!ctx.in.drop(Core::TOK_Colon))
            throw Core::ParseExceptExpect(ctx.in.peek(), ":", true);

         // constant-expression;
         auto exp = CC::ExpPromo_LValue(ctx.getExp_Assi(scope));

         auto type = exp->getType();

         if(!type->isTypePointer() || !(type = type->getBaseType())->isTypeFunction())
            throw Core::ExceptStr(exp->pos, "expected function type");

         if(nameSpecial) switch(nameSpecial)
         {
         case Core::STR_begin: decl.propBegin = exp; break;
         case Core::STR_end:   decl.propEnd   = exp; break;
         default:
            throw Core::ExceptStr(namePos, "unrecognized special name");
         }
         else
         {
            decl.props[name].prop = exp;
         }
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
      // Parser::getDecl_CreateTrans
      //
      SR::Statement::CRef Parser::getDecl_CreateTrans(Scope_Global &scope)
      {
         if(!in.peek(Core::TOK_KeyWrd, Core::STR_createtranslation))
            throw Core::ParseExceptExpect(in.peek(),
               "create-translation-declaration", false);

         auto pos = in.get().pos;

         if(!in.peek(Core::TOK_Identi))
            throw Core::ParseExceptExpect(in.peek(), "identifier", false);

         auto name = in.get().str;

         if(!in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(in.peek(), "(", true);

         CreateTransDecl decl;

         do
            ParseDecl_CreateTransProp(*this, scope, decl);
         while(in.drop(Core::TOK_Comma));

         if(!in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(in.peek(), ")", true);

         if(!in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(in.peek(), ";", true);

         scope.addCreateTrans(name, std::move(decl));

         return SR::StatementCreate_Empty(pos);
      }
   }
}

// EOF

