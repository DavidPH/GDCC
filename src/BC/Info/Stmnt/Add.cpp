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

      IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))};
      IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(n))};

      // First words.
      GDCC_BC_AddStmnt({codeAdX, 1}, IR::Arg_Stk(), lop, rop);

      // Mid words.
      for(Core::FastU i = n - 2; i--;)
         GDCC_BC_AddStmnt({codeAdX, 1}, IR::Arg_Stk(), IR::Arg_Stk(), ++lop, ++rop);

      // Last words.
      GDCC_BC_AddStmnt({codeAdd,          1}, IR::Arg_Stk(), ++lop, ++rop);
      GDCC_BC_AddStmnt({IR::Code::AddU_W, 1}, IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

      // Return.
      GDCC_BC_AddStmnt({IR::Code::Retn, n}, IR::Arg_Stk());

      throw ResetFunc();
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

