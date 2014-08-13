//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // ParseImport
      //
      void ParseImport(ParserCtx const &ctx, IR::Import &)
      {
         while(!ctx.in.drop(Core::TOK_LnEnd))
            switch(TokenPeekIdenti(ctx).in.get().str)
         {
         default:
            throw Core::ParseExceptExpect(ctx.in.reget(), "Import argument", false);
         }
      }
   }
}

// EOF

