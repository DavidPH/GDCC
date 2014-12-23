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
         // Info::genStmnt_CmpU_EQ_W2
         //
         void Info::genStmnt_CmpU_EQ_W2()
         {
            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               genStmntPushArg(stmnt->args[1], 0, 2);
               genStmntPushArg(stmnt->args[2], 0, 2);
               numChunkCODE += 12;
            }
            else
               genStmntCall(1);
         }

         //
         // Info::genStmnt_CmpU_EQ_W3
         //
         void Info::genStmnt_CmpU_EQ_W3()
         {
            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               genStmntPushArg(stmnt->args[1], 0, 3);
               genStmntPushArg(stmnt->args[2], 0, 3);
               numChunkCODE += 20;
            }
            else
               genStmntCall(1);
         }

         //
         // Info::putStmnt_CmpU_EQ_W2
         //
         void Info::putStmnt_CmpU_EQ_W2()
         {
            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(Code::CmpU_EQ);
               putStmntPushArg(stmnt->args[1], 1);
               putStmntPushArg(stmnt->args[2], 1);
               putCode(Code::CmpU_EQ);
               putCode(Code::AndU);
            }
            else
               putStmntCall("___GDCC__CmpU_EQ_W2", 1);
         }

         //
         // Info::putStmnt_CmpU_EQ_W3
         //
         void Info::putStmnt_CmpU_EQ_W3()
         {
            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(Code::CmpU_EQ);
               putStmntPushArg(stmnt->args[1], 1);
               putStmntPushArg(stmnt->args[2], 1);
               putCode(Code::CmpU_EQ);
               putCode(Code::AndU);
               putStmntPushArg(stmnt->args[1], 2);
               putStmntPushArg(stmnt->args[2], 2);
               putCode(Code::CmpU_EQ);
               putCode(Code::AndU);
            }
            else
               putStmntCall("___GDCC__CmpU_EQ_W3", 1);
         }

         //
         // Info::putStmnt_CmpU_NE_W2
         //
         void Info::putStmnt_CmpU_NE_W2()
         {
            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(Code::CmpU_NE);
               putStmntPushArg(stmnt->args[1], 1);
               putStmntPushArg(stmnt->args[2], 1);
               putCode(Code::CmpU_NE);
               putCode(Code::OrIU);
            }
            else
               putStmntCall("___GDCC__CmpU_NE_W2", 1);
         }

         //
         // Info::putStmnt_CmpU_NE_W3
         //
         void Info::putStmnt_CmpU_NE_W3()
         {
            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               putStmntPushArg(stmnt->args[1], 0);
               putStmntPushArg(stmnt->args[2], 0);
               putCode(Code::CmpU_NE);
               putStmntPushArg(stmnt->args[1], 1);
               putStmntPushArg(stmnt->args[2], 1);
               putCode(Code::CmpU_NE);
               putCode(Code::OrIU);
               putStmntPushArg(stmnt->args[1], 2);
               putStmntPushArg(stmnt->args[2], 2);
               putCode(Code::CmpU_NE);
               putCode(Code::OrIU);
            }
            else
               putStmntCall("___GDCC__CmpU_NE_W3", 1);
         }

         //
         // Info::trStmnt_CmpU_EQ
         //
         void Info::trStmnt_CmpU_EQ()
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

