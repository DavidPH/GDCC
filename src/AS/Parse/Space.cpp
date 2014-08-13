//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ParseSpace
      //
      void ParseSpace(ParserCtx const &ctx, IR::Space &space)
      {
         while(!ctx.in.drop(Core::TOK_LnEnd))
            switch(TokenPeekIdenti(ctx).in.get().str)
         {
         case Core::STR_alloc: space.alloc = GetFastU(TokenDropEq(ctx));   break;
         case Core::STR_defin: space.defin = GetFastU(TokenDropEq(ctx));   break;
         case Core::STR_linka: space.linka = GetLinkage(TokenDropEq(ctx)); break;
         case Core::STR_value: space.value = GetFastU(TokenDropEq(ctx));   break;
         case Core::STR_words: space.words = GetFastU(TokenDropEq(ctx));   break;

         default:
            throw Core::ParseExceptExpect(ctx.in.reget(), "Space argument", false);
         }
      }
   }
}

// EOF

