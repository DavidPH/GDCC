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
         // Info::genStmnt_InvU_W
         //
         void Info::genStmnt_InvU_W()
         {
            if(isPushArg(stmnt->args[1]))
            {
               numChunkCODE += lenPushArg(stmnt->args[1], 0, stmnt->op.size);
               numChunkCODE += stmnt->op.size * 4;
            }
            else if(isDropArg(stmnt->args[0]))
            {
               numChunkCODE += lenDropArg(stmnt->args[0], 0, stmnt->op.size);
               numChunkCODE += stmnt->op.size * 4;
            }
            else
            {
               numChunkCODE += stmnt->op.size * 4;

               if(stmnt->op.size > 1)
                  numChunkCODE += 8;

               if(stmnt->op.size > 2)
                  numChunkCODE += (stmnt->op.size - 2) * 16;
            }
         }

         //
         // Info::putStmnt_InvU_W
         //
         void Info::putStmnt_InvU_W()
         {
            if(isPushArg(stmnt->args[1]))
            {
               for(Core::FastU i = 0; i != stmnt->op.size; ++i)
               {
                  putStmntPushArg(stmnt->args[1], i);
                  putCode(Code::InvU);
               }
            }
            else if(isDropArg(stmnt->args[0]))
            {
               if(auto i = stmnt->op.size) while(i--)
               {
                  putCode(Code::InvU);
                  putStmntDropArg(stmnt->args[0], i);
               }
            }
            else
            {
               if(stmnt->op.size > 2)
               {
                  for(Core::FastU i = stmnt->op.size - 2; i--;)
                  {
                     putCode(Code::InvU);
                     putCode(Code::Drop_LocReg, func->localReg + i);
                  }
               }

               if(stmnt->op.size > 0)
                  putCode(Code::InvU);

               if(stmnt->op.size > 1)
               {
                  putCode(Code::Swap);
                  putCode(Code::InvU);
                  putCode(Code::Swap);
               }

               if(stmnt->op.size > 2)
               {
                  for(Core::FastU i = 0, e = stmnt->op.size - 2; i != e; ++i)
                     putCode(Code::Push_LocReg, func->localReg + i);
               }
            }
         }

         //
         // Info::trStmnt_InvU_W
         //
         void Info::trStmnt_InvU_W()
         {
            CheckArgC(stmnt, 2);

            if(isPushArg(stmnt->args[1]))
               moveArgStk_dst(stmnt->args[0], stmnt->op.size);
            else if(isDropArg(stmnt->args[0]))
               moveArgStk_src(stmnt->args[1], stmnt->op.size);
            else
            {
               if(stmnt->op.size > 2)
                  func->setLocalTmp(stmnt->op.size - 2);

               moveArgStk_dst(stmnt->args[0], stmnt->op.size);
               moveArgStk_src(stmnt->args[1], stmnt->op.size);
            }
         }
      }
   }
}

// EOF

