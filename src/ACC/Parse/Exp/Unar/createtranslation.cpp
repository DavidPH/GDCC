//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS create-translation-expression parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/PropDecl.hpp"

#include "AST/Type.hpp"

#include "CC/Exp.hpp"

#include "Core/Array.hpp"
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
      // GetTranslationExp
      //
      static Core::FastU GetTranslationExp(Parser &ctx, CC::Scope &scope,
         std::vector<AST::Exp::CRef> &expv)
      {
         // translation-expression:
         //    conditional-expression
         //    [ expression-list ]

         if(ctx.in.drop(Core::TOK_BrackO))
         {
            Core::FastU argc = 0;
            do
            {
               expv.emplace_back(ctx.getExp_Assi(scope));
               ++argc;
            }
            while(ctx.in.drop(Core::TOK_Comma));

            // ]
            if(!ctx.in.drop(Core::TOK_BrackC))
               throw Core::ParseExceptExpect(ctx.in.peek(), "]", true);

            return argc;
         }
         else
         {
            expv.emplace_back(ctx.getExp_Cond(scope));
            return 1;
         }
      }

      //
      // GetTranslation
      //
      static AST::Exp::CRef GetTranslation(Parser &ctx, CC::Scope &scope,
         CreateTransDecl const *ctrans)
      {
         // translation:
         //    translation-expression : translation-expression =
         //       translation-prefix(opt) translation-expression : translation-expression

         std::vector<AST::Exp::CRef> expv;

         CreateTransPropName name;

         Core::Origin pos = ctx.in.peek().pos;

         // translation-expression
         name.argc[0] = GetTranslationExp(ctx, scope, expv);

         // :
         if(!ctx.in.drop(Core::TOK_Colon))
            throw Core::ParseExceptExpect(ctx.in.peek(), ":", true);

         // translation-expression
         name.argc[1] = GetTranslationExp(ctx, scope, expv);

         // =
         if(!ctx.in.drop(Core::TOK_Equal))
            throw Core::ParseExceptExpect(ctx.in.peek(), ":", true);

         // translation-prefix:
         //    %
         if(ctx.in.peek(Core::TOK_Mod))
            name.prefix = ctx.in.get();
         else
            name.prefix = Core::TokenEOF;

         // translation-expression
         name.argc[2] = GetTranslationExp(ctx, scope, expv);

         // :
         if(!ctx.in.drop(Core::TOK_Colon))
            throw Core::ParseExceptExpect(ctx.in.peek(), ":", true);

         // translation-expression
         name.argc[3] = GetTranslationExp(ctx, scope, expv);

         auto propItr = ctrans->props.find(name);
         if(propItr == ctrans->props.end())
            throw Core::ExceptStr(pos, "no matching translation function");

         return CC::ExpCreate_Call(propItr->second.prop, {expv.begin(), expv.end()}, pos);
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
      // Parser::getExp_Unar_createtranslation
      //
      AST::Exp::CRef Parser::getExp_Unar_createtranslation(CC::Scope &scope,
         CreateTransDecl const *ctrans)
      {
         // create-translation-expression:
         //    <createtranslation> ( assignment-expression )
         //    <createtranslation> ( assignment-expression , translation-list )

         // <createtranslation>
         auto pos = in.get().pos;

         // (
         if(!in.drop(Core::TOK_ParenO))
            throw Core::ParseExceptExpect(in.peek(), "(", true);

         // Start with a no-op expression.
         auto exp = AST::ExpCreate_Size(0);

         if(ctrans->propBegin)
         {
            Core::FastU argc = ctrans->propBegin->getType()->getBaseType()->getParameters()->size();

            std::vector<AST::Exp::CRef> argv;

            if(argc)
            {
               argv.emplace_back(getExp_Cond(scope));

               while(--argc)
               {
                  // ,
                  if(!in.drop(Core::TOK_Comma))
                     throw Core::ParseExceptExpect(in.peek(), ",", true);

                  argv.emplace_back(getExp_Cond(scope));
               }
            }

            exp = CC::ExpCreate_Comma(exp, CC::ExpCreate_Call(ctrans->propBegin,
               {argv.begin(), argv.end()}, pos), pos);
         }

         // translation-list:
         //    translation
         //    translation-list , translation
         while(in.drop(Core::TOK_Comma))
            exp = CC::ExpCreate_Comma(exp, GetTranslation(*this, scope, ctrans), pos);

         if(ctrans->propEnd)
         {
            exp = CC::ExpCreate_Comma(exp,
               CC::ExpCreate_Call(ctrans->propEnd, {}, pos), pos);
         }

         // )
         if(!in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(in.peek(), ")", true);

         return exp;
      }
   }
}

// EOF

