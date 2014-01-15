//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source position/origin information.
//
//-----------------------------------------------------------------------------

#include "Core/Origin.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // operator std::ostream << Origin
      //
      std::ostream &operator << (std::ostream &out, Origin const &in)
      {
         return out << in.file << ':' << in.line;
      }
   }
}

// EOF

