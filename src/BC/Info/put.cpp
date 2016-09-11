//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Generic output functions.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      //
      // putData
      //
      void Info::putData(char const *data, std::size_t size)
      {
         out->write(data, size);
         putPos += size;
      }
   }
}

// EOF

