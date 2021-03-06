//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Space parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Space.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AS
{
   //
   // ParseSpace
   //
   void ParseSpace(ParserCtx const &ctx, IR::Space &space)
   {
      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      while(!ctx.in.drop(Core::TOK_ParenC))
         switch(TokenPeekIdenti(ctx).in.get().str)
      {
      case Core::STR_alloc: space.alloc = GetFastU(TokenDropEq(ctx));   break;
      case Core::STR_defin: space.defin = GetFastU(TokenDropEq(ctx));   break;
      case Core::STR_linka: space.linka = GetLinkage(TokenDropEq(ctx)); break;
      case Core::STR_value: space.value = GetFastU(TokenDropEq(ctx));   break;
      case Core::STR_words: space.words = GetFastU(TokenDropEq(ctx));   break;

      default:
         Core::ErrorExpect("Space argument", ctx.in.reget());
      }
   }
}

// EOF

