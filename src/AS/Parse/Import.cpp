//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Import parsing utilities.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/TokenStream.hpp"

#include "IR/Import.hpp"

#include <iostream>


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
      void ParseImport(Core::TokenStream &in, IR::Program &, IR::Import &)
      {
         while(!in.drop(Core::TOK_LnEnd)) switch(static_cast<Core::StringIndex>(
            ExpectToken(in, Core::TOK_Identi, "identifier").get().str))
         {
         default:
            in.unget();
            std::cerr << "ERROR: " << in.peek().pos << ": bad Import argument: '"
               << in.peek().str << "'\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

