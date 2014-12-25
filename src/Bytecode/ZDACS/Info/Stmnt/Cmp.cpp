//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Compare statement handling.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Bytecode/ZDACS/Code.hpp"

#include "Core/Exception.hpp"

#include "IR/Statement.hpp"


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
         // Info::genStmnt_CmpI_W
         //
         void Info::genStmnt_CmpI_W()
         {
            if(stmnt->op.size == 1)
               numChunkCODE += 4;
            else
               genStmntCall(1);
         }

         //
         // Info::genStmnt_CmpU_EQ_W
         //
         void Info::genStmnt_CmpU_EQ_W()
         {
            if(!stmnt->op.size) return;

            if(stmnt->args[1].a != IR::ArgBase::Stk ||
               stmnt->args[1].a != IR::ArgBase::Stk)
            {
               numChunkCODE += stmnt->op.size * 8 - 4 +
                  lenPushArg(stmnt->args[1], 0, stmnt->op.size) +
                  lenPushArg(stmnt->args[2], 0, stmnt->op.size);
            }
            else if(stmnt->op.size == 1)
               numChunkCODE += 4;
            else
               genStmntCall(1);
         }

         //
         // Info::putStmnt_CmpI_W
         //
         void Info::putStmnt_CmpI_W(Code code)
         {
            if(stmnt->op.size == 1)
               putCode(code);
            else
               putStmntCall(1);
         }

         //
         // Info::putStmnt_CmpU_EQ_W
         //
         void Info::putStmnt_CmpU_EQ_W()
         {
            if(!stmnt->op.size) return;

            if(stmnt->args[1].a != IR::ArgBase::Stk ||
               stmnt->args[1].a != IR::ArgBase::Stk)
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(Code::CmpU_EQ);

               for(Core::FastU i = 1; i != stmnt->op.size; ++i)
               {
                  putStmntPushArg(stmnt->args[1], i);
                  putStmntPushArg(stmnt->args[2], i);
                  putCode(Code::CmpU_EQ);
                  putCode(Code::AndU);
               }
            }
            else if(stmnt->op.size == 1)
               putCode(Code::CmpU_EQ);
            else
               putStmntCall(1);
         }

         //
         // Info::putStmnt_CmpU_NE_W
         //
         void Info::putStmnt_CmpU_NE_W()
         {
            if(!stmnt->op.size) return;

            if(stmnt->args[1].a != IR::ArgBase::Stk ||
               stmnt->args[1].a != IR::ArgBase::Stk)
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(Code::CmpU_NE);

               for(Core::FastU i = 1; i != stmnt->op.size; ++i)
               {
                  putStmntPushArg(stmnt->args[1], i);
                  putStmntPushArg(stmnt->args[2], i);
                  putCode(Code::CmpU_NE);
                  putCode(Code::OrIU);
               }
            }
            else if(stmnt->op.size == 1)
               putCode(Code::CmpU_NE);
            else
               putStmntCall(1);
         }

         //
         // Info::trStmnt_CmpU_EQ_W
         //
         void Info::trStmnt_CmpU_EQ_W()
         {
            CheckArgC(stmnt, 3);

            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
               moveArgStk_dst(stmnt->args[0], 1);
            else
               trStmntStk3(1, stmnt->op.size, false);
         }
      }
   }
}

// EOF

