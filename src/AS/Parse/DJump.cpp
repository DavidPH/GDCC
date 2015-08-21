//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// DJump parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/DJump.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ParseDJump
      //
      void ParseDJump(ParserCtx const &ctx, IR::DJump &jump)
      {
         while(!ctx.in.drop(Core::TOK_LnEnd))
            switch(TokenPeekIdenti(ctx).in.get().str)
         {
         case Core::STR_alloc: jump.alloc = GetFastU(TokenDropEq(ctx));  break;
         case Core::STR_defin: jump.defin = GetFastU(TokenDropEq(ctx));  break;
         case Core::STR_label: jump.label = GetString(TokenDropEq(ctx)); break;
         case Core::STR_value: jump.value = GetFastU(TokenDropEq(ctx));  break;

         default:
            throw Core::ParseExceptExpect(ctx.in.reget(), "DJump argument", false);
         }
      }
   }
}

// EOF

