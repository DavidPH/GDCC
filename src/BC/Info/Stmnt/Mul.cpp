//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Multiplication statement handling.
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
   // Info::addFunc_MuXU_W
   //
   void Info::addFunc_MuXU_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre({IR::Code::MuXU_W, n}, n * 2, n * 2, n * 2, __FILE__);

      IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))};
      IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(n))};

      IR::Arg_Stk stk{};

      GDCC_BC_AddStmnt({IR::Code::MuXU_W, 1}, stk, lop, rop);
      GDCC_BC_AddStmnt({IR::Code::Move_W, 1}, stk, 0);

      Core::FastU colEnd = n * 2 - 1;
      for(Core::FastU col = 1; col != colEnd; ++col)
      {
         if(colEnd - col > 1)
            GDCC_BC_AddStmnt({IR::Code::Move_W, 1}, stk, 0);

         Core::FastU sizeAdd = col < colEnd - 1 ? 3 : 2;

         Core::FastU rowEnd = std::min(col + 1, n);
         lop.off = std::min(col, n - 1);
         rop.off = col < n ? 0 : col - n + 1;
         for(; rop.off != rowEnd; --lop, ++rop)
         {
            GDCC_BC_AddStmnt({IR::Code::MuXU_W, 1}, stk, lop, rop);

            if(sizeAdd == 3)
               GDCC_BC_AddStmnt({IR::Code::Move_W, 1}, stk, 0);

            GDCC_BC_AddStmnt({IR::Code::AddU_W, sizeAdd}, stk, stk, stk);
         }
      }

      GDCC_BC_AddStmnt({IR::Code::Retn, n * 2}, stk);

      throw ResetFunc();
   }

   //
   // Info::addFunc_MulU_W
   //
   void Info::addFunc_MulU_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre({IR::Code::MulU_W, n}, n, n * 2, n * 2, __FILE__);

      IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))};
      IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(n))};

      IR::Arg_Stk stk{};

      GDCC_BC_AddStmnt({IR::Code::MuXU_W, 1}, stk, lop, rop);

      if(n > 2)
         GDCC_BC_AddStmnt({IR::Code::Move_W, 1}, stk, 0);

      for(Core::FastU col = 1; col != n; ++col)
      {
         if(n - col > 3)
            GDCC_BC_AddStmnt({IR::Code::Move_W, 1}, stk, 0);

         Core::FastU sizeAdd = std::min<Core::FastU>(3, n - col);
         IR::Code    codeMul = col < n - 1 ? IR::Code::MuXU_W : IR::Code::MulU_W;

         Core::FastU rowEnd = col + 1;
         lop.off = col;
         rop.off = 0;
         for(; rop.off != rowEnd; --lop, ++rop)
         {
            GDCC_BC_AddStmnt({codeMul, 1}, stk, lop, rop);

            if(sizeAdd == 3)
               GDCC_BC_AddStmnt({IR::Code::Move_W, 1}, stk, 0);

            GDCC_BC_AddStmnt({IR::Code::AddU_W, sizeAdd}, stk, stk, stk);
         }
      }

      GDCC_BC_AddStmnt({IR::Code::Retn, n}, stk);

      throw ResetFunc();
   }
}

// EOF

