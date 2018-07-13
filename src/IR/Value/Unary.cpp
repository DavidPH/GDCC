//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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
// Extern Functions                                                           |
//

namespace GDCC::IR
{
   //
   // operator - Value
   //
   Value operator - (Value const &e)
   {
      if(e.v == ValueBase::Fixed) return -e.vFixed;
      if(e.v == ValueBase::Float) return -e.vFloat;

      throw TypeError();
   }

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
   // operator - Value_Float
   //
   Value_Float operator - (Value_Float const &e)
   {
      return Value_Float(-e.value, e.vtype).clamp();
   }

   //
   // operator - Value_Float
   //
   Value_Float &&operator - (Value_Float &&e)
   {
      e.value = -e.value;

      return std::move(e.clamp());
   }

   //
   // operator ~ Value
   //
   Value operator ~ (Value const &e)
   {
      if(e.v == ValueBase::Fixed) return ~e.vFixed;

      throw TypeError();
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

// EOF

