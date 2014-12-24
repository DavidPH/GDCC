//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Swap statement handling.
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
         // Info::genStmnt_Swap_W
         //
         void Info::genStmnt_Swap_W()
         {
            if(stmnt->op.size == 1)
               numChunkCODE += 4;
            else
               numChunkCODE += stmnt->op.size * 32;
         }

         //
         // Info::putStmnt_Swap_W
         //
         void Info::putStmnt_Swap_W()
         {
            if(stmnt->op.size == 1)
            {
               putCode(Code::Swap);
               return;
            }

            for(Core::FastU i = stmnt->op.size; i--;)
               putCode(Code::Drop_LocReg, func->localReg + i);
            for(Core::FastU i = stmnt->op.size; i--;)
               putCode(Code::Drop_LocReg, func->localReg + i + stmnt->op.size);

            for(Core::FastU i = 0; i != stmnt->op.size; ++i)
               putCode(Code::Push_LocReg, func->localReg + i);
            for(Core::FastU i = 0; i != stmnt->op.size; ++i)
               putCode(Code::Push_LocReg, func->localReg + i + stmnt->op.size);
         }

         //
         // Info::trStmnt_Swap_W
         //
         void Info::trStmnt_Swap_W()
         {
            CheckArgC(stmnt, 2);

            if(stmnt->op.size != 1)
               func->setLocalTmp(stmnt->op.size * 2);

            CheckArgB(stmnt, 0, IR::ArgBase::Stk);
            CheckArgB(stmnt, 1, IR::ArgBase::Stk);
         }
      }
   }
}

// EOF

