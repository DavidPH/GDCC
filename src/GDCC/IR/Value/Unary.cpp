//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression value unary operators.
//
//-----------------------------------------------------------------------------

#include "Util.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // operator - Value_Fixed
      //
      Value_Fixed operator - (Value_Fixed const &e)
      {
         return Value_Fixed(-e.value, e.vtype).clamp();
      }

      //
      // operator - Value_Fixed
      //
      Value_Fixed &&operator - (Value_Fixed &&e)
      {
         e.value = -e.value;

         return std::move(e.clamp());
      }

      //
      // operator ~ Value_Fixed
      //
      Value_Fixed operator ~ (Value_Fixed const &e)
      {
         return Value_Fixed(~e.value, e.vtype);
      }

      //
      // operator ~ Value_Fixed
      //
      Value_Fixed &&operator ~ (Value_Fixed &&e)
      {
         e.value = ~e.value;

         return std::move(e.clamp());
      }
   }
}

// EOF

