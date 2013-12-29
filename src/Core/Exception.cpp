//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Base GDCC exception classes.
//
//-----------------------------------------------------------------------------

#include "Core/Exception.hpp"

#include <sstream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // Exception::genMsg
      //
      void Exception::genMsg() const
      {
         std::ostringstream oss;
         oss << "ERROR: " << pos << ": unknown error";
         auto const &tmp = oss.str();
         msg = StrDup(tmp.data(), tmp.size());
      }

      //
      // ExceptStr::genMsg
      //
      void ExceptStr::genMsg() const
      {
         std::ostringstream oss;
         oss << "ERROR: " << pos << ": " << str;
         auto const &tmp = oss.str();
         msg = StrDup(tmp.data(), tmp.size());
      }
   }
}

// EOF

