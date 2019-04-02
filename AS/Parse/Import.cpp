//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Import parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/Import.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AS
{
   //
   // ParseImport
   //
   void ParseImport(ParserCtx const &ctx, IR::Import &)
   {
      TokenDrop(ctx, Core::TOK_ParenO, "'('");
      while(!ctx.in.drop(Core::TOK_ParenC))
         switch(TokenPeekIdenti(ctx).in.get().str)
      {
      default:
         Core::ErrorExpect("Import argument", ctx.in.reget());
      }
   }
}

// EOF

