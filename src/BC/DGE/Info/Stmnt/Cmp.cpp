//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Compare statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   GDCC_BC_CodeTypeSwitchFn(pre, CmpEQ, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, CmpGE, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, CmpGT, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, CmpLE, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, CmpLT, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, CmpNE, FIU)

   GDCC_BC_CodeTypeSwitchFn(put, CmpEQ, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, CmpGE, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, CmpGT, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, CmpLE, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, CmpLT, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, CmpNE, FIU)

   //
   // Info::putStmnt_CmpEQ_U
   //
   void Info::putStmnt_CmpEQ_U(IR::Code code)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return putCode("Push_Lit", code == IR::CodeBase::CmpEQ+'U');

      if(n == 1)
         return putCode(code == IR::CodeBase::CmpEQ+'U' ? "CmpU_EQ" : "CmpU_NE");

      putStmntCall(getFuncName(code, n), n * 2);
   }
}

// EOF

