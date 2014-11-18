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
         // Info::putStmnt_DiXI_W
         //
         void Info::putStmnt_DiXI_W()
         {
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
         // Info::trStmnt_DiXI_W
         //
         void Info::trStmnt_DiXI_W()
         {
            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               moveArgStk_dst(stmnt->args[0], IR::Code::Move_W2);
            }
            else
            {
               trStmntStk3(IR::Code::Move_W2, IR::Code::Move_W, true);
               func->setLocalTmp(2);
            }
         }
      }
   }
}

// EOF

