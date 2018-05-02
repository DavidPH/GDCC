//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Swap statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::genStmnt_Swap_W
   //
   void Info::genStmnt_Swap_W()
   {
      auto n = getStmntSizeW();

      if(n == 1)
         numChunkCODE += 4;
      else
         numChunkCODE += n * 32;
   }

   //
   // Info::putStmnt_Swap_W
   //
   void Info::putStmnt_Swap_W()
   {
      auto n = getStmntSizeW();

      if(n == 1)
      {
         putCode(Code::Swap);
         return;
      }

      for(Core::FastU i = n; i--;)
         putCode(Code::Drop_LocReg, func->localReg + i);
      for(Core::FastU i = n; i--;)
         putCode(Code::Drop_LocReg, func->localReg + i + n);

      for(Core::FastU i = 0; i != n; ++i)
         putCode(Code::Push_LocReg, func->localReg + i);
      for(Core::FastU i = 0; i != n; ++i)
         putCode(Code::Push_LocReg, func->localReg + i + n);
   }

   //
   // Info::trStmnt_Swap_W
   //
   void Info::trStmnt_Swap_W()
   {
      CheckArgC(stmnt, 2);

      auto n = getStmntSizeW();

      if(n != 1)
         func->setLocalTmp(n * 2);

      CheckArgB(stmnt, 0, IR::ArgBase::Stk);
      CheckArgB(stmnt, 1, IR::ArgBase::Stk);
   }
}

// EOF

