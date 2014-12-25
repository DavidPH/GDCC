//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Division statement handling.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Bytecode/ZDACS/Code.hpp"

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
         // Info::genStmnt_DiXI_W
         //
         void Info::genStmnt_DiXI_W()
         {
            if(stmnt->op.size != 1)
            {
               genStmntCall(stmnt->op.size * 2);
               return;
            }

            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               numChunkCODE += 56;
            }
            else
            {
               numChunkCODE += 8
                  + lenPushArg(stmnt->args[1], 0, 1) * 2
                  + lenPushArg(stmnt->args[2], 0, 1) * 2;
            }
         }

         //
         // Info::genStmnt_DivI_W
         //
         void Info::genStmnt_DivI_W()
         {
            if(stmnt->op.size == 1)
               numChunkCODE += 4;
            else
               genStmntCall(stmnt->op.size);
         }

         //
         // Info::putStmnt_DiXI_W
         //
         void Info::putStmnt_DiXI_W()
         {
            if(stmnt->op.size != 1)
            {
               putStmntCall(stmnt->op.size * 2);
               return;
            }

            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               putCode(Code::Drop_LocReg, func->localReg + 1);
               putCode(Code::Drop_LocReg, func->localReg + 0);

               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::Push_LocReg, func->localReg + 1);
               putCode(Code::DivI);

               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::Push_LocReg, func->localReg + 1);
               putCode(Code::ModI);
            }
            else
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(Code::DivI);

               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(Code::ModI);
            }
         }

         //
         // Info::putStmnt_DivI_W
         //
         void Info::putStmnt_DivI_W()
         {
            if(stmnt->op.size == 1)
               putCode(Code::DivI);
            else
               putStmntCall(stmnt->op.size);
         }

         //
         // Info::putStmnt_DivX_W
         //
         void Info::putStmnt_DivX_W()
         {
            if(stmnt->op.size == 1)
               putCode(Code::DivX);
            else
               putStmntCall(stmnt->op.size);
         }

         //
         // Info::putStmnt_ModI_W
         //
         void Info::putStmnt_ModI_W()
         {
            if(stmnt->op.size == 1)
               putCode(Code::ModI);
            else
               putStmntCall(stmnt->op.size);
         }

         //
         // Info::trStmnt_DiXI_W
         //
         void Info::trStmnt_DiXI_W()
         {
            if(stmnt->op.size != 1)
            {
               trStmntStk3(stmnt->op.size * 2, stmnt->op.size, true);
               return;
            }

            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               moveArgStk_dst(stmnt->args[0], stmnt->op.size * 2);
            }
            else
            {
               trStmntStk3(stmnt->op.size * 2, stmnt->op.size, true);
               func->setLocalTmp(stmnt->op.size * 2);
            }
         }

         //
         // Info::trStmnt_DiXU_W
         //
         void Info::trStmnt_DiXU_W()
         {
            trStmntStk3(stmnt->op.size * 2, stmnt->op.size, true);
         }
      }
   }
}

// EOF

