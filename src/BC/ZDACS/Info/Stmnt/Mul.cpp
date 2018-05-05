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
   // Info::preStmnt_MuXU
   //
   void Info::preStmnt_MuXU()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
      {
         preStmnt_MuXU1();
         return;
      }

      addFunc_MuXU_W(n);
   }

   //
   // Info::preStmnt_MuXU1
   //
   void Info::preStmnt_MuXU1()
   {
      GDCC_BC_AddFuncPre(Code::MuXU, 1, 2, 2, 5, __FILE__);
      GDCC_BC_AddFuncObjBin(1, 1);

      GDCC_BC_AddFuncObjReg(llo, 1, 0);
      GDCC_BC_AddFuncObjReg(lhi, 1, 1);
      GDCC_BC_AddFuncObjReg(rlo, 1, 2);
      GDCC_BC_AddFuncObjReg(rhi, 1, 3);
      GDCC_BC_AddFuncObjReg(tmp, 1, 4);

      // Split arguments to 16-bit pieces. Take care due to aliasing.
      GDCC_BC_AddStmnt(IR::Code::BAnd, 1, rlo, rop, 0xFFFF);
      GDCC_BC_AddStmnt(IR::Code::ShRU, 1, rhi, rop, 16);
      GDCC_BC_AddStmnt(IR::Code::ShRU, 1, lhi, lop, 16);
      GDCC_BC_AddStmnt(IR::Code::BAnd, 1, llo, lop, 0xFFFF);

      // Column 0.
      GDCC_BC_AddStmnt(IR::Code::MulU, 1, stk, llo, rlo);
      GDCC_BC_AddStmnt(IR::Code::Move, 1, stk, 0);

      // Column 1.
      GDCC_BC_AddStmnt(IR::Code::MulU, 1, tmp, llo, rhi);
      GDCC_BC_AddStmnt(IR::Code::ShLU, 1, stk, tmp, 16);
      GDCC_BC_AddStmnt(IR::Code::ShRU, 1, stk, tmp, 16);

      GDCC_BC_AddStmnt(IR::Code::MulU, 1, tmp, lhi, rlo);
      GDCC_BC_AddStmnt(IR::Code::ShLU, 1, stk, tmp, 16);
      GDCC_BC_AddStmnt(IR::Code::ShRU, 1, stk, tmp, 16);

      // Column 2.
      GDCC_BC_AddStmnt(IR::Code::MulU, 1, stk, lhi, rhi);
      GDCC_BC_AddStmnt(IR::Code::AddU, 1, stk, stk, stk);

      GDCC_BC_AddStmnt(IR::Code::AddU, 2, stk, stk, stk);
      GDCC_BC_AddStmnt(IR::Code::AddU, 2, stk, stk, stk);

      GDCC_BC_AddStmnt(IR::Code::Retn, 2, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info:preStmnt_MulF
   //
   void Info::preStmnt_MulF()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      addFunc_MulF_W(n);
   }

   //
   // Info::preStmnt_MulK
   //
   void Info::preStmnt_MulK()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      addFunc_MulK_W(n);
   }

   //
   // Info::preStmnt_MulU
   //
   void Info::preStmnt_MulU()
   {
      auto n = getStmntSize();

      if(n <= 1)
         return;

      addFunc_MulU_W(n);
   }

   //
   // Info::preStmnt_MulX
   //
   void Info::preStmnt_MulX()
   {
      auto n = getStmntSize();

      if(n <= 1)
         return;

      addFunc_MulX_W(n);
   }
}

// EOF

