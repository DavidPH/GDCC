//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Shift statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   GDCC_BC_CodeTypeSwitchFn(pre, ShL, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, ShR, FIU)

   GDCC_BC_CodeTypeSwitchFn(put, ShL, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, ShR, FIU)
}

// EOF

