//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2025 David Hill
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

      IR::Glyph labelCheckRop{prog, name + "$checkRop"};
      IR::Glyph labelShiftLow{prog, name + "$shiftLow"};

      // Calculate low word using normal multiply.
      GDCC_BC_AddStmnt(Code::Mul+U, 1, stk, lop, rop);

      // Calculate bottom 16 bits of high word using a fixed-mul.
      // Fixed-mul essentially gives up bits 16-47 of an expanding multiply, so
      // taking the top 16 bits would give us the 16 bits we want.
      // However, because it is a signed operation, if the high bit is set in
      // an operand, it gets sign-extended, ruining the result.
      // This can be overcome by simply discarding the low bit, leaving the
      // high bit always unset.
      // However, if the discarded bit was set, this sacrifices one bit of the
      // output we need for each operand.
      // This can be overcome by adding in the high bits of the opposite
      // operand into the fixed-mul result, so long as it is rounded half-up.
      GDCC_BC_AddStmnt(Code::ShR+U, 1, stk, lop, 1);
      GDCC_BC_AddStmnt(Code::ShR+U, 1, stk, rop, 1);
      GDCC_BC_AddStmnt(Code::Mul+X, 1, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::ShL+U, 1, stk, stk, 2);

      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, lop, 0x0001);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelCheckRop);
      GDCC_BC_AddStmnt(Code::ShR+U,    1, stk, rop, 16);
      GDCC_BC_AddStmnt(Code::Add+U,    1, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop, 0x8000);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelCheckRop);
      GDCC_BC_AddStmnt(Code::Add+U,    1, stk, stk, 1);

      GDCC_BC_AddLabel(labelCheckRop);
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop, 0x0001);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelShiftLow);
      GDCC_BC_AddStmnt(Code::ShR+U,    1, stk, lop, 16);
      GDCC_BC_AddStmnt(Code::Add+U,    1, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, lop, 0x8000);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelShiftLow);
      GDCC_BC_AddStmnt(Code::Add+U,    1, stk, stk, 1);

      GDCC_BC_AddLabel(labelShiftLow);
      GDCC_BC_AddStmnt(Code::ShR+U, 1, stk, stk, 16);

      // Calculate top 16 bits of high word using a shifted fixed-mul.
      // Shifting the two arguments by 8 shifts the result window over 16 bits,
      // but sacrifices the lower 16 bits of precision.
      GDCC_BC_AddStmnt(Code::ShR+U, 1, stk, lop, 8);
      GDCC_BC_AddStmnt(Code::ShR+U, 1, stk, rop, 8);
      GDCC_BC_AddStmnt(Code::Mul+X, 1, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::BAnd,  1, stk, stk, 0xFFFF0000);

      // Combine both halves of high word.
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, stk);

      GDCC_BC_AddStmnt(Code::Retn, 2, stk);

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

