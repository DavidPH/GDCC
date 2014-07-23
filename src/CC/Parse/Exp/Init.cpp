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

#include "Core/TokenStream.hpp"


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
         auto pos = ctx.in.peek().pos;

         // Bracketed or string initializer.
         if(ctx.in.peek(Core::TOK_BraceO) || Init::IsInitString(ctx.in.peek(), type))
         {
            auto init = Init::Create(type, pos);
            init->parse(ctx, scope);
            return Exp_Init::Create(std::move(init), false);
         }

         // Unbracketed initializer.
         return ExpPromo_Assign(type, GetExp_Assi(ctx, scope), pos);
      }
   }
}

// EOF

