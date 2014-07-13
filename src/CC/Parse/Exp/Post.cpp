//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

//
// GetExp_Post_Add2
//
static GDCC::AST::Exp::CRef GetExp_Post_Add2(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &, GDCC::AST::Exp const *exp)
{
   return GDCC::CC::ExpCreate_IncSuf(exp, ctx.in.get().pos);
}

//
// GetExp_Post_BrackO
//
static GDCC::AST::Exp::CRef GetExp_Post_BrackO(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope, GDCC::AST::Exp const *exp)
{
   using namespace GDCC;

   auto pos = ctx.in.get().pos;

   auto idx = CC::GetExp(ctx, scope);

   if(!ctx.in.drop(Core::TOK_BrackC))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected ']'");

   return CC::ExpCreate_Array(exp, idx, pos);
}

//
// GetExp_Post_Dot
//
static GDCC::AST::Exp::CRef GetExp_Post_Dot(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &, GDCC::AST::Exp const *exp)
{
   using namespace GDCC;

   auto pos = ctx.in.get().pos;

   if(ctx.in.peek().tok != Core::TOK_Identi)
      throw Core::ExceptStr(ctx.in.peek().pos, "expected identifier");

   return CC::ExpCreate_Mem(exp, ctx.in.get().str, pos);
}

//
// GetExp_Post_Mem
//
static GDCC::AST::Exp::CRef GetExp_Post_Mem(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &, GDCC::AST::Exp const *exp)
{
   using namespace GDCC;

   auto pos = ctx.in.get().pos;

   if(ctx.in.peek().tok != Core::TOK_Identi)
      throw Core::ExceptStr(ctx.in.peek().pos, "expected identifier");

   return CC::ExpCreate_MemPt(exp, ctx.in.get().str, pos);
}

//
// GetExp_Post_ParenO
//
static GDCC::AST::Exp::CRef GetExp_Post_ParenO(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope, GDCC::AST::Exp const *exp)
{
   using namespace GDCC;

   auto pos = ctx.in.get().pos;

   std::vector<AST::Exp::CRef> args;

   if(ctx.in.peek().tok != Core::TOK_ParenC) do
      args.emplace_back(CC::GetExp_Assi(ctx, scope));
   while(ctx.in.drop(Core::TOK_Comma));

   if(!ctx.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

   return CC::ExpCreate_Call(exp,
      Core::Array<AST::Exp::CRef>(args.begin(), args.end()), pos);
}

//
// GetExp_Post_Sub2
//
static GDCC::AST::Exp::CRef GetExp_Post_Sub2(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &, GDCC::AST::Exp const *exp)
{
   return GDCC::CC::ExpCreate_DecSuf(exp, ctx.in.get().pos);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GetExp_Post
      //
      AST::Exp::CRef GetExp_Post(ParserCtx const &ctx, Scope &scope)
      {
         if(IsExp_Cast(ctx, scope))
            return GetExp_Post(ctx, scope, GetExp_CLit(ctx, scope));

         return GetExp_Post(ctx, scope, GetExp_Prim(ctx, scope));
      }

      //
      // GetExp_Post
      //
      AST::Exp::CRef GetExp_Post(ParserCtx const &ctx, Scope &scope,
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

