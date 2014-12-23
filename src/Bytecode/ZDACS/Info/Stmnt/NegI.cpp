//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Negate statement handling.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Bytecode/ZDACS/Code.hpp"

#include "Core/Exception.hpp"

#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
      {
         //
         // Info::genStmnt_NegF_Wx
         //
         void Info::genStmnt_NegF_Wx()
         {
            numChunkCODE += 12;
         }

         //
         // Info::genStmnt_NegI_W2
         //
         void Info::genStmnt_NegI_W2()
         {
            if(isCopyArg(stmnt->args[0]) && isFastArg(stmnt->args[0]))
            {
               numChunkCODE += 16 +
                  lenDropArg(stmnt->args[0], 0, 2) +
                  lenIncUArg(stmnt->args[0], 0, 2) +
                  lenPushArg(stmnt->args[0], 0);
            }
            else
               numChunkCODE += 72;
         }

         //
         // Info::genStmnt_NegI_W3
         //
         void Info::genStmnt_NegI_W3()
         {
            if(isCopyArg(stmnt->args[0]) && isFastArg(stmnt->args[0]))
            {
               numChunkCODE += 28 +
                  lenDropArg(stmnt->args[0], 0, 3) +
                  lenIncUArg(stmnt->args[0], 0, 3) +
                  lenPushArg(stmnt->args[0], 0, 2);
            }
            else
               numChunkCODE += 116;
         }

         //
         // Info::putStmnt_NegF_Wx
         //
         void Info::putStmnt_NegF_Wx()
         {
            putCode(Code::Push_Lit, 0x80000000);
            putCode(Code::OrXU);
         }

         //
         // Info::putStmnt_NegI_W2
         //
         void Info::putStmnt_NegI_W2()
         {
            if(isCopyArg(stmnt->args[0]) && isFastArg(stmnt->args[0]))
            {
               Core::FastU jump0 = 8 + lenIncUArg(stmnt->args[0], 1);

               putCode(Code::InvU);
               putStmntDropArg(stmnt->args[0], 1);
               putCode(Code::InvU);
               putStmntDropArg(stmnt->args[0], 0);

               putStmntIncUArg(stmnt->args[0], 0);
               putStmntPushArg(stmnt->args[0], 0);
               putCode(Code::Jcnd_Tru, putPos + jump0);
               putStmntIncUArg(stmnt->args[0], 1);
            }
            else
            {
               putCode(Code::InvU);
               putCode(Code::Drop_LocReg, func->localReg + 1);
               putCode(Code::InvU);
               putCode(Code::Drop_LocReg, func->localReg + 0);

               putCode(Code::IncU_LocReg, func->localReg + 0);
               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::Jcnd_Tru,    putPos + 16);
               putCode(Code::IncU_LocReg, func->localReg + 1);

               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::Push_LocReg, func->localReg + 1);
            }
         }

         //
         // Info::putStmnt_NegI_W3
         //
         void Info::putStmnt_NegI_W3()
         {
            if(isCopyArg(stmnt->args[0]) && isFastArg(stmnt->args[0]))
            {
               Core::FastU jump1 = 8 + lenIncUArg(stmnt->args[0], 2);
               Core::FastU jump0 = jump1 + 8 +
                  lenIncUArg(stmnt->args[0], 1) +
                  lenPushArg(stmnt->args[0], 1);

               putCode(Code::InvU);
               putStmntDropArg(stmnt->args[0], 2);
               putCode(Code::InvU);
               putStmntDropArg(stmnt->args[0], 1);
               putCode(Code::InvU);
               putStmntDropArg(stmnt->args[0], 0);

               putStmntIncUArg(stmnt->args[0], 0);
               putStmntPushArg(stmnt->args[0], 0);
               putCode(Code::Jcnd_Tru, putPos + jump0);
               putStmntIncUArg(stmnt->args[0], 1);
               putStmntPushArg(stmnt->args[0], 1);
               putCode(Code::Jcnd_Tru, putPos + jump1);
               putStmntIncUArg(stmnt->args[0], 2);
            }
            else
            {
               putCode(Code::InvU);
               putCode(Code::Drop_LocReg, func->localReg + 2);
               putCode(Code::InvU);
               putCode(Code::Drop_LocReg, func->localReg + 1);
               putCode(Code::InvU);
               putCode(Code::Drop_LocReg, func->localReg + 0);

               putCode(Code::IncU_LocReg, func->localReg + 0);
               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::Jcnd_Tru,    putPos + 40);
               putCode(Code::IncU_LocReg, func->localReg + 1);
               putCode(Code::Push_LocReg, func->localReg + 1);
               putCode(Code::Jcnd_Tru,    putPos + 16);
               putCode(Code::IncU_LocReg, func->localReg + 2);

               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::Push_LocReg, func->localReg + 1);
               putCode(Code::Push_LocReg, func->localReg + 2);
            }
         }

         //
         // Info::trStmnt_NegI_W2
         //
         void Info::trStmnt_NegI_W2()
         {
            CheckArgC(stmnt, 2);

            if(!isCopyArg(stmnt->args[0]) || !isFastArg(stmnt->args[0]))
            {
               func->setLocalTmp(stmnt->op.size);
               moveArgStk_dst(stmnt->args[0], stmnt->op.size);
            }

            moveArgStk_src(stmnt->args[1], stmnt->op.size);
         }

         //
         // Info::trStmnt_NegI_W3
         //
         void Info::trStmnt_NegI_W3()
         {
            CheckArgC(stmnt, 2);

            if(!isCopyArg(stmnt->args[0]) || !isFastArg(stmnt->args[0]))
            {
               func->setLocalTmp(stmnt->op.size);
               moveArgStk_dst(stmnt->args[0], stmnt->op.size);
            }

            moveArgStk_src(stmnt->args[1], stmnt->op.size);
         }
      }
   }
}

// EOF

