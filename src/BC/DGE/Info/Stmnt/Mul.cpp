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
   //
   // Info::putStmnt_MulA
   //
   void Info::putStmnt_MulA()
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
   // Info::putStmnt_MulR
   //
   void Info::putStmnt_MulR()
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

   //
   // Info::putStmnt_MulU
   //
   void Info::putStmnt_MulU()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         return putCode("MulU");

      putStmntCall(getFuncName(IR::Code::MulU, n), n * 2);
   }

   //
   // Info::trStmnt_MuXU
   //
   void Info::trStmnt_MuXU()
   {
      trStmntStk3(false);
   }
}

// EOF

