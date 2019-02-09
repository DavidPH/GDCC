//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Multiplication statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   GDCC_BC_CodeTypeSwitchFn(pre, Mul,)
   GDCC_BC_CodeTypeSwitchFn(pre, MulX, Ux)

   GDCC_BC_CodeTypeSwitchFn(put, Mul,)
   GDCC_BC_CodeTypeSwitchFn(put, MulX, Ux)

   //
   // Info::putStmnt_Mul_A
   //
   void Info::putStmnt_Mul_A()
   {
      auto b = getStmntSize();

      if(b == 0)
         return;

      if(b == 1 || b == 2)
      {
         putCode("MulU");

         putCode("Push_Lit", b * 8);
         putCode("ShRU");

         return;
      }

      auto n = getStmntSizeW(b);

      putStmntCall(getFuncName(stmnt->code, n), n * 2);
   }

   //
   // Info::putStmnt_Mul_R
   //
   void Info::putStmnt_Mul_R()
   {
      auto b = getStmntSize();

      if(b == 0)
         return;

      if(b == 1 || b == 2)
      {
         putCode("Swap");
         putCode("Push_Lit", 32 - b * 8);
         putCode("ShLU");
         putCode("Push_Lit", 32 - b * 8);
         putCode("ShRI");
         putCode("Swap");

         putCode("Push_Lit", 32 - b * 8);
         putCode("ShLU");
         putCode("Push_Lit", 32 - b * 8);
         putCode("ShRI");

         putCode("MulU");

         putCode("Push_Lit", b * 8);
         putCode("ShRI");

         return;
      }

      auto n = getStmntSizeW(b);

      putStmntCall(getFuncName(stmnt->code, n), n * 2);
   }
}

// EOF

