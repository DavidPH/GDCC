//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
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

namespace GDCC::AR
{
   //
   // Error
   //
   void Error(char const *str)
   {
      Core::Error({}, str);
   }
}

// EOF

