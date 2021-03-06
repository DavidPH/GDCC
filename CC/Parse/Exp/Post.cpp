//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C postfix-expression parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Factory.hpp"

#include "Core/Array.hpp"
#include "Core/TokenStream.hpp"

#include "SR/Exp.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // GetExp_Post_Add2
   //
   static SR::Exp::CRef GetExp_Post_Add2(Parser &ctx, Scope &, SR::Exp const *exp)
   {
      return ctx.fact.expCreate_IncSuf(exp, ctx.in.get().pos);
   }

   //
   // GetExp_Post_BrackO
   //
   static SR::Exp::CRef GetExp_Post_BrackO(Parser &ctx, Scope &scope, SR::Exp const *exp)
   {
      auto pos = ctx.in.get().pos;

      auto idx = ctx.getExp(scope);

      ctx.expect(Core::TOK_BrackC);

      return ctx.fact.expCreate_Array(exp, idx, pos);
   }

   //
   // GetExp_Post_Dot
   //
   static SR::Exp::CRef GetExp_Post_Dot(Parser &ctx, Scope &scope, SR::Exp const *exp)
   {
      auto pos = ctx.in.get().pos;

      return ctx.fact.expCreate_Mem(exp,
         ctx.expectIdenti().str, pos, scope);
   }

   //
   // GetExp_Post_Mem
   //
   static SR::Exp::CRef GetExp_Post_Mem(Parser &ctx, Scope &scope, SR::Exp const *exp)
   {
      auto pos = ctx.in.get().pos;

      return ctx.fact.expCreate_MemPt(exp,
         ctx.expectIdenti().str, pos, scope);
   }

   //
   // GetExp_Post_Sub2
   //
   static SR::Exp::CRef GetExp_Post_Sub2(Parser &ctx, Scope &, SR::Exp const *exp)
   {
      return ctx.fact.expCreate_DecSuf(exp, ctx.in.get().pos);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Parser::getExp_Post_ParenO
   //
   SR::Exp::CRef Parser::getExp_Post_ParenO(Scope &scope, SR::Exp const *exp)
   {
      auto pos = in.get().pos;

      Core::Array<SR::Exp::CRef> args;

      if(!in.peek(Core::TOK_ParenC))
         args = getExpList(scope);

      expect(Core::TOK_ParenC);

      return fact.expCreate_Call(exp, std::move(args), scope, pos);
   }

   //
   // Parser::getExp_Post
   //
   SR::Exp::CRef Parser::getExp_Post(Scope &scope)
   {
      if(isExp_Cast(scope))
         return getExp_Post(scope, getExp_CLit(scope));

      return getExp_Post(scope, getExp_Prim(scope));
   }

   //
   // Parser::getExp_Post
   //
   SR::Exp::CRef Parser::getExp_Post(Scope &scope, SR::Exp const *e)
   {
      SR::Exp::CRef exp{e};

      for(;;) switch(in.peek().tok)
      {
      case Core::TOK_Add2:   exp = GetExp_Post_Add2  (*this, scope, exp); break;
      case Core::TOK_Dot:    exp = GetExp_Post_Dot   (*this, scope, exp); break;
      case Core::TOK_Mem:    exp = GetExp_Post_Mem   (*this, scope, exp); break;
      case Core::TOK_Sub2:   exp = GetExp_Post_Sub2  (*this, scope, exp); break;
      case Core::TOK_BrackO: exp = GetExp_Post_BrackO(*this, scope, exp); break;
      case Core::TOK_ParenO: exp = getExp_Post_ParenO(       scope, exp); break;

      default:
         return exp;
      }
   }
}

// EOF

