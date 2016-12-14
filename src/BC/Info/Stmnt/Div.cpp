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

#include "Platform/Platform.hpp"


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

      GDCC_BC_AddFuncObjReg(quot, n * 2);
      GDCC_BC_AddFuncObjReg(rem,  n * 3);
      GDCC_BC_AddFuncObjReg(mask, n * 4);

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

   //
   // Info::addFunc_DivF_W
   //
   void Info::addFunc_DivF_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre({Code::DivF_W, n}, n, n * 2, n * 3 + 3, __FILE__);
      GDCC_BC_AddFuncObjBin(n);

      GDCC_BC_AddFuncObjReg(lhi, n * 1 - 1);
      GDCC_BC_AddFuncObjReg(rhi, n * 2 - 1);
      GDCC_BC_AddFuncObjReg(man, n * 2);
      GDCC_BC_AddFuncObjReg(mhi, n * 3 - 1);
      GDCC_BC_AddFuncObjReg(sig, n * 3 + 0);
      GDCC_BC_AddFuncObjReg(exp, n * 3 + 1);
      GDCC_BC_AddFuncObjReg(tmp, n * 3 + 2);

      FloatInfo fi = getFloatInfo(n);

      IR::Glyph labelL0  {prog, name + "$l0"};
      IR::Glyph labelLINF{prog, name + "$linf"};
      IR::Glyph labelLNaN{prog, name + "$lnan"};
      IR::Glyph labelR0  {prog, name + "$r0"};
      IR::Glyph labelRINF{prog, name + "$rinf"};
      IR::Glyph labelRNaN{prog, name + "$rnan"};

      IR::Glyph labelLoop   {prog, name + "$loop"};
      IR::Glyph labelLoopNil{prog, name + "$loopnil"};

      IR::Glyph label0  {prog, name + "$0"};
      IR::Glyph labelINF{prog, name + "$inf"};
      IR::Glyph labelNaN{prog, name + "$nan"};

      // Determine result sign.
      GDCC_BC_AddStmnt({Code::BOrX_W,    1}, stk, lhi, rhi);
      GDCC_BC_AddStmnt({Code::BAnd_W,    1}, sig, stk, fi.maskSig);

      // Clear operand signs.
      GDCC_BC_AddStmnt({Code::BAnd_W,    1}, lhi, lhi, ~fi.maskSig);
      GDCC_BC_AddStmnt({Code::BAnd_W,    1}, rhi, rhi, ~fi.maskSig);

      // Check for special operands.
      GDCC_BC_AddStmnt({Code::CmpI_GT_W, 1}, stk, lhi, fi.maskExp);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru,  1}, stk, labelLNaN);
      GDCC_BC_AddStmnt({Code::Move_W,    1}, stk, rhi);
      GDCC_BC_AddStmnt({Code::Jcnd_Tab,  1}, stk, 0, labelR0, fi.maskExp, labelRINF);
      GDCC_BC_AddStmnt({Code::CmpI_GT_W, 1}, stk, stk, fi.maskExp);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru,  1}, stk, labelRNaN);
      GDCC_BC_AddStmnt({Code::Move_W,    1}, stk, lhi);
      GDCC_BC_AddStmnt({Code::Jcnd_Tab,  1}, stk, 0, labelL0, fi.maskExp, labelLINF);
      GDCC_BC_AddStmnt({Code::Move_W,    1}, nul, stk);

      // Determine result exponent. Will be adjusted, so no range check.
      GDCC_BC_AddStmnt({Code::Move_W,    1}, stk, fi.offExp);
      GDCC_BC_AddStmnt({Code::ShRI_W,    1}, stk, rhi, fi.bitsMan);
      GDCC_BC_AddStmnt({Code::SubU_W,    1}, stk, stk, stk);
      GDCC_BC_AddStmnt({Code::ShRI_W,    1}, stk, lhi, fi.bitsMan);
      GDCC_BC_AddStmnt({Code::AddU_W,    1}, exp, stk, stk);

      // Clear operand exponents and add implicit bit.
      GDCC_BC_AddStmnt({Code::BAnd_W,    1}, stk, lhi, fi.maskMan);
      GDCC_BC_AddStmnt({Code::BOrI_W,    1}, lhi, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt({Code::BAnd_W,    1}, stk, rhi, fi.maskMan);
      GDCC_BC_AddStmnt({Code::BOrI_W,    1}, rhi, stk, fi.maskMan + 1);

      // Division loop.
      GDCC_BC_AddStmnt({Code::Move_W,    n}, man, 0);
      GDCC_BC_AddStmnt({Code::Move_W,    1}, tmp, fi.bitsManFull);

      // Division check.
      GDCC_BC_AddLabel(labelLoop);
      GDCC_BC_AddStmnt({Code::CmpI_GE_W, n}, stk, lop, rop);
      GDCC_BC_AddStmnt({Code::Jcnd_Nil,  1}, stk, labelLoopNil);
      GDCC_BC_AddStmnt({Code::SubU_W,    n}, lop, lop, rop);
      GDCC_BC_AddStmnt({Code::AddU_W,    1}, man, man, 1);

      GDCC_BC_AddLabel(labelLoopNil);
      GDCC_BC_AddStmnt({Code::ShLU_W,    n}, lop, lop, 1);
      GDCC_BC_AddStmnt({Code::ShLU_W,    n}, man, man, 1);

      GDCC_BC_AddStmnt({Code::SubU_W,    1}, tmp, tmp, 1);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru,  1}, tmp, labelLoop);

      // Final division check.
      GDCC_BC_AddStmnt({Code::CmpI_GE_W, n}, stk, lop, rop);
      GDCC_BC_AddStmnt({Code::AddU_W,    1}, man, man, stk);

      // Shift 1 into implicit bit.
      GDCC_BC_AddStmnt({Code::Bclz_W,    n}, tmp, man);
      GDCC_BC_AddStmnt({Code::SubU_W,    1}, tmp, tmp, fi.bitsExp);

      GDCC_BC_AddStmnt({Code::ShLU_W,    n}, man, man, tmp);
      GDCC_BC_AddStmnt({Code::SubU_W,    1}, exp, exp, tmp);

      GDCC_BC_AddStmnt({Code::BAnd_W,    1}, mhi, mhi, fi.maskMan);

      // Check for out of range exponent.
      GDCC_BC_AddStmnt({Code::CmpI_GE_W, 1}, stk, exp, fi.maxExp);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru,  1}, stk, labelINF);
      GDCC_BC_AddStmnt({Code::CmpI_LE_W, 1}, stk, exp, 0);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru,  1}, stk, label0);

      // Return result.
      GDCC_BC_AddStmnt({Code::Move_W,    n}, stk, man);
      GDCC_BC_AddStmnt({Code::ShLU_W,    1}, stk, exp, fi.bitsMan);
      GDCC_BC_AddStmnt({Code::BOrI_W,    1}, stk, stk, stk);
      GDCC_BC_AddStmnt({Code::BOrI_W,    1}, stk, stk, sig);
      GDCC_BC_AddStmnt({Code::Retn,      n}, stk);

      // Return NaN.
      GDCC_BC_AddLabel(labelNaN);
      for(auto i = n - 1; i--;)
         GDCC_BC_AddStmnt({Code::Move_W, 1}, stk, 0xFFFFFFFF);
      GDCC_BC_AddStmnt({Code::BOrI_W,    1}, stk, sig, fi.maskExp | fi.maskMan);
      GDCC_BC_AddStmnt({Code::Retn,      n}, stk);

      // l is NaN. Therefore, result is l.
      GDCC_BC_AddLabel(labelLNaN);
      // l is 0, r is normal. Therefore, result is l.
      GDCC_BC_AddLabel(labelL0);
      // l is INF, r is normal. Therefore, result is l.
      GDCC_BC_AddLabel(labelLINF);
      GDCC_BC_AddStmnt({Code::Move_W,    n}, stk, lop);
      GDCC_BC_AddStmnt({Code::BOrI_W,    1}, stk, stk, sig);
      GDCC_BC_AddStmnt({Code::Retn,      n}, stk);

      // r is NaN. Therefore, result is r.
      GDCC_BC_AddLabel(labelRNaN);
      GDCC_BC_AddStmnt({Code::Move_W,    n}, stk, rop);
      GDCC_BC_AddStmnt({Code::BOrI_W,    1}, stk, stk, sig);
      GDCC_BC_AddStmnt({Code::Retn,      n}, stk);

      // r is 0.
      GDCC_BC_AddLabel(labelR0);

      // 0 / 0 = NaN.
      GDCC_BC_AddStmnt({Code::Jcnd_Nil, n}, lop, labelNaN);

      // Otherwise, result is INF.
      GDCC_BC_AddLabel(labelINF);
      for(auto i = n - 1; i--;)
         GDCC_BC_AddStmnt({Code::Move_W, 1}, stk, 0);
      GDCC_BC_AddStmnt({Code::BOrI_W,    1}, stk, sig, fi.maskExp);
      GDCC_BC_AddStmnt({Code::Retn,      n}, stk);

      // r is INF.
      GDCC_BC_AddLabel(labelRINF);

      // INF / INF = NaN.
      GDCC_BC_AddStmnt({Code::CmpU_EQ_W, 1}, stk, lhi, fi.maskMan);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru,  1}, stk, labelNaN);

      // Otherwie result is 0.
      GDCC_BC_AddLabel(label0);
      for(auto i = n - 1; i--;)
         GDCC_BC_AddStmnt({Code::Move_W, 1}, stk, 0);
      GDCC_BC_AddStmnt({Code::Move_W,    1}, stk, sig);
      GDCC_BC_AddStmnt({Code::Retn,      n}, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_DivK_W
   //
   void Info::addFunc_DivK_W(Core::FastU n)
   {
      addFunc_DivX_W(n, IR::Code::DivK_W, IR::Code::DivU_W, false);
   }

   //
   // Info::addFunc_DivX_W
   //
   void Info::addFunc_DivX_W(Core::FastU n)
   {
      addFunc_DivX_W(n, IR::Code::DivX_W, IR::Code::DivI_W, true);
   }

   //
   // Info::addFunc_DivX_W
   //
   void Info::addFunc_DivX_W(Core::FastU n, IR::Code code, IR::Code codeDiv, bool sign)
   {
      GDCC_BC_AddFuncPre({code, n}, n, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n);

      FixedInfo fi = getFixedInfo(n, sign);

      Core::FastU nf = fi.wordsF;
      Core::FastU nd = n + nf;

      if(fi.bitsF % 32)
      {
         GDCC_BC_AddStmnt({Code::Move_W, n},  stk, lop);
         if(sign)
         {
            GDCC_BC_AddStmnt({Code::Copy_W, 1}, stk, stk);
            GDCC_BC_AddStmnt({Code::ShRI_W, 1}, stk, stk, 31);
            for(Core::FastU i = nf - 1; i--;)
               GDCC_BC_AddStmnt({Code::Copy_W, 1}, stk, stk);
         }
         else
            GDCC_BC_AddStmnt({Code::Move_W, nf}, stk, 0);

         GDCC_BC_AddStmnt({Code::ShLU_W, nd}, stk, stk, fi.bitsF);
      }
      else
      {
         GDCC_BC_AddStmnt({Code::Move_W, nf}, stk, 0);
         GDCC_BC_AddStmnt({Code::Move_W, n},  stk, lop);
      }

      GDCC_BC_AddStmnt({Code::Move_W, n}, stk, rop);
      if(sign)
      {
         GDCC_BC_AddStmnt({Code::Copy_W, 1}, stk, stk);
         GDCC_BC_AddStmnt({Code::ShRI_W, 1}, stk, stk, 31);
         for(Core::FastU i = nf - 1; i--;)
            GDCC_BC_AddStmnt({Code::Copy_W, 1}, stk, stk);
      }
      else
         GDCC_BC_AddStmnt({Code::Move_W, nf}, stk, 0);

      GDCC_BC_AddStmnt({codeDiv,      nd}, stk, stk, stk);
      GDCC_BC_AddStmnt({Code::Move_W, nf}, nul, stk);

      GDCC_BC_AddStmnt({Code::Retn, n}, stk);

      GDCC_BC_AddFuncEnd();
   }
}

// EOF

