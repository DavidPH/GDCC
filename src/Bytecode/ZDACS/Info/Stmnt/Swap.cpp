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
         // Info::putStmnt_Swap_Wx
         //
         void Info::putStmnt_Swap_Wx(Core::FastU x)
         {
            for(Core::FastU i = x; i--;)
               putCode(Code::Drop_LocReg, func->localReg + i);
            for(Core::FastU i = x; i--;)
               putCode(Code::Drop_LocReg, func->localReg + i + x);

            for(Core::FastU i = 0; i != x; ++i)
               putCode(Code::Push_LocReg, func->localReg + i);
            for(Core::FastU i = 0; i != x; ++i)
               putCode(Code::Push_LocReg, func->localReg + i + x);
         }

         //
         // Info::trStmnt_Swap_Wx
         //
         void Info::trStmnt_Swap_Wx(Core::FastU x)
         {
            CheckArgC(stmnt, 2);

            if(x > 1) func->setLocalTmp(x * 2);

            CheckArgB(stmnt, 0, IR::ArgBase::Stk);
            CheckArgB(stmnt, 1, IR::ArgBase::Stk);
         }
      }
   }
}

// EOF

