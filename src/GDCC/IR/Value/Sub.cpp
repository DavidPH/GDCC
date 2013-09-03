//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression value operator -.
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
      GDCC_IR_ValueBinOpImplEq(-, Fixed);
      GDCC_IR_ValueBinOpImplEq(-, Float);

      //
      // operator Value_Fixed -= Value_Fixed
      //
      Value_Fixed &operator -= (Value_Fixed &l, Value_Fixed const &r)
      {
         GDCC_IR_ValueBinOpBitsOp(-);

         return l.clamp();
      }

      //
      // operator Value_Float -= Value_Float
      //
      Value_Float &operator -= (Value_Float &l, Value_Float const &r)
      {
         GDCC_IR_ValueBinOpFltsOp(-);

         return l.clamp();
      }
   }
}

// EOF

