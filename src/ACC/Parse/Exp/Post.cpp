//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS postfix-expression parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "AST/Exp.hpp"
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
      // GetExp_Post_Add2
      //
      static AST::Exp::CRef GetExp_Post_Add2(ParserCtx const &ctx,
         CC::Scope &, AST::Exp const *exp)
      {
         return CC::ExpCreate_IncSuf(exp, ctx.in.get().pos);
      }

      //
      // GetExp_Post_BrackO
      //
      static AST::Exp::CRef GetExp_Post_BrackO(ParserCtx const &ctx,
         CC::Scope &scope, AST::Exp const *exp)
      {
         auto pos = ctx.in.get().pos;

         auto idx = GetExp(ctx, scope);

         if(!ctx.in.drop(Core::TOK_BrackC))
            throw Core::ParseExceptExpect(ctx.in.peek(), "]", true);

         return CC::ExpCreate_Array(exp, idx, pos);
      }

      //
      // GetExp_Post_Dot
      //
      static AST::Exp::CRef GetExp_Post_Dot(ParserCtx const &ctx,
         CC::Scope &, AST::Exp const *exp)
      {
         auto pos = ctx.in.get().pos;

         if(ctx.in.peek().tok != Core::TOK_Identi)
            throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);

         return CC::ExpCreate_Mem(exp, ctx.in.get().str, pos);
      }

      //
      // GetExp_Post_Mem
      //
      static AST::Exp::CRef GetExp_Post_Mem(ParserCtx const &ctx,
         CC::Scope &, AST::Exp const *exp)
      {
         auto pos = ctx.in.get().pos;

         if(ctx.in.peek().tok != Core::TOK_Identi)
            throw Core::ParseExceptExpect(ctx.in.peek(), "identifier", false);

         return CC::ExpCreate_MemPt(exp, ctx.in.get().str, pos);
      }

      //
      // GetExp_Post_ParenO
      //
      static AST::Exp::CRef GetExp_Post_ParenO(ParserCtx const &ctx,
         CC::Scope &scope, AST::Exp const *exp)
      {
         auto pos = ctx.in.get().pos;

         std::vector<AST::Exp::CRef> args;

         if(ctx.in.peek().tok != Core::TOK_ParenC) do
            args.emplace_back(GetExp_Assi(ctx, scope));
         while(ctx.in.drop(Core::TOK_Comma));

         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ParseExceptExpect(ctx.in.peek(), ")", true);

         return CC::ExpCreate_Call(exp,
            Core::Array<AST::Exp::CRef>(args.begin(), args.end()), pos);
      }

      //
      // GetExp_Post_Sub2
      //
      static AST::Exp::CRef GetExp_Post_Sub2(ParserCtx const &ctx,
         CC::Scope &, AST::Exp const *exp)
      {
         return CC::ExpCreate_DecSuf(exp, ctx.in.get().pos);
      }
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // GetExp_Post
      //
      AST::Exp::CRef GetExp_Post(ParserCtx const &ctx, CC::Scope &scope)
      {
         return GetExp_Post(ctx, scope, GetExp_Prim(ctx, scope));
      }

      //
      // GetExp_Post
      //
      AST::Exp::CRef GetExp_Post(ParserCtx const &ctx, CC::Scope &scope,
         AST::Exp const *e)
      {
         AST::Exp::CRef exp{e};

         for(;;) switch(ctx.in.peek().tok)
         {
         case Core::TOK_Add2:   exp = GetExp_Post_Add2  (ctx, scope, exp); break;
         case Core::TOK_Dot:    exp = GetExp_Post_Dot   (ctx, scope, exp); break;
         case Core::TOK_Mem:    exp = GetExp_Post_Mem   (ctx, scope, exp); break;
         case Core::TOK_Sub2:   exp = GetExp_Post_Sub2  (ctx, scope, exp); break;
         case Core::TOK_BrackO: exp = GetExp_Post_BrackO(ctx, scope, exp); break;
         case Core::TOK_ParenO: exp = GetExp_Post_ParenO(ctx, scope, exp); break;

         default:
            return exp;
         }
      }
   }
}

// EOF

