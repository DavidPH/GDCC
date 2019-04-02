//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// GlyphData parsing.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AS
{
   //
   // ParseGlyph
   //
   void ParseGlyph(ParserCtx const &ctx, IR::GlyphData &glyph)
   {
      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      while(!ctx.in.drop(Core::TOK_ParenC))
         switch(TokenPeekIdenti(ctx).in.get().str)
      {
      case Core::STR_type:  glyph.type  = GetType(TokenDropEq(ctx)); break;
      case Core::STR_value: glyph.value = GetExp(TokenDropEq(ctx));  break;

      default:
         Core::ErrorExpect("Glyph argument", ctx.in.reget());
      }
   }
}

// EOF

