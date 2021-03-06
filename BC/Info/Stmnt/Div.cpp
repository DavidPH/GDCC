//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
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
   // Info::addFunc_Div_AW
   //
   void Info::addFunc_Div_AW(Core::FastU n)
   {
      GDCC_BC_AddFuncPre((Code::Div+A, n), n, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      Core::FastU nf = n;
      Core::FastU nd = n + nf;

      GDCC_BC_AddStmnt(Code::Move,   nf, stk, 0);
      GDCC_BC_AddStmnt(Code::Move,   n,  stk, lop);

      GDCC_BC_AddStmnt(Code::Move,   n,  stk, rop);
      GDCC_BC_AddStmnt(Code::Move,   nf, stk, 0);

      GDCC_BC_AddStmnt(Code::DivX+U, nd, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::Move,   nf, nul, stk);

      GDCC_BC_AddStmnt(Code::Retn,   n,  stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_Div_FW
   //
   void Info::addFunc_Div_FW(Core::FastU n)
   {
      GDCC_BC_AddFuncPre((Code::Div+F, n), n, n * 2, n * 3 + 3, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      GDCC_BC_AddFuncObjReg(man, n, n * 2);
      GDCC_BC_AddFuncObjReg(sig, 1, n * 3 + 0);
      GDCC_BC_AddFuncObjReg(exp, 1, n * 3 + 1);
      GDCC_BC_AddFuncObjReg(tmp, 1, n * 3 + 2);

      FloatInfo fi = getFloatInfo(n, true);

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
      GDCC_BC_AddStmnt(Code::BOrX,     1, stk, lop.hi, rop.hi);
      GDCC_BC_AddStmnt(Code::BAnd,     1, sig, stk,    fi.maskSig);

      // Clear operand signs.
      GDCC_BC_AddStmnt(Code::BAnd,     1, lop.hi, lop.hi, ~fi.maskSig);
      GDCC_BC_AddStmnt(Code::BAnd,     1, rop.hi, rop.hi, ~fi.maskSig);

      // Check for special operands.
      GDCC_BC_AddStmnt(Code::CmpGT+I,  1, stk, lop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelLNaN);
      GDCC_BC_AddStmnt(Code::Move,     1, stk, rop.hi);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, 0, labelR0, fi.maskExp, labelRINF);
      GDCC_BC_AddStmnt(Code::CmpGT+I,  1, stk, stk, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelRNaN);
      GDCC_BC_AddStmnt(Code::Move,     1, stk, lop.hi);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, 0, labelL0, fi.maskExp, labelLINF);
      GDCC_BC_AddStmnt(Code::Move,     1, nul, stk);

      // Determine result exponent. Will be adjusted, so no range check.
      GDCC_BC_AddStmnt(Code::Move,     1, stk, fi.offExp);
      GDCC_BC_AddStmnt(Code::ShR+I,    1, stk, rop.hi, fi.bitsMan);
      GDCC_BC_AddStmnt(Code::Sub+U,    1, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::ShR+I,    1, stk, lop.hi, fi.bitsMan);
      GDCC_BC_AddStmnt(Code::Add+U,    1, exp, stk, stk);

      // Clear operand exponents and add implicit bit.
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, lop.hi, fi.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI,     1, lop.hi, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, fi.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI,     1, rop.hi, stk, fi.maskMan + 1);

      // Division loop.
      GDCC_BC_AddStmnt(Code::Move,     n, man, 0);
      GDCC_BC_AddStmnt(Code::Move,     1, tmp, fi.bitsManFull);

      // Division check.
      GDCC_BC_AddLabel(labelLoop);
      GDCC_BC_AddStmnt(Code::CmpGE+I,  1, stk, lop, rop);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelLoopNil);
      GDCC_BC_AddStmnt(Code::Sub+U,    n, lop, lop, rop);
      GDCC_BC_AddStmnt(Code::Add+U,    n, man, man, 1);

      GDCC_BC_AddLabel(labelLoopNil);
      GDCC_BC_AddStmnt(Code::ShL+U,    1, lop, lop, 1);
      GDCC_BC_AddStmnt(Code::ShL+U,    1, man, man, 1);

      GDCC_BC_AddStmnt(Code::Sub+U,    1, tmp, tmp, 1);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, tmp, labelLoop);

      // Final division check.
      GDCC_BC_AddStmnt(Code::CmpGE+I,  1, stk, lop, rop);
      GDCC_BC_AddStmnt(Code::Add+U,    1, man.lo, man.lo, stk);

      // Shift 1 into implicit bit.
      GDCC_BC_AddStmnt(Code::Bclz,     n, tmp, man);
      GDCC_BC_AddStmnt(Code::Sub+U,    1, tmp, tmp, fi.bitsExp);

      GDCC_BC_AddStmnt(Code::ShL+U,    n, man, man, tmp);
      GDCC_BC_AddStmnt(Code::Sub+U,    1, exp, exp, tmp);

      GDCC_BC_AddStmnt(Code::BAnd,     1, man.hi, man.hi, fi.maskMan);

      // Check for out of range exponent.
      GDCC_BC_AddStmnt(Code::CmpGE+I,  1, stk, exp, fi.maxExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelINF);
      GDCC_BC_AddStmnt(Code::CmpLE+I,  1, stk, exp, 0);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, label0);

      // Return result.
      GDCC_BC_AddStmnt(Code::Move,     n, stk, man);
      GDCC_BC_AddStmnt(Code::ShL+U,    1, stk, exp, fi.bitsMan);
      GDCC_BC_AddStmnt(Code::BOrI,     1, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::BOrI,     1, stk, stk, sig);
      GDCC_BC_AddStmnt(Code::Retn,     n, stk);

      // Return NaN.
      GDCC_BC_AddLabel(labelNaN);
      for(auto i = n - 1; i--;)
         GDCC_BC_AddStmnt(Code::Move,  1, stk, 0xFFFFFFFF);
      GDCC_BC_AddStmnt(Code::BOrI,     1, stk, sig, fi.maskExp | fi.maskMan);
      GDCC_BC_AddStmnt(Code::Retn,     n, stk);

      // l is NaN. Therefore, result is l.
      GDCC_BC_AddLabel(labelLNaN);
      // l is 0, r is normal. Therefore, result is l.
      GDCC_BC_AddLabel(labelL0);
      // l is INF, r is normal. Therefore, result is l.
      GDCC_BC_AddLabel(labelLINF);
      GDCC_BC_AddStmnt(Code::Move,     n, stk, lop);
      GDCC_BC_AddStmnt(Code::BOrI,     1, stk, stk, sig);
      GDCC_BC_AddStmnt(Code::Retn,     n, stk);

      // r is NaN. Therefore, result is r.
      GDCC_BC_AddLabel(labelRNaN);
      GDCC_BC_AddStmnt(Code::Move,     n, stk, rop);
      GDCC_BC_AddStmnt(Code::BOrI,     1, stk, stk, sig);
      GDCC_BC_AddStmnt(Code::Retn,     n, stk);

      // r is 0.
      GDCC_BC_AddLabel(labelR0);

      // 0 / 0 = NaN.
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, n, lop, labelNaN);

      // Otherwise, result is INF.
      GDCC_BC_AddLabel(labelINF);
      for(auto i = n - 1; i--;)
         GDCC_BC_AddStmnt(Code::Move,  1, stk, 0);
      GDCC_BC_AddStmnt(Code::BOrI,     1, stk, sig, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Retn,     n, stk);

      // r is INF.
      GDCC_BC_AddLabel(labelRINF);

      // INF / INF = NaN.
      GDCC_BC_AddStmnt(Code::CmpEQ+U,  1, stk, lop.hi, fi.maskMan);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelNaN);

      // Otherwie result is 0.
      GDCC_BC_AddLabel(label0);
      for(auto i = n - 1; i--;)
         GDCC_BC_AddStmnt(Code::Move,  1, stk, 0);
      GDCC_BC_AddStmnt(Code::Move,     1, stk, sig);
      GDCC_BC_AddStmnt(Code::Retn,     n, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_Div_KW
   //
   void Info::addFunc_Div_KW(Core::FastU n)
   {
      addFunc_Div_XW(n, IR::CodeBase::Div+'K', IR::CodeBase::Div+'U', false);
   }

   //
   // Info::addFunc_Div_RW
   //
   void Info::addFunc_Div_RW(Core::FastU n)
   {
      GDCC_BC_AddFuncPre((Code::Div+R, n), n, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      Core::FastU nf = n;
      Core::FastU nd = n + nf;

      GDCC_BC_AddStmnt(Code::Move,  nf, stk, 0);
      GDCC_BC_AddStmnt(Code::Move,  n,  stk, lop);

      GDCC_BC_AddStmnt(Code::Move,  n, stk, rop);
      GDCC_BC_AddStmnt(Code::Copy,  1, stk, stk);
      GDCC_BC_AddStmnt(Code::ShR+I, 1, stk, stk, 31);
      for(Core::FastU i = nf - 1; i--;)
         GDCC_BC_AddStmnt(Code::Copy, 1, stk, stk);

      GDCC_BC_AddStmnt(Code::DivX+I, nd, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::Move,   nf, nul, stk);

      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_Div_XW
   //
   void Info::addFunc_Div_XW(Core::FastU n)
   {
      addFunc_Div_XW(n, IR::CodeBase::Div+'X', IR::CodeBase::Div+'I', true);
   }

   //
   // Info::addFunc_Div_XW
   //
   void Info::addFunc_Div_XW(Core::FastU n, IR::Code code, IR::Code codeDiv, bool sign)
   {
      GDCC_BC_AddFuncPre((code, n), n, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      FixedInfo fi = getFixedInfo(n, sign);

      Core::FastU nf = fi.wordsF;
      Core::FastU nd = n + nf;

      if(fi.bitsF % 32)
      {
         GDCC_BC_AddStmnt(Code::Move, n, stk, lop);
         if(sign)
         {
            GDCC_BC_AddStmnt(Code::Copy,  1, stk, stk);
            GDCC_BC_AddStmnt(Code::ShR+I, 1, stk, stk, 31);
            for(Core::FastU i = nf - 1; i--;)
               GDCC_BC_AddStmnt(Code::Copy, 1, stk, stk);
         }
         else
            GDCC_BC_AddStmnt(Code::Move, nf, stk, 0);

         GDCC_BC_AddStmnt(Code::ShL+U, 1, Stk(nd), Stk(nd), fi.bitsF);
      }
      else
      {
         GDCC_BC_AddStmnt(Code::Move, nf, stk, 0);
         GDCC_BC_AddStmnt(Code::Move, n,  stk, lop);
      }

      GDCC_BC_AddStmnt(Code::Move, n, stk, rop);
      if(sign)
      {
         GDCC_BC_AddStmnt(Code::Copy,  1, stk, stk);
         GDCC_BC_AddStmnt(Code::ShR+I, 1, stk, stk, 31);
         for(Core::FastU i = nf - 1; i--;)
            GDCC_BC_AddStmnt(Code::Copy, 1, stk, stk);
      }
      else
         GDCC_BC_AddStmnt(Code::Move, nf, stk, 0);

      GDCC_BC_AddStmnt(codeDiv,    nd, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::Move, nf, nul, stk);

      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_DivX_IW
   //
   void Info::addFunc_DivX_IW(Core::FastU n)
   {
      GDCC_BC_AddFuncPre((Code::DivX+I, n), n * 2, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      IR::Glyph labelL0R1{prog, name + "$l0r1"};
      IR::Glyph labelL1  {prog, name + "$l1"};
      IR::Glyph labelL1R1{prog, name + "$l1r1"};

      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, lop.hi, 0x80000000);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelL1);

      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, 0x80000000);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelL0R1);

      // +dividend, +divisor -> +quotient, +remainder
      GDCC_BC_AddStmnt(Code::DivX+U, n * 2, stk, lop, rop);
      GDCC_BC_AddStmnt(Code::Retn,   n * 2, stk);

      GDCC_BC_AddLabel(labelL0R1);
      GDCC_BC_AddStmnt(Code::Neg+I, n, rop, rop);

      // +dividend, -divisor -> -quotient, -remainder
      GDCC_BC_AddStmnt(Code::DivX+U, n * 2, stk, lop, rop);
      GDCC_BC_AddStmnt(Code::Neg+I,  n,     rop, stk);
      GDCC_BC_AddStmnt(Code::Neg+I,  n,     stk, stk);
      GDCC_BC_AddStmnt(Code::Move,   n,     stk, rop);
      GDCC_BC_AddStmnt(Code::Retn,   n * 2, stk);

      GDCC_BC_AddLabel(labelL1);
      GDCC_BC_AddStmnt(Code::Neg+I, n, lop, lop);

      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, 0x80000000);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelL1R1);

      // -dividend, +divisor -> -quotient, +remainder
      GDCC_BC_AddStmnt(Code::DivX+U, n * 2, stk, lop, rop);
      GDCC_BC_AddStmnt(Code::Move,   n,     rop, stk);
      GDCC_BC_AddStmnt(Code::Neg+I,  n,     stk, stk);
      GDCC_BC_AddStmnt(Code::Move,   n,     stk, rop);
      GDCC_BC_AddStmnt(Code::Retn,   n * 2, stk);

      GDCC_BC_AddLabel(labelL1R1);
      GDCC_BC_AddStmnt(Code::Neg+I, n, rop, rop);

      // -dividend, -divisor -> +quotient, -remainder
      GDCC_BC_AddStmnt(Code::DivX+U, n * 2, stk, lop, rop);
      GDCC_BC_AddStmnt(Code::Neg+I,  n,     stk, stk);
      GDCC_BC_AddStmnt(Code::Retn,   n * 2, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_DivX_UW
   //
   void Info::addFunc_DivX_UW(Core::FastU n)
   {
      GDCC_BC_AddFuncPre((Code::DivX+U, n), n * 2, n * 2, n * 5, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      IR::Glyph labelFull    {prog, name + "$full"};
      IR::Glyph labelLoopBody{prog, name + "$loop_body"};
      IR::Glyph labelLoopCond{prog, name + "$loop_cond"};
      IR::Glyph labelLoopShft{prog, name + "$loop_shft"};
      IR::Glyph labelRetn    {prog, name + "$retn"};

      GDCC_BC_AddFuncObjReg(respt, n * 2, n * 0);
      GDCC_BC_AddFuncObjReg(lpart, n - 1, n * 0);
      GDCC_BC_AddFuncObjReg(rpart, n - 1, n * 1);
      GDCC_BC_AddFuncObjReg(res,   n * 2, n * 2);

      GDCC_BC_AddFuncObjReg(quot, n, n * 2);
      GDCC_BC_AddFuncObjReg(rem,  n, n * 3);
      GDCC_BC_AddFuncObjReg(mask, n, n * 4);

      // If both operands have the high word clear, defer to smaller op.
      GDCC_BC_AddStmnt(Code::BOrI,     1, stk, lop.hi, rop.hi);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelFull);

      GDCC_BC_AddStmnt(Code::DivX+U, n*2-2, stk, lpart, rpart);
      GDCC_BC_AddStmnt(Code::Move,   n - 1, rpart, stk);
      GDCC_BC_AddStmnt(Code::Move,   n - 1, lpart, stk);

      GDCC_BC_AddStmnt(Code::Retn, n * 2, respt);

      GDCC_BC_AddLabel(labelFull);
      GDCC_BC_AddStmnt(Code::Move, n, quot, 0);
      GDCC_BC_AddStmnt(Code::Move, n, rem,  lop);

      // If lop < rop, return now.
      GDCC_BC_AddStmnt(Code::CmpLT+U,  1, stk, lop, rop);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelRetn);

      // Calculate mask and adjust divisor.
      GDCC_BC_AddStmnt(Code::Bclz,  1, stk, rop);
      GDCC_BC_AddStmnt(Code::Bclz,  1, stk, lop);
      GDCC_BC_AddStmnt(Code::Sub+U, 1, mask.lo, stk, stk);

      GDCC_BC_AddStmnt(Code::ShL+U, n, rop,  rop, mask.lo);
      GDCC_BC_AddStmnt(Code::ShL+U, n, mask, 1,   mask.lo);

      // Division loop.
      GDCC_BC_AddStmnt(Code::Jump, 1, labelLoopCond);

      GDCC_BC_AddLabel(labelLoopBody);
      GDCC_BC_AddStmnt(Code::CmpGE+U,  1, stk, rem, rop);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelLoopShft);

      GDCC_BC_AddStmnt(Code::BOrI,  n, quot, quot, mask);
      GDCC_BC_AddStmnt(Code::Sub+U, n, rem,  rem,  rop);

      GDCC_BC_AddLabel(labelLoopShft);
      GDCC_BC_AddStmnt(Code::ShR+U, 1, rop,  rop,  1);
      GDCC_BC_AddStmnt(Code::ShR+U, 1, mask, mask, 1);

      GDCC_BC_AddLabel(labelLoopCond);
      GDCC_BC_AddStmnt(Code::LAnd,     1, stk, mask, rem);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelLoopBody);

      GDCC_BC_AddLabel(labelRetn);
      GDCC_BC_AddStmnt(Code::Retn, n * 2, res);

      GDCC_BC_AddFuncEnd();
   }
}

// EOF

