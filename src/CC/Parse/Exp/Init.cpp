//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

static GDCC::CC::InitRawDes GetInitRawDes(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope);

//
// GetInitRaw
//
static GDCC::CC::InitRaw GetInitRaw(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   CC::InitRaw init{ctx.in.peek().pos};

   // Bracketed initializer.
   if(ctx.in.drop(Core::TOK_BraceO))
   {
      std::vector<CC::InitRaw> sub;

      do
      {
         // Designators.
         if(ctx.in.peek(Core::TOK_Dot) || ctx.in.peek(Core::TOK_BrackO))
         {
            std::vector<CC::InitRawDes> desig;

            do
               desig.emplace_back(GetInitRawDes(ctx, scope));
            while(!ctx.in.drop(Core::TOK_Equal));

            sub.emplace_back(GetInitRaw(ctx, scope));

            sub.back().desig = {Core::Move, desig.begin(), desig.end()};
         }
         else
            sub.emplace_back(GetInitRaw(ctx, scope));
      }
      while(ctx.in.drop(Core::TOK_Comma) && !ctx.in.peek(Core::TOK_BraceC));

      if(!ctx.in.drop(Core::TOK_BraceC))
         throw Core::ExceptStr(ctx.in.peek().pos, "expected '}'");

      init.valueSub = {Core::Move, sub.begin(), sub.end()};
   }

   // Unbracketed initializer.
   else
   {
      // Possible string-initializer.
      if(ctx.in.peek().isTokString())
      {
         auto tok = ctx.in.get();

         if(ctx.in.peek(Core::TOK_Comma) || ctx.in.peek(Core::TOK_BraceC))
         {
            init.valueTok = tok;
            return init;
         }
         else
            ctx.in.unget();
      }

      init.valueExp = CC::GetExp_Assi(ctx, scope);
   }

   return init;
}

//
// GetInitRawDes
//
static GDCC::CC::InitRawDes GetInitRawDes(GDCC::CC::ParserCtx const &ctx,
   GDCC::CC::Scope &scope)
{
   using namespace GDCC;

   if(ctx.in.drop(Core::TOK_Dot))
   {
      if(!ctx.in.peek(Core::TOK_Identi))
         throw Core::ExceptStr(ctx.in.peek().pos, "expected identifier");

      return ctx.in.get().str;
   }
   else if(ctx.in.drop(Core::TOK_BrackO))
   {
      CC::InitRawDes desig = CC::ExpToFastU(CC::GetExp_Cond(ctx, scope));

      if(!ctx.in.drop(Core::TOK_BrackC))
         throw Core::ExceptStr(ctx.in.peek().pos, "expected ']'");

      return desig;
   }
   else
      throw Core::ExceptStr(ctx.in.peek().pos, "expected designator");
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GetExp_Init
      //
      AST::Exp::CRef GetExp_Init(ParserCtx const &ctx, Scope &scope,
         AST::Type const *type)
      {
         auto raw = GetInitRaw(ctx, scope);

         return Exp_Init::Create(Init::Create(raw, ctx, scope, type), false);
      }
   }
}

// EOF

