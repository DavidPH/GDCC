//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Bitwise statement handling.
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
         // Info::genStmntBitwise2
         //
         void Info::genStmntBitwise2()
         {
            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               numChunkCODE += 56;
            }
            else
            {
               genStmntPushArg(stmnt->args[1], 0, 2);
               genStmntPushArg(stmnt->args[2], 0, 2);
               numChunkCODE += 8;
            }
         }

         //
         // Info::genStmntBitwise3
         //
         void Info::genStmntBitwise3()
         {
            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               numChunkCODE += 92;
            }
            else
            {
               genStmntPushArg(stmnt->args[1], 0, 3);
               genStmntPushArg(stmnt->args[2], 0, 3);
               numChunkCODE += 12;
            }
         }

         //
         // Info::putStmntBitwise2
         //
         void Info::putStmntBitwise2(Code code)
         {
            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 0);
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 1);
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 2);

               putCode(Code::Push_LocReg);
               putWord(func->localReg + 1);
               putCode(code);

               putCode(Code::Push_LocReg);
               putWord(func->localReg + 2);
               putCode(Code::Push_LocReg);
               putWord(func->localReg + 0);
               putCode(code);
            }
            else
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(code);
               putStmntPushArg(stmnt->args[1], 1);
               putStmntPushArg(stmnt->args[2], 1);
               putCode(code);
            }
         }

         //
         // Info::putStmntBitwise3
         //
         void Info::putStmntBitwise3(Code code)
         {
            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 0);
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 1);
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 2);
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 3);
               putCode(Code::Drop_LocReg);
               putWord(func->localReg + 4);

               putCode(Code::Push_LocReg);
               putWord(func->localReg + 2);
               putCode(code);

               putCode(Code::Push_LocReg);
               putWord(func->localReg + 4);
               putCode(Code::Push_LocReg);
               putWord(func->localReg + 1);
               putCode(code);

               putCode(Code::Push_LocReg);
               putWord(func->localReg + 3);
               putCode(Code::Push_LocReg);
               putWord(func->localReg + 0);
               putCode(code);
            }
            else
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(code);
               putStmntPushArg(stmnt->args[1], 1);
               putStmntPushArg(stmnt->args[2], 1);
               putCode(code);
               putStmntPushArg(stmnt->args[1], 2);
               putStmntPushArg(stmnt->args[2], 2);
               putCode(code);
            }
         }

         //
         // Info::trStmntBitwise2
         //
         void Info::trStmntBitwise2()
         {
            CheckArgC(stmnt, 3);

            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               moveArgStk_dst(stmnt->args[0], IR::Code::Move_W2);
            }
            else
            {
               trStmntStk3(IR::Code::Move_W2, false);
               func->setLocalTmp(3);
            }
         }

         //
         // Info::trStmntBitwise3
         //
         void Info::trStmntBitwise3()
         {
            CheckArgC(stmnt, 3);

            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               moveArgStk_dst(stmnt->args[0], IR::Code::Move_W3);
            }
            else
            {
               trStmntStk3(IR::Code::Move_W3, false);
               func->setLocalTmp(5);
            }
         }
      }
   }
}

// EOF

