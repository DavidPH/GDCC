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

#include "Parse.hpp"

#include "GDCC/TokenStream.hpp"

#include "GDCC/IR/Import.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   //
   // ParseImport
   //
   void ParseImport(GDCC::TokenStream &in, GDCC::IR::Program &,
      GDCC::IR::Import &)
   {
      while(!in.drop(GDCC::TOK_LnEnd)) switch(static_cast<GDCC::StringIndex>(
         ExpectToken(in, GDCC::TOK_Identi, "identifier").get().str))
      {
      default:
         in.unget();
         std::cerr << "ERROR: " << in.peek().pos << ": bad Import argument: '"
            << in.peek().str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

