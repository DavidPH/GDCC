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
   // Info::putStmnt_AndU_W
   //
   void Info::putStmnt_AndU_W(char const *code)
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
   // Info::putStmnt_OrIU_W
   //
   void Info::putStmnt_OrIU_W()
   {
      putStmnt_AndU_W("OrIU");
   }

   //
   // Info::putStmnt_OrXU_W
   //
   void Info::putStmnt_OrXU_W()
   {
      putStmnt_AndU_W("OrXU");
   }

   //
   // Info::trStmnt_AndU_W
   //
   void Info::trStmnt_AndU_W()
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
   // Info::trStmnt_OrIU_W
   //
   void Info::trStmnt_OrIU_W()
   {
      trStmnt_AndU_W();
   }

   //
   // Info::trStmnt_OrXU_W
   //
   void Info::trStmnt_OrXU_W()
   {
      trStmnt_AndU_W();
   }
}

// EOF

