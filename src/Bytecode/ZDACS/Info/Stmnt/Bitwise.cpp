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
         // Info::genStmntBitwise
         //
         void Info::genStmntBitwise()
         {
            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               if(stmnt->op.size > 1)
                  numChunkCODE += (stmnt->op.size * 4 - 2) * 8;
               numChunkCODE += stmnt->op.size * 4;
            }
            else
            {
               numChunkCODE += lenPushArg(stmnt->args[1], 0, stmnt->op.size);
               numChunkCODE += lenPushArg(stmnt->args[2], 0, stmnt->op.size);
               numChunkCODE += stmnt->op.size * 4;
            }
         }

         //
         // Info::putStmntBitwise
         //
         void Info::putStmntBitwise(Code code)
         {
            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               if(stmnt->op.size > 1)
               {
                  for(Core::FastU i = 0, e = stmnt->op.size * 2 - 1; i != e; ++i)
                     putCode(Code::Drop_LocReg, func->localReg + i);

                  putCode(Code::Push_LocReg, func->localReg + stmnt->op.size - 1);
               }

               putCode(code);

               if(stmnt->op.size > 1) for(Core::FastU i = stmnt->op.size - 1; i--;)
               {
                  putCode(Code::Push_LocReg, func->localReg + stmnt->op.size + i);
                  putCode(Code::Push_LocReg, func->localReg + i);
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
         // Info::trStmntBitwise
         //
         void Info::trStmntBitwise()
         {
            CheckArgC(stmnt, 3);

            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               moveArgStk_dst(stmnt->args[0], stmnt->op.size);
            }
            else
            {
               trStmntStk3(stmnt->op.size, stmnt->op.size, false);

               if(stmnt->op.size > 1)
                  func->setLocalTmp(stmnt->op.size * 2 - 1);
            }
         }
      }
   }
}

// EOF

