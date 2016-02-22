//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Archive exceptions.
//
//-----------------------------------------------------------------------------

#include "AR/Exception.hpp"

#include <sstream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace AR
   {
      //
      // ReadError::genMsg
      //
      void ReadError::genMsg() const
      {
         std::ostringstream oss;
         oss << "ERROR: ";
         if(pos.file) oss << pos << ": ";
         oss << err;
         auto const &tmp = oss.str();
         msg = Core::StrDup(tmp.data(), tmp.size());
      }
   }
}

// EOF

