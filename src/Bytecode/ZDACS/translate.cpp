//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code translation.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info::translateBlock
      //
      void Info::translateBlock(GDCC::IR::Block &)
      {
         std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
         throw EXIT_FAILURE;
      }
   }
}

// EOF

