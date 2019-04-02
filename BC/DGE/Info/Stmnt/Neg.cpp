//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Negation statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   GDCC_BC_CodeTypeSwitchFn(pre, Neg, FIU)

   GDCC_BC_CodeTypeSwitchFn(put, Neg, FIU)

   //
   // Info::putStmnt_Neg_F
   //
   void Info::putStmnt_Neg_F()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      putCode("Push_Lit", 0x80000000);
      putCode("BOrX");
   }

   //
   // Info::putStmnt_Neg_I
   //
   void Info::putStmnt_Neg_I()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         return putCode("NegI");

      putStmntCall(getFuncName(IR::CodeBase::Neg+'I', n), n);
   }
}

// EOF

