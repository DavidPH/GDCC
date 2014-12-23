//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Invert statement handling.
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
         // Info::genStmnt_InvU_W2
         //
         void Info::genStmnt_InvU_W2()
         {
            if(isPushArg(stmnt->args[1]))
            {
               genStmntPushArg(stmnt->args[1], 0, 2);
               numChunkCODE += 8;
            }
            else if(isDropArg(stmnt->args[0]))
            {
               genStmntDropArg(stmnt->args[0], 0, 2);
               numChunkCODE += 8;
            }
            else
               numChunkCODE += 16;
         }

         //
         // Info::genStmnt_InvU_W3
         //
         void Info::genStmnt_InvU_W3()
         {
            if(isPushArg(stmnt->args[1]))
            {
               genStmntPushArg(stmnt->args[1], 0, 3);
               numChunkCODE += 12;
            }
            else if(isDropArg(stmnt->args[0]))
            {
               genStmntDropArg(stmnt->args[0], 0, 3);
               numChunkCODE += 12;
            }
            else
               numChunkCODE += 36;
         }

         //
         // Info::putStmnt_InvU_W2
         //
         void Info::putStmnt_InvU_W2()
         {
            if(isPushArg(stmnt->args[1]))
            {
               putStmntPushArg(stmnt->args[1], 0);
               putCode(Code::InvU);
               putStmntPushArg(stmnt->args[1], 1);
               putCode(Code::InvU);
            }
            else if(isDropArg(stmnt->args[0]))
            {
               putCode(Code::InvU);
               putStmntDropArg(stmnt->args[0], 1);
               putCode(Code::InvU);
               putStmntDropArg(stmnt->args[0], 0);
            }
            else
            {
               putCode(Code::InvU);
               putCode(Code::Swap);
               putCode(Code::InvU);
               putCode(Code::Swap);
            }
         }

         //
         // Info::putStmnt_InvU_W3
         //
         void Info::putStmnt_InvU_W3()
         {
            if(isPushArg(stmnt->args[1]))
            {
               putStmntPushArg(stmnt->args[1], 0);
               putCode(Code::InvU);
               putStmntPushArg(stmnt->args[1], 1);
               putCode(Code::InvU);
               putStmntPushArg(stmnt->args[1], 2);
               putCode(Code::InvU);
            }
            else if(isDropArg(stmnt->args[0]))
            {
               putCode(Code::InvU);
               putStmntDropArg(stmnt->args[0], 2);
               putCode(Code::InvU);
               putStmntDropArg(stmnt->args[0], 1);
               putCode(Code::InvU);
               putStmntDropArg(stmnt->args[0], 0);
            }
            else
            {
               putCode(Code::InvU);
               putCode(Code::Drop_LocReg, func->localReg + 0);
               putCode(Code::InvU);
               putCode(Code::Swap);
               putCode(Code::InvU);
               putCode(Code::Swap);
               putCode(Code::Push_LocReg, func->localReg + 0);
            }
         }

         //
         // Info::trStmnt_InvU_W2
         //
         void Info::trStmnt_InvU_W2()
         {
            CheckArgC(stmnt, 2);

            if(isPushArg(stmnt->args[1]))
               moveArgStk_dst(stmnt->args[0], stmnt->op.size);
            else if(isDropArg(stmnt->args[0]))
               moveArgStk_src(stmnt->args[1], stmnt->op.size);
            else
            {
               moveArgStk_dst(stmnt->args[0], stmnt->op.size);
               moveArgStk_src(stmnt->args[1], stmnt->op.size);
            }
         }

         //
         // Info::trStmnt_InvU_W3
         //
         void Info::trStmnt_InvU_W3()
         {
            CheckArgC(stmnt, 2);

            if(isPushArg(stmnt->args[1]))
               moveArgStk_dst(stmnt->args[0], stmnt->op.size);
            else if(isDropArg(stmnt->args[0]))
               moveArgStk_src(stmnt->args[1], stmnt->op.size);
            else
            {
               func->setLocalTmp(stmnt->op.size - 2);
               moveArgStk_dst(stmnt->args[0], stmnt->op.size);
               moveArgStk_src(stmnt->args[1], stmnt->op.size);
            }
         }
      }
   }
}

// EOF

