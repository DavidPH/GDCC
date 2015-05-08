//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C postfix-expression parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp.hpp"

#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "Core/Array.hpp"
#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GetExp_Post_Add2
      //
      static AST::Exp::CRef GetExp_Post_Add2(Parser &ctx, Scope &, AST::Exp const *exp)
      {
         return ExpCreate_IncSuf(exp, ctx.in.get().pos);
      }

      //
      // GetExp_Post_BrackO
      //
      static AST::Exp::CRef GetExp_Post_BrackO(Parser &ctx, Scope &scope, AST::Exp const *exp)
      {
         auto pos = ctx.in.get().pos;

         auto idx = ctx.getExp(scope);

         if(!ctx.in.drop(Core::TOK_BrackC))
            throw Core::ParseExceptExpect(ctx.in.peek(), "]", true);

         return ExpCreate_Array(exp, idx, pos);
      }

      //
      // GetExp_Post_Dot
      //
      static AST::Exp::CRef GetExp_Post_Dot(Parser &ctx, Scope &, AST::Exp const *exp)
      {
         auto pos = ctx.in.get().pos;

         if(ctx.in.peek().tok != Core::TOK_Identi)
            throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);

         return ExpCreate_Mem(exp, ctx.in.get().str, pos);
      }

      //
      // GetExp_Post_Mem
      //
      static AST::Exp::CRef GetExp_Post_Mem(Parser &ctx, Scope &, AST::Exp const *exp)
      {
         auto pos = ctx.in.get().pos;

         if(ctx.in.peek().tok != Core::TOK_Identi)
            throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);

         return ExpCreate_MemPt(exp, ctx.in.get().str, pos);
      }

      //
      // GetExp_Post_Sub2
      //
      static AST::Exp::CRef GetExp_Post_Sub2(Parser &ctx, Scope &, AST::Exp const *exp)
      {
         return ExpCreate_DecSuf(exp, ctx.in.get().pos);
      }
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Parser::getExp_Post_ParenO
      //
      AST::Exp::CRef Parser::getExp_Post_ParenO(Scope &scope, AST::Exp const *exp)
      {
         auto pos = in.get().pos;

         Core::Array<AST::Exp::CRef> args;

         if(!in.peek(Core::TOK_ParenC))
            args = getExpList(scope);

         if(!in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(in.peek(), ")", true);

         return ExpCreate_Call(exp, args, pos);
      }

      //
      // Parser::getExp_Post
      //
      AST::Exp::CRef Parser::getExp_Post(Scope &scope)
      {
         if(isExp_Cast(scope))
            return getExp_Post(scope, getExp_CLit(scope));

         return getExp_Post(scope, getExp_Prim(scope));
      }

      //
      // Parser::getExp_Post
      //
      AST::Exp::CRef Parser::getExp_Post(Scope &scope, AST::Exp const *e)
      {
         AST::Exp::CRef exp{e};

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
}

// EOF

