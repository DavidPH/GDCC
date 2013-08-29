//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression value comparison operators.
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
      GDCC_IR_ValueCmpOpBitsImpl(<)
      GDCC_IR_ValueCmpOpBitsImpl(>)
      GDCC_IR_ValueCmpOpBitsImpl(<=)
      GDCC_IR_ValueCmpOpBitsImpl(>=)

      GDCC_IR_ValueCmpOpBitsImpl(==)
      GDCC_IR_ValueCmpOpBitsImpl(!=)
   }
}

// EOF

