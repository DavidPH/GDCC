//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Division statement handling.
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
   // Info::putStmnt_DivA
   //
   void Info::putStmnt_DivA()
   {
      auto b = getStmntSize();

      if(b == 0)
         return;

      if(b == 1 || b == 2)
      {
         putCode("Swap");
         putCode("Push_Lit", b * 8);
         putCode("ShLU");
         putCode("Swap");

         putCode("DivU");

         return;
      }

      auto n = getStmntSizeW(b);

      putStmntCall(getFuncName(stmnt->code, n), n * 2);
   }

   //
   // Info::putStmnt_DivR
   //
   void Info::putStmnt_DivR()
   {
      auto b = getStmntSize();

      if(b == 0)
         return;

      if(b == 1 || b == 2)
      {
         putCode("Swap");
         putCode("Push_Lit", 32 - b * 8);
         putCode("ShLU");
         if(b == 1)
         {
            putCode("Push_Lit", 16);
            putCode("ShRI");
         }
         putCode("Swap");

         putCode("Push_Lit", 32 - b * 8);
         putCode("ShLU");
         putCode("Push_Lit", 32 - b * 8);
         putCode("ShRI");

         putCode("DivU");

         return;
      }

      auto n = getStmntSizeW(b);

      putStmntCall(getFuncName(stmnt->code, n), n * 2);
   }

   //
   // Info::putStmnt_DivU
   //
   void Info::putStmnt_DivU(char const *code, IR::Code codeX, bool mod)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         return putCode(code);

      putStmntCall(getFuncName(codeX, n), n * 2);
      if(mod) putStmntDropTmp(0, n);
      for(Core::FastU i = n; i--;) putCode("Drop_Nul");
      if(mod) putStmntPushTmp(0, n);
   }

   //
   // Info::trStmnt_DiXU
   //
   void Info::trStmnt_DiXU()
   {
      trStmntStk3(true);
   }

   //
   // Info::trStmnt_ModU
   //
   void Info::trStmnt_ModU()
   {
      auto n = getStmntSizeW();

      trStmntStk3(true);

      if(n > 1)
         trStmntTmp(n);
   }
}

// EOF

