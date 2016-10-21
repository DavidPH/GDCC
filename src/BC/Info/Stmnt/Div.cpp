//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Division statement handling.
//
//----------------------------------------------------------------------------- 

#include "BC/Info.hpp"

#include "BC/AddFunc.hpp"

#include "IR/Function.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::addFunc_DiXI_W
   //
   void Info::addFunc_DiXI_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre({Code::DiXI_W, n}, n, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n);

      IR::Glyph labelL0R1{prog, name + "$l0r1"};
      IR::Glyph labelL1  {prog, name + "$l1"};
      IR::Glyph labelL1R1{prog, name + "$l1r1"};

      GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, lop + (n - 1), 0x80000000);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru, 1}, stk, labelL1);

      GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, rop + (n - 1), 0x80000000);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru, 1}, stk, labelL0R1);

      // +dividend, +divisor -> +quotient, +remainder
      GDCC_BC_AddStmnt({Code::DiXU_W, n},     stk, lop, rop);
      GDCC_BC_AddStmnt({Code::Retn,   n * 2}, stk);

      GDCC_BC_AddLabel(labelL0R1);
      GDCC_BC_AddStmnt({Code::NegI_W, n}, rop, rop);

      // +dividend, -divisor -> -quotient, -remainder
      GDCC_BC_AddStmnt({Code::DiXU_W, n},     stk, lop, rop);
      GDCC_BC_AddStmnt({Code::NegI_W, n},     rop, stk);
      GDCC_BC_AddStmnt({Code::NegI_W, n},     stk, stk);
      GDCC_BC_AddStmnt({Code::Move_W, n},     stk, rop);
      GDCC_BC_AddStmnt({Code::Retn,   n * 2}, stk);

      GDCC_BC_AddLabel(labelL1);
      GDCC_BC_AddStmnt({Code::NegI_W, n}, lop, lop);

      GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, rop + (n - 1), 0x80000000);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru, 1}, stk, labelL1R1);

      // -dividend, +divisor -> -quotient, +remainder
      GDCC_BC_AddStmnt({Code::DiXU_W, n},     stk, lop, rop);
      GDCC_BC_AddStmnt({Code::Move_W, n},     rop, stk);
      GDCC_BC_AddStmnt({Code::NegI_W, n},     stk, stk);
      GDCC_BC_AddStmnt({Code::Move_W, n},     stk, rop);
      GDCC_BC_AddStmnt({Code::Retn,   n * 2}, stk);

      GDCC_BC_AddLabel(labelL1R1);
      GDCC_BC_AddStmnt({Code::NegI_W, n}, rop, rop);

      // -dividend, -divisor -> +quotient, -remainder
      GDCC_BC_AddStmnt({Code::DiXU_W, n},     stk, lop, rop);
      GDCC_BC_AddStmnt({Code::NegI_W, n},     stk, stk);
      GDCC_BC_AddStmnt({Code::Retn,   n * 2}, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_DiXU_W
   //
   void Info::addFunc_DiXU_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre({Code::DiXU_W, n}, n, n * 2, n * 5, __FILE__);
      GDCC_BC_AddFuncObjBin(n);

      IR::Glyph labelFull    {prog, name + "$full"};
      IR::Glyph labelLoopBody{prog, name + "$loop_body"};
      IR::Glyph labelLoopCond{prog, name + "$loop_cond"};
      IR::Glyph labelLoopShft{prog, name + "$loop_shft"};
      IR::Glyph labelRetn    {prog, name + "$retn"};

      IR::Arg_LocReg quot{GDCC_BC_ArgLit(n * 2)};
      IR::Arg_LocReg rem {GDCC_BC_ArgLit(n * 3)};
      IR::Arg_LocReg mask{GDCC_BC_ArgLit(n * 4)};

      // If both operands have the high word clear, defer to smaller op.
      GDCC_BC_AddStmnt({Code::BOrI_W,   1}, stk, lop + (n - 1), rop + (n - 1));
      GDCC_BC_AddStmnt({Code::Jcnd_Tru, 1}, stk, labelFull);

      GDCC_BC_AddStmnt({Code::DiXU_W, n - 1}, stk, lop, rop);
      GDCC_BC_AddStmnt({Code::Move_W, n - 1}, rop, stk);
      GDCC_BC_AddStmnt({Code::Move_W, n - 1}, lop, stk);

      GDCC_BC_AddStmnt({Code::Retn, n * 2}, lop);

      GDCC_BC_AddLabel(labelFull);
      GDCC_BC_AddStmnt({Code::Move_W, n}, quot, 0);
      GDCC_BC_AddStmnt({Code::Move_W, n}, rem,  lop);

      // If lop < rop, return now.
      GDCC_BC_AddStmnt({Code::CmpU_LT_W, n}, stk, lop, rop);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru,  1}, stk, labelRetn);

      // Calculate mask and adjust divisor.
      GDCC_BC_AddStmnt({Code::Bclz_W, n}, stk, rop);
      GDCC_BC_AddStmnt({Code::Bclz_W, n}, stk, lop);
      GDCC_BC_AddStmnt({Code::SubU_W, 1}, mask, stk, stk);

      GDCC_BC_AddStmnt({Code::ShLU_W, n}, rop,  rop, mask);
      GDCC_BC_AddStmnt({Code::ShLU_W, n}, mask, 1,   mask);

      // Division loop.
      GDCC_BC_AddStmnt({Code::Jump, 1}, labelLoopCond);

      GDCC_BC_AddLabel(labelLoopBody);
      GDCC_BC_AddStmnt({Code::CmpU_GE_W, n}, stk, rem, rop);
      GDCC_BC_AddStmnt({Code::Jcnd_Nil,  1}, stk, labelLoopShft);

      GDCC_BC_AddStmnt({Code::BOrI_W, n}, quot, quot, mask);
      GDCC_BC_AddStmnt({Code::SubU_W, n}, rem,  rem,  rop);

      GDCC_BC_AddLabel(labelLoopShft);
      GDCC_BC_AddStmnt({Code::ShRU_W, n}, rop,  rop,  1);
      GDCC_BC_AddStmnt({Code::ShRU_W, n}, mask, mask, 1);

      GDCC_BC_AddLabel(labelLoopCond);
      // TODO: Use multiword LAnd.
      GDCC_BC_AddStmnt({Code::Move_W,    n}, stk, mask);
      for(Core::FastU i = n; --i;)
         GDCC_BC_AddStmnt({Code::BOrI_W, 1}, stk, stk, stk);
      GDCC_BC_AddStmnt({Code::Move_W,    n}, stk, rem);
      for(Core::FastU i = n; --i;)
         GDCC_BC_AddStmnt({Code::BOrI_W, 1}, stk, stk, stk);
      GDCC_BC_AddStmnt({Code::LAnd,      1}, stk, stk, stk);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru,  1}, stk, labelLoopBody);

      GDCC_BC_AddLabel(labelRetn);
      GDCC_BC_AddStmnt({Code::Retn, n * 2}, quot);

      GDCC_BC_AddFuncEnd();
   }
}

// EOF

