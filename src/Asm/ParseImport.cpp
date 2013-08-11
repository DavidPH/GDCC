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

#include "IStream.hpp"

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
   void ParseImport(IStream &in, GDCC::IR::Import &)
   {
      for(GDCC::Token tok; in >> tok;) switch(static_cast<GDCC::StringIndex>(tok.str))
      {
      default:
         if(tok.tok == GDCC::TOK_EOL) return;

         std::cerr << "ERROR: " << tok.pos << ": bad Import argument: '"
            << tok.str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

