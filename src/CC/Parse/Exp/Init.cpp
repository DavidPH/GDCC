//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C initializer parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp.hpp"
#include "CC/Exp/Init.hpp"
#include "CC/Init.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // GetInitRawDes
   //
   static InitRawDes GetInitRawDes(Parser &ctx, Scope &scope)
   {
      if(ctx.in.drop(Core::TOK_Dot))
      {
         return ctx.expectIdenti().str;
      }
      else if(ctx.in.drop(Core::TOK_BrackO))
      {
         InitRawDes desig = ExpToFastU(ctx.getExp_Cond(scope));

         ctx.expect(Core::TOK_BrackC);

         return desig;
      }
      else
         Core::ErrorExpect("designator", ctx.in.peek());
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Parser::getExp_Init
   //
   SR::Exp::CRef Parser::getExp_Init(Scope &scope, SR::Type const *type)
   {
      auto raw = getInitRaw(scope);

      return Exp_Init::Create(Init::Create(raw, *this, scope, type, fact), false);
   }

   //
   // Parser::getInitRaw
   //
   InitRaw Parser::getInitRaw(Scope &scope)
   {
      InitRaw init{in.peek().pos};

      // Bracketed initializer.
      if(in.drop(Core::TOK_BraceO))
      {
         std::vector<InitRaw> sub;

         if(!in.peek(Core::TOK_BraceC)) do
         {
            // Designators.
            if(in.peek(Core::TOK_Dot) || in.peek(Core::TOK_BrackO))
            {
               std::vector<InitRawDes> desig;

               do
                  desig.emplace_back(GetInitRawDes(*this, scope));
               while(!in.drop(Core::TOK_Equal));

               sub.emplace_back(getInitRaw(scope));

               sub.back().desig = {Core::Move, desig.begin(), desig.end()};
            }
            else
               sub.emplace_back(getInitRaw(scope));
         }
         while(in.drop(Core::TOK_Comma) && !in.peek(Core::TOK_BraceC));

         expect(Core::TOK_BraceC);

         init.valueSub = {Core::Move, sub.begin(), sub.end()};
      }

      // Unbracketed initializer.
      else
      {
         // Possible string-initializer.
         if(in.peek().isTokString())
         {
            auto tok = in.get();

            if(in.peek(Core::TOK_Comma) ||
               in.peek(Core::TOK_Semico) ||
               in.peek(Core::TOK_BraceC))
            {
               init.valueTok = tok;
               return init;
            }
            else
               in.unget();
         }

         init.valueExp = getExp_Assi(scope);
      }

      return init;
   }
}

// EOF

