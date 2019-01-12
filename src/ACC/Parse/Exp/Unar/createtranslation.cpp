//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS create-translation-expression parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Factory.hpp"
#include "ACC/PropDecl.hpp"

#include "SR/Type.hpp"

#include "CC/Exp.hpp"

#include "Core/Array.hpp"
#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // GetTranslationExp
   //
   static Core::FastU GetTranslationExp(Parser &ctx, CC::Scope &scope,
      std::vector<SR::Exp::CRef> &expv)
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
         ctx.expect(Core::TOK_BrackC);

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
   static SR::Exp::CRef GetTranslation(Parser &ctx, CC::Scope &scope,
      CreateTransDecl const *ctrans)
   {
      // translation:
      //    translation-expression : translation-expression =
      //       translation-prefix(opt) translation-expression : translation-expression

      std::vector<SR::Exp::CRef> expv;

      CreateTransPropName name;

      Core::Origin pos = ctx.in.peek().pos;

      // translation-expression
      name.argc[0] = GetTranslationExp(ctx, scope, expv);

      // :
      ctx.expect(Core::TOK_Colon);

      // translation-expression
      name.argc[1] = GetTranslationExp(ctx, scope, expv);

      // =
      ctx.expect(Core::TOK_Equal);

      // translation-prefix:
      //    %
      if(ctx.in.peek(Core::TOK_Mod))
         name.prefix = ctx.in.get();
      else
         name.prefix = Core::TokenEOF;

      // translation-expression
      name.argc[2] = GetTranslationExp(ctx, scope, expv);

      // :
      ctx.expect(Core::TOK_Colon);

      // translation-expression
      name.argc[3] = GetTranslationExp(ctx, scope, expv);

      auto propItr = ctrans->props.find(name);
      if(propItr == ctrans->props.end())
         Core::Error(pos, "no matching translation function");

      return ctx.fact.expCreate_Call(propItr->second.prop,
         {expv.begin(), expv.end()}, scope, pos);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Parser::getExp_Unar_createtranslation
   //
   SR::Exp::CRef Parser::getExp_Unar_createtranslation(CC::Scope &scope,
      CreateTransDecl const *ctrans)
   {
      // create-translation-expression:
      //    <createtranslation> ( assignment-expression )
      //    <createtranslation> ( assignment-expression , translation-list )

      // <createtranslation>
      auto pos = expect(Core::TOK_KeyWrd, Core::STR_createtranslation).pos;

      // (
      expect(Core::TOK_ParenO);

      // Start with a no-op expression.
      auto exp = SR::ExpCreate_Size(0);

      if(ctrans->propBegin)
      {
         Core::FastU argc = ctrans->propBegin->getType()->getBaseType()->getParameters()->size();

         std::vector<SR::Exp::CRef> argv;

         if(argc)
         {
            argv.emplace_back(getExp_Cond(scope));

            while(--argc)
            {
               // ,
               expect(Core::TOK_Comma);

               argv.emplace_back(getExp_Cond(scope));
            }
         }

         exp = fact.expCreate_Comma(exp, fact.expCreate_Call(ctrans->propBegin,
            {argv.begin(), argv.end()}, scope, pos), pos);
      }

      // translation-list:
      //    translation
      //    translation-list , translation
      while(in.drop(Core::TOK_Comma))
         exp = fact.expCreate_Comma(exp, GetTranslation(*this, scope, ctrans), pos);

      if(ctrans->propEnd)
      {
         exp = fact.expCreate_Comma(exp,
            fact.expCreate_Call(ctrans->propEnd, {}, scope, pos), pos);
      }

      // )
      expect(Core::TOK_ParenC);

      return exp;
   }
}

// EOF

