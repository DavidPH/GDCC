//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code output.
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
      // Info::put
      //
      void Info::put(std::ostream &)
      {
         std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
         throw EXIT_FAILURE;
      }
   }
}

// EOF

