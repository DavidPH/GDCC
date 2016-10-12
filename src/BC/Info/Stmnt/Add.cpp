//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Addition and subtraction statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "BC/AddFunc.hpp"

#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::addFunc_AddU_W
   //
   void Info::addFunc_AddU_W(Core::FastU n)
   {
      addFunc_AddU_W(n, IR::Code::AddU_W, IR::Code::AdXU_W);
   }

   //
   // Info::addFunc_AddU_W
   //
   void Info::addFunc_AddU_W(Core::FastU n, IR::Code codeAdd, IR::Code codeAdX)
   {
      GDCC_BC_AddFuncPre({codeAdd, n}, n, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n);

      // First words.
      GDCC_BC_AddStmnt({codeAdX, 1}, stk, lop, rop);

      // Mid words.
      for(Core::FastU i = n - 2; i--;)
         GDCC_BC_AddStmnt({codeAdX, 1}, stk, stk, ++lop, ++rop);

      // Last words.
      GDCC_BC_AddStmnt({codeAdd,      1}, stk, ++lop, ++rop);
      GDCC_BC_AddStmnt({Code::AddU_W, 1}, stk,   stk,   stk);

      // Return.
      GDCC_BC_AddStmnt({Code::Retn, n}, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_SubU_W
   //
   void Info::addFunc_SubU_W(Core::FastU n)
   {
      addFunc_AddU_W(n, IR::Code::SubU_W, IR::Code::SuXU_W);
   }
}

// EOF

