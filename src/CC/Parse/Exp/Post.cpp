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
static GDCC::AST::Exp::CRef GetExp_Post_Add2(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *, GDCC::AST::Exp const *exp)
{
   return GDCC::CC::ExpCreate_IncSuf(exp, in.in.get().pos);
}

//
// GetExp_Post_BrackO
//
static GDCC::AST::Exp::CRef GetExp_Post_BrackO(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *ctx, GDCC::AST::Exp const *exp)
{
   using namespace GDCC;

   auto pos = in.in.get().pos;

   auto idx = CC::GetExp(in, ctx);

   if(!in.in.drop(Core::TOK_BrackC))
      throw Core::ExceptStr(in.in.peek().pos, "expected ']'");

   return CC::ExpCreate_Array(exp, idx, pos);
}

//
// GetExp_Post_Dot
//
static GDCC::AST::Exp::CRef GetExp_Post_Dot(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *, GDCC::AST::Exp const *exp)
{
   using namespace GDCC;

   auto pos = in.in.get().pos;

   if(in.in.peek().tok != Core::TOK_Identi)
      throw Core::ExceptStr(in.in.peek().pos, "expected identifier");

   return CC::ExpCreate_Mem(exp, in.in.get().str, pos);
}

//
// GetExp_Post_Mem
//
static GDCC::AST::Exp::CRef GetExp_Post_Mem(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *, GDCC::AST::Exp const *exp)
{
   using namespace GDCC;

   auto pos = in.in.get().pos;

   if(in.in.peek().tok != Core::TOK_Identi)
      throw Core::ExceptStr(in.in.peek().pos, "expected identifier");

   return CC::ExpCreate_MemPt(exp, in.in.get().str, pos);
}

//
// GetExp_Post_ParenO
//
static GDCC::AST::Exp::CRef GetExp_Post_ParenO(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *ctx, GDCC::AST::Exp const *exp)
{
   using namespace GDCC;

   auto pos = in.in.get().pos;

   std::vector<AST::Exp::CRef> args;

   if(in.in.peek().tok != Core::TOK_ParenC) do
      args.emplace_back(CC::GetExp_Assi(in, ctx));
   while(in.in.drop(Core::TOK_Comma));

   if(!in.in.drop(Core::TOK_ParenC))
      throw Core::ExceptStr(in.in.peek().pos, "expected ')'");

   return CC::ExpCreate_Call(exp,
      Core::Array<AST::Exp::CRef>(args.begin(), args.end()), pos);
}

//
// GetExp_Post_Sub2
//
static GDCC::AST::Exp::CRef GetExp_Post_Sub2(GDCC::CC::ParserData &in,
   GDCC::CC::Scope *, GDCC::AST::Exp const *exp)
{
   return GDCC::CC::ExpCreate_DecSuf(exp, in.in.get().pos);
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
      AST::Exp::CRef GetExp_Post(ParserData &in, Scope *ctx)
      {
         if(IsExp_Cast(in, ctx))
            return GetExp_Post(in, ctx, GetExp_CLit(in, ctx));

         return GetExp_Post(in, ctx, GetExp_Prim(in, ctx));
      }

      //
      // GetExp_Post
      //
      AST::Exp::CRef GetExp_Post(ParserData &in, Scope *ctx, AST::Exp const *e)
      {
         AST::Exp::CRef exp{e};

         for(;;) switch(in.in.peek().tok)
         {
         case Core::TOK_Add2:   exp = GetExp_Post_Add2  (in, ctx, exp); break;
         case Core::TOK_Dot:    exp = GetExp_Post_Dot   (in, ctx, exp); break;
         case Core::TOK_Mem:    exp = GetExp_Post_Mem   (in, ctx, exp); break;
         case Core::TOK_Sub2:   exp = GetExp_Post_Sub2  (in, ctx, exp); break;
         case Core::TOK_BrackO: exp = GetExp_Post_BrackO(in, ctx, exp); break;
         case Core::TOK_ParenO: exp = GetExp_Post_ParenO(in, ctx, exp); break;

         default:
            return exp;
         }
      }
   }
}

// EOF

