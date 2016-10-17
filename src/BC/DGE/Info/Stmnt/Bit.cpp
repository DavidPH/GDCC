//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Bitwise statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::preStmnt_Bclo_W
   //
   void Info::preStmnt_Bclo_W()
   {
      if(stmnt->op.size <= 1)
         return;

      addFunc_Bclo_W(stmnt->op.size);
   }

   //
   // Info::preStmnt_Bclz_W
   //
   void Info::preStmnt_Bclz_W()
   {
      if(stmnt->op.size <= 1)
         return;

      addFunc_Bclz_W(stmnt->op.size);
   }

   //
   // Info::putStmnt_BAnd_W
   //
   void Info::putStmnt_BAnd_W(char const *code)
   {
      if(stmnt->args[1].a == IR::ArgBase::Stk)
      {
         if(stmnt->op.size > 1)
         {
            for(Core::FastU i = 0, e = stmnt->op.size * 2 - 1; i != e; ++i)
               putCode("Drop_Reg", (func->localReg + i) * 4);

            putCode("Push_Reg", (func->localReg + stmnt->op.size - 1) * 4);
         }

         putCode(code);

         if(stmnt->op.size > 1) for(Core::FastU i = stmnt->op.size - 1; i--;)
         {
            putCode("Push_Reg", (func->localReg + stmnt->op.size + i) * 4);
            putCode("Push_Reg", (func->localReg + i) * 4);
            putCode(code);
         }
      }
      else
      {
         for(Core::FastU i = 0; i != stmnt->op.size; ++i)
         {
            putStmntPushArg(stmnt->args[1], i);
            putStmntPushArg(stmnt->args[2], i);
            putCode(code);
         }
      }
   }

   //
   // Info::putStmnt_Bclz_W
   //
   void Info::putStmnt_Bclz_W(char const *code)
   {
      if(stmnt->op.size == 0)
         return putCode("Push_Lit", 0);

      if(stmnt->op.size == 1)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->op), stmnt->op.size);
   }

   //
   // Info::trStmnt_BAnd_W
   //
   void Info::trStmnt_BAnd_W()
   {
      CheckArgC(stmnt, 3);

      if(!isPushArg(stmnt->args[1]) || !isPushArg(stmnt->args[2]))
      {
         trStmntStk3(stmnt->op.size, stmnt->op.size, false);

         if(stmnt->op.size > 1)
            func->setLocalTmp(stmnt->op.size * 2 - 1);
      }
      else
         moveArgStk_dst(stmnt->args[0], stmnt->op.size);
   }

   //
   // Info::trStmnt_Bclz_W
   //
   void Info::trStmnt_Bclz_W()
   {
      trStmntStk2(1, stmnt->op.size);
   }
}

// EOF

