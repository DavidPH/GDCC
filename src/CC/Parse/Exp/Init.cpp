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

namespace GDCC
{
   namespace CC
   {
      static InitRawDes GetInitRawDes(ParserCtx const &ctx, Scope &scope);

      //
      // GetInitRaw
      //
      static InitRaw GetInitRaw(ParserCtx const &ctx, Scope &scope)
      {
         InitRaw init{ctx.in.peek().pos};

         // Bracketed initializer.
         if(ctx.in.drop(Core::TOK_BraceO))
         {
            std::vector<InitRaw> sub;

            do
            {
               // Designators.
               if(ctx.in.peek(Core::TOK_Dot) || ctx.in.peek(Core::TOK_BrackO))
               {
                  std::vector<InitRawDes> desig;

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

               if(ctx.in.peek(Core::TOK_Comma) ||
                  ctx.in.peek(Core::TOK_Semico) ||
                  ctx.in.peek(Core::TOK_BraceC))
               {
                  init.valueTok = tok;
                  return init;
               }
               else
                  ctx.in.unget();
            }

            init.valueExp = GetExp_Assi(ctx, scope);
         }

         return init;
      }

      //
      // GetInitRawDes
      //
      static InitRawDes GetInitRawDes(ParserCtx const &ctx, Scope &scope)
      {
         if(ctx.in.drop(Core::TOK_Dot))
         {
            if(!ctx.in.peek(Core::TOK_Identi))
               throw Core::ExceptStr(ctx.in.peek().pos, "expected identifier");

            return ctx.in.get().str;
         }
         else if(ctx.in.drop(Core::TOK_BrackO))
         {
            InitRawDes desig = ExpToFastU(GetExp_Cond(ctx, scope));

            if(!ctx.in.drop(Core::TOK_BrackC))
               throw Core::ExceptStr(ctx.in.peek().pos, "expected ']'");

            return desig;
         }
         else
            throw Core::ExceptStr(ctx.in.peek().pos, "expected designator");
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

