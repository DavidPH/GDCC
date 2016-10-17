//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Negate statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "Core/Exception.hpp"

#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      namespace ZDACS
      {
         //
         // Info::genStmnt_NegF_W
         //
         void Info::genStmnt_NegF_W()
         {
            numChunkCODE += 12;
         }

         //
         // Info::genStmnt_NegI_W
         //
         void Info::genStmnt_NegI_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
            {
               numChunkCODE += 4;

               if(stmnt->args[0].a != IR::ArgBase::Stk)
                  numChunkCODE += lenDropArg(stmnt->args[0], 0);

               return;
            }

            if(stmnt->args[0].a == IR::ArgBase::Stk)
            {
               numChunkCODE += (stmnt->op.size - 1) * 24 + 8 +
                  stmnt->op.size * 20;
            }
            else
            {
               numChunkCODE += (stmnt->op.size - 1) * 12 + 4 +
                  lenDropArg(stmnt->args[0], 0, stmnt->op.size) +
                  lenIncUArg(stmnt->args[0], 0, stmnt->op.size) +
                  lenPushArg(stmnt->args[0], 0, stmnt->op.size - 1);
            }
         }

         //
         // Info::putStmnt_NegF_W
         //
         void Info::putStmnt_NegF_W()
         {
            putCode(Code::Push_Lit, 0x80000000);
            putCode(Code::BOrX);
         }

         //
         // Info::putStmnt_NegI_W
         //
         void Info::putStmnt_NegI_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
            {
               putCode(Code::NegI);

               if(stmnt->args[0].a != IR::ArgBase::Stk)
                  putStmntDropArg(stmnt->args[0], 0);

               return;
            }

            if(stmnt->args[0].a == IR::ArgBase::Stk)
            {
               for(auto i = stmnt->op.size; i--;)
               {
                  putCode(Code::BNot);
                  putCode(Code::Drop_LocReg, func->localReg + i);
               }

               putCode(Code::IncU_LocReg, func->localReg + 0);

               for(Core::FastU i = 0, e = stmnt->op.size - 1; i != e; ++i)
               {
                  putCode(Code::Push_LocReg, func->localReg + i);
                  putCode(Code::Jcnd_Tru,    putPos + (e - i) * 24 - 8);
                  putCode(Code::IncU_LocReg, func->localReg + i + 1);
               }

               for(Core::FastU i = 0; i != stmnt->op.size; ++i)
                  putCode(Code::Push_LocReg, func->localReg + i);
            }
            else
            {
               for(auto i = stmnt->op.size; i--;)
               {
                  putCode(Code::BNot);
                  putStmntDropArg(stmnt->args[0], i);
               }

               putStmntIncUArg(stmnt->args[0], 0);

               for(Core::FastU i = 0, e = stmnt->op.size - 1; i != e; ++i)
               {
                  Core::FastU jump = (e - i) * 8 + // Jcnd_Tru
                     lenIncUArg(stmnt->args[0], i + 1, e + 1) +
                     lenPushArg(stmnt->args[0], i + 1, e);

                  putStmntPushArg(stmnt->args[0], i);
                  putCode(Code::Jcnd_Tru, putPos + jump);
                  putStmntIncUArg(stmnt->args[0], i + 1);
               }
            }
         }

         //
         // Info::trStmnt_NegI_W
         //
         void Info::trStmnt_NegI_W()
         {
            CheckArgC(stmnt, 2);

            if(!isCopyArg(stmnt->args[0]) || !isFastArg(stmnt->args[0]))
            {
               if(stmnt->op.size > 1)
                  func->setLocalTmp(stmnt->op.size);

               moveArgStk_dst(stmnt->args[0], stmnt->op.size);
            }

            moveArgStk_src(stmnt->args[1], stmnt->op.size);
         }
      }
   }
}

// EOF

