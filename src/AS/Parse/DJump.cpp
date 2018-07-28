//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
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


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AS
{
   //
   // ParseDJump
   //
   void ParseDJump(ParserCtx const &ctx, IR::DJump &jump)
   {
      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      while(ctx.in.drop(Core::TOK_ParenC))
         switch(TokenPeekIdenti(ctx).in.get().str)
      {
      case Core::STR_alloc: jump.alloc = GetFastU(TokenDropEq(ctx));  break;
      case Core::STR_defin: jump.defin = GetFastU(TokenDropEq(ctx));  break;
      case Core::STR_label: jump.label = GetString(TokenDropEq(ctx)); break;
      case Core::STR_value: jump.value = GetFastU(TokenDropEq(ctx));  break;

      default:
         Core::ErrorExpect("DJump argument", ctx.in.reget());
      }
   }
}

// EOF

