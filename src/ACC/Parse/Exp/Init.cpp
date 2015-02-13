//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS initializer parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

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
   namespace ACC
   {
      //
      // GetInitRaw
      //
      static CC::InitRaw GetInitRaw(ParserCtx const &ctx, CC::Scope &scope)
      {
         CC::InitRaw init{ctx.in.peek().pos};

         // Bracketed initializer.
         if(ctx.in.drop(Core::TOK_BraceO))
         {
            std::vector<CC::InitRaw> sub;

            do
            {
               sub.emplace_back(GetInitRaw(ctx, scope));
            }
            while(ctx.in.drop(Core::TOK_Comma) && !ctx.in.peek(Core::TOK_BraceC));

            if(!ctx.in.drop(Core::TOK_BraceC))
               throw Core::ParseExceptExpect(ctx.in.peek(), "}", true);

            init.valueSub = {Core::Move, sub.begin(), sub.end()};
         }

         // Unbracketed initializer.
         else
         {
            init.valueExp = GetExp_Assi(ctx, scope);
         }

         return init;
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
      // GetExp_Init
      //
      AST::Exp::CRef GetExp_Init(ParserCtx const &ctx, CC::Scope &scope,
         AST::Type const *type)
      {
         auto raw = GetInitRaw(ctx, scope);

         return CC::Exp_Init::Create(CC::Init::Create(raw, ctx, scope, type), false);
      }
   }
}

// EOF

