//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
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
   GDCC_BC_CodeTypeSwitchFn(gen, Mul,)
   GDCC_BC_CodeTypeSwitchFn(gen, MulX, Ux)

   GDCC_BC_CodeTypeSwitchFn(pre, Mul,)
   GDCC_BC_CodeTypeSwitchFn(pre, MulX, Ux)

   //
   // Info::addFunc_MulX_UW1
   //
   void Info::addFunc_MulX_UW1()
   {
      GDCC_BC_AddFuncPre((Code::MulX+U, 1), 2, 2, 5, __FILE__);
      GDCC_BC_AddFuncObjBin(1, 1);

      GDCC_BC_AddFuncObjReg(llo, 1, 0);
      GDCC_BC_AddFuncObjReg(lhi, 1, 1);
      GDCC_BC_AddFuncObjReg(rlo, 1, 2);
      GDCC_BC_AddFuncObjReg(rhi, 1, 3);
      GDCC_BC_AddFuncObjReg(tmp, 1, 4);

      // Split arguments to 16-bit pieces. Take care due to aliasing.
      GDCC_BC_AddStmnt(Code::BAnd,  1, rlo, rop, 0xFFFF);
      GDCC_BC_AddStmnt(Code::ShR+U, 1, rhi, rop, 16);
      GDCC_BC_AddStmnt(Code::ShR+U, 1, lhi, lop, 16);
      GDCC_BC_AddStmnt(Code::BAnd,  1, llo, lop, 0xFFFF);

      // Column 0.
      GDCC_BC_AddStmnt(Code::Mul+U, 1, stk, llo, rlo);
      GDCC_BC_AddStmnt(Code::Move,  1, stk, 0);

      // Column 1.
      GDCC_BC_AddStmnt(Code::Mul+U, 1, tmp, llo, rhi);
      GDCC_BC_AddStmnt(Code::ShL+U, 1, stk, tmp, 16);
      GDCC_BC_AddStmnt(Code::ShR+U, 1, stk, tmp, 16);

      GDCC_BC_AddStmnt(Code::Mul+U, 1, tmp, lhi, rlo);
      GDCC_BC_AddStmnt(Code::ShL+U, 1, stk, tmp, 16);
      GDCC_BC_AddStmnt(Code::ShR+U, 1, stk, tmp, 16);

      // Column 2.
      GDCC_BC_AddStmnt(Code::Mul+U, 1, stk, lhi, rhi);
      GDCC_BC_AddStmnt(Code::Add+U, 1, stk, stk, stk);

      GDCC_BC_AddStmnt(Code::Add+U, 2, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::Add+U, 2, stk, stk, stk);

      GDCC_BC_AddStmnt(Code::Retn,  2, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::preStmnt_MulX_U
   //
   void Info::preStmnt_MulX_U()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
         return addFunc_MulX_UW1();

      addFunc_MulX_UW(n);
   }
}

// EOF

