//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ParseGlyph
      //
      void ParseGlyph(ParserCtx const &ctx, IR::GlyphData &glyph)
      {
         while(!ctx.in.drop(Core::TOK_LnEnd))
            switch(TokenPeekIdenti(ctx).in.get().str)
         {
         case Core::STR_type:  glyph.type  = GetType(TokenDropEq(ctx));  break;
         case Core::STR_value: glyph.value = GetExp(TokenDropEq(ctx)); break;

         default:
            throw Core::ParseExceptExpect(ctx.in.reget(), "Glyph argument", false);
         }
      }
   }
}

// EOF

