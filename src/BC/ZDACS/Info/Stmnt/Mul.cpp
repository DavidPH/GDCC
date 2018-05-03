//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Multiplication statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/AddFunc.hpp"

#include "IR/Function.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::preStmnt_MuXU_W
   //
   void Info::preStmnt_MuXU_W()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
      {
         preStmnt_MuXU_W1();
         return;
      }

      addFunc_MuXU_W(n);
   }

   //
   // Info::preStmnt_MuXU_W1
   //
   void Info::preStmnt_MuXU_W1()
   {
      GDCC_BC_AddFuncPre(Code::MuXU_W, 1, 2, 2, 5, __FILE__);
      GDCC_BC_AddFuncObjBin(1, 1);

      GDCC_BC_AddFuncObjReg(llo, 1, 0);
      GDCC_BC_AddFuncObjReg(lhi, 1, 1);
      GDCC_BC_AddFuncObjReg(rlo, 1, 2);
      GDCC_BC_AddFuncObjReg(rhi, 1, 3);
      GDCC_BC_AddFuncObjReg(tmp, 1, 4);

      // Split arguments to 16-bit pieces. Take care due to aliasing.
      GDCC_BC_AddStmnt(IR::Code::BAnd_W, 1, rlo, rop, 0xFFFF);
      GDCC_BC_AddStmnt(IR::Code::ShRU_W, 1, rhi, rop, 16);
      GDCC_BC_AddStmnt(IR::Code::ShRU_W, 1, lhi, lop, 16);
      GDCC_BC_AddStmnt(IR::Code::BAnd_W, 1, llo, lop, 0xFFFF);

      // Column 0.
      GDCC_BC_AddStmnt(IR::Code::MulU_W, 1, stk, llo, rlo);
      GDCC_BC_AddStmnt(IR::Code::Move_W, 1, stk, 0);

      // Column 1.
      GDCC_BC_AddStmnt(IR::Code::MulU_W, 1, tmp, llo, rhi);
      GDCC_BC_AddStmnt(IR::Code::ShLU_W, 1, stk, tmp, 16);
      GDCC_BC_AddStmnt(IR::Code::ShRU_W, 1, stk, tmp, 16);

      GDCC_BC_AddStmnt(IR::Code::MulU_W, 1, tmp, lhi, rlo);
      GDCC_BC_AddStmnt(IR::Code::ShLU_W, 1, stk, tmp, 16);
      GDCC_BC_AddStmnt(IR::Code::ShRU_W, 1, stk, tmp, 16);

      // Column 2.
      GDCC_BC_AddStmnt(IR::Code::MulU_W, 1, stk, lhi, rhi);
      GDCC_BC_AddStmnt(IR::Code::AddU_W, 1, stk, stk, stk);

      GDCC_BC_AddStmnt(IR::Code::AddU_W, 2, stk, stk, stk);
      GDCC_BC_AddStmnt(IR::Code::AddU_W, 2, stk, stk, stk);

      GDCC_BC_AddStmnt(IR::Code::Retn,   2, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info:preStmnt_MulF_W
   //
   void Info::preStmnt_MulF_W()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      addFunc_MulF_W(n);
   }

   //
   // Info::preStmnt_MulK_W
   //
   void Info::preStmnt_MulK_W()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      addFunc_MulK_W(n);
   }

   //
   // Info::preStmnt_MulU_W
   //
   void Info::preStmnt_MulU_W()
   {
      auto n = getStmntSizeW();

      if(n <= 1)
         return;

      addFunc_MulU_W(n);
   }

   //
   // Info::preStmnt_MulX_W
   //
   void Info::preStmnt_MulX_W()
   {
      auto n = getStmntSizeW();

      if(n <= 1)
         return;

      addFunc_MulX_W(n);
   }
}

// EOF

