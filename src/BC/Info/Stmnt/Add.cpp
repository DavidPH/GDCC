//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
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
#include "IR/Glyph.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::addFunc_AddF_W
   //
   void Info::addFunc_AddF_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre(Code::AddF, n, n, n * 2, n * 3 + 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      GDCC_BC_AddFuncObjReg(tmp,  n, n * 2);
      GDCC_BC_AddFuncObjReg(expL, 1, n * 3 + 0);
      GDCC_BC_AddFuncObjReg(expR, 1, n * 3 + 1);

      FloatInfo fi = getFloatInfo(n);

      IR::Glyph labelLPos{prog, name + "$lpos"};
      IR::Glyph labelRPos{prog, name + "$rpos"};

      IR::Glyph labelLEMax{prog, name + "$lemax"};
      IR::Glyph labelLEMin{prog, name + "$lemin"};
      IR::Glyph labelREMax{prog, name + "$remax"};
      IR::Glyph labelREMin{prog, name + "$remin"};

      IR::Glyph labelLEQRRet{prog, name + "$leqr_ret"};
      IR::Glyph labelLGTR   {prog, name + "$lgtr"};
      IR::Glyph labelLGTRRet{prog, name + "$lgtr_ret"};
      IR::Glyph labelLLTR   {prog, name + "$lltr"};
      IR::Glyph labelLLTRRet{prog, name + "$lltr_ret"};

      IR::Glyph labelInfinity{prog, name + "$infinity"};

      // Is l negative? l + r = r + l = r - -l
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, lop.hi, fi.maskSig);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelLPos);
      GDCC_BC_AddStmnt(Code::Move,     n, stk, rop);
      GDCC_BC_AddStmnt(Code::NegF,     n, stk, lop);
      GDCC_BC_AddStmnt(Code::SubF,     n, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::Retn,     n, stk);
      GDCC_BC_AddLabel(labelLPos);

      // Is r negative? l + r = l - -r
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, fi.maskSig);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelRPos);
      GDCC_BC_AddStmnt(Code::Move,     n, stk, lop);
      GDCC_BC_AddStmnt(Code::NegF,     n, stk, rop);
      GDCC_BC_AddStmnt(Code::SubF,     n, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::Retn,     n, stk);
      GDCC_BC_AddLabel(labelRPos);

      // Does l have special exponent?
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk,  lop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk,  fi.maskExp, labelLEMax, 0, labelLEMin);
      GDCC_BC_AddStmnt(Code::ShRI,     1, expL, stk, fi.bitsMan);

      // Does r have special exponent?
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk,  rop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk,  fi.maskExp, labelREMax, 0, labelREMin);
      GDCC_BC_AddStmnt(Code::ShRI,     1, expR, stk, fi.bitsMan);

      // Both operands are normalized.

      // Is l.exp > r.exp?
      GDCC_BC_AddStmnt(Code::CmpI_GT,  1, stk, expL, expR);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelLGTR);

      // Is l.exp < r.exp?
      GDCC_BC_AddStmnt(Code::CmpI_LT,  1, stk, expL, expR);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelLLTR);

      // l.exp == r.exp

      // tmp = l.manfull + r.manfull;
      GDCC_BC_AddStmnt(Code::Move, n, stk, lop);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt(Code::Move, n, stk, rop);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt(Code::AddU, n, tmp, stk, stk);

      // If mantissa overflow, increment exponent.
      GDCC_BC_AddStmnt(Code::CmpI_LE,  1, stk, tmp.hi, fi.maskMan * 2 + 1);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelLEQRRet);

      GDCC_BC_AddStmnt(Code::ShRI,     1, tmp,  tmp,  1);
      GDCC_BC_AddStmnt(Code::AddI,     1, expL, expL, 1);

      // If exponent overflow, return infinity.
      GDCC_BC_AddStmnt(Code::CmpI_EQ,  1, stk, expL, fi.maxExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelInfinity);

      GDCC_BC_AddLabel(labelLEQRRet);
      GDCC_BC_AddStmnt(Code::Move, n, stk, tmp);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::ShLU, 1, stk, expL, fi.bitsMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      // l.exp > r.exp

      GDCC_BC_AddLabel(labelLGTR);

      // Calculate exponent difference.
      GDCC_BC_AddStmnt(Code::SubU, 1, tmp.lo, expL, expR);

      // If difference >= total mantissa bits, r is too small to affect l.
      GDCC_BC_AddStmnt(Code::CmpI_GE,  1, stk, tmp.lo, fi.bitsManFull + 1);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelREMin);

      // tmp = l.manfull + (r.manfull >> difference);
      GDCC_BC_AddStmnt(Code::Move, n, stk, lop);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt(Code::Move, n, stk, rop);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt(Code::ShRI, n, stk, stk, tmp.lo);
      GDCC_BC_AddStmnt(Code::AddU, n, tmp, stk, stk);

      // If mantissa overflow, increment exponent.
      GDCC_BC_AddStmnt(Code::CmpI_LE,  1, stk, tmp.hi, fi.maskMan * 2 + 1);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelLGTRRet);

      GDCC_BC_AddStmnt(Code::ShRI,     1, tmp,  tmp,  1);
      GDCC_BC_AddStmnt(Code::AddI,     1, expL, expL, 1);

      // If exponent overflow, return infinity.
      GDCC_BC_AddStmnt(Code::CmpI_EQ,  1, stk, expL, fi.maxExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelInfinity);

      GDCC_BC_AddLabel(labelLGTRRet);
      GDCC_BC_AddStmnt(Code::Move, n, stk, tmp);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::ShLU, 1, stk, expL, fi.bitsMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      // l.exp < r.exp

      GDCC_BC_AddLabel(labelLLTR);

      // Calculate exponent difference.
      GDCC_BC_AddStmnt(Code::SubU, 1, tmp.lo, expR, expL);

      // If difference >= total mantissa bits, l is too small to affect r.
      GDCC_BC_AddStmnt(Code::CmpI_GE,  1, stk, tmp.lo, fi.bitsManFull + 1);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelLEMin);

      // tmp = (l.manfull >> difference) + r.manfull;
      GDCC_BC_AddStmnt(Code::Move, n, stk, lop);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt(Code::ShRI, n, stk, stk, tmp[0]);
      GDCC_BC_AddStmnt(Code::Move, n, stk, rop);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt(Code::AddU, n, tmp, stk, stk);

      // If mantissa overflow, increment exponent.
      GDCC_BC_AddStmnt(Code::CmpI_LE,  1, stk, tmp.hi, fi.maskMan * 2 + 1);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelLLTRRet);

      GDCC_BC_AddStmnt(Code::ShRI,     1, tmp,  tmp,  1);
      GDCC_BC_AddStmnt(Code::AddI,     1, expR, expR, 1);

      // If exponent overflow, return infinity.
      GDCC_BC_AddStmnt(Code::CmpI_EQ,  1, stk, expR, fi.maxExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelInfinity);

      GDCC_BC_AddLabel(labelLLTRRet);
      GDCC_BC_AddStmnt(Code::Move, n, stk, tmp);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk,  fi.maskMan);
      GDCC_BC_AddStmnt(Code::ShLU, 1, stk, expR, fi.bitsMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk,  stk);
      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      // Special exponents.

      // l has max exponent. It is either INF or NaN. Either way, return it.
      GDCC_BC_AddLabel(labelLEMax);
      // Unless r is NaN, then return r.
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::CmpU_EQ,  1, stk, stk,    fi.maskExp);
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, fi.maskMan);
      for(Core::FastU i = n - 1; i--;)
         GDCC_BC_AddStmnt(Code::BOrI,  1, stk, stk, rop[i]);
      GDCC_BC_AddStmnt(Code::LAnd,     1, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelLEMin);

      // r has min exponent. Therefore, r == 0 and the result is l.
      GDCC_BC_AddLabel(labelREMin);
      GDCC_BC_AddStmnt(Code::Retn, n, lop);

      // r has max exponent. It is either INF or NaN. Either way, return it.
      GDCC_BC_AddLabel(labelREMax);
      // l has min exponent. Therefore, l == 0 and the result is r.
      GDCC_BC_AddLabel(labelLEMin);
      GDCC_BC_AddStmnt(Code::Retn, n, rop);

      // Return infinity.
      GDCC_BC_AddLabel(labelInfinity);
      for(Core::FastU i = n - 1; i--;)
         GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);
      GDCC_BC_AddStmnt(Code::Move,    1, stk, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Retn,    n, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_AddU_W
   //
   void Info::addFunc_AddU_W(Core::FastU n)
   {
      addFunc_AddU_W(n, IR::Code::AddU, IR::Code::AdXU);
   }

   //
   // Info::addFunc_AddU_W
   //
   void Info::addFunc_AddU_W(Core::FastU n, IR::Code codeAdd, IR::Code codeAdX)
   {
      GDCC_BC_AddFuncPre(codeAdd, n, n, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      // First words.
      GDCC_BC_AddStmnt(codeAdX, 1, Stk(2), lop.lo, rop.lo);

      // Mid words.
      for(Core::FastU i = 1; i != n - 1; ++i)
         GDCC_BC_AddStmnt(codeAdX, 1, Stk(2), stk, lop[i], rop[i]);

      // Last words.
      GDCC_BC_AddStmnt(codeAdd,    1, stk, lop.hi, rop.hi);
      GDCC_BC_AddStmnt(Code::AddU, 1, stk, stk,    stk);

      // Return.
      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_SubF_W
   //
   void Info::addFunc_SubF_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre(Code::SubF, n, n, n * 2, n * 3 + 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      GDCC_BC_AddFuncObjReg(tmp,  n, n * 2);
      GDCC_BC_AddFuncObjReg(expL, 1, n * 3 + 0);
      GDCC_BC_AddFuncObjReg(expR, 1, n * 3 + 1);

      FloatInfo fi = getFloatInfo(n);

      IR::Glyph labelLPos{prog, name + "$lpos"};
      IR::Glyph labelRPos{prog, name + "$rpos"};

      IR::Glyph labelLEMax{prog, name + "$lemax"};
      IR::Glyph labelLEMin{prog, name + "$lemin"};
      IR::Glyph labelREMax{prog, name + "$remax"};
      IR::Glyph labelREMin{prog, name + "$remin"};

      IR::Glyph labelLREMax{prog, name + "$lremax"};

      IR::Glyph labelLGTR{prog, name + "$lgtr"};
      IR::Glyph labelLLTR{prog, name + "$lltr"};

      IR::Glyph labelNeg0{prog, name + "$neg0"};
      IR::Glyph labelPos0{prog, name + "$pos0"};

      // +0 - +0 = +0
      GDCC_BC_AddStmnt(Code::BOrI,     n, stk, lop, rop);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, Stk(n), labelPos0);

      // -0 - -0 = +0
      GDCC_BC_AddStmnt(Code::BOrI,     n, stk, lop, rop);
      GDCC_BC_AddStmnt(Code::CmpU_NE,  1, stk, stk, fi.maskSig);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, Stk(n), labelPos0);

      // Is l negative? l - r = -(-l + r)
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, lop.hi, fi.maskSig);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelLPos);
      GDCC_BC_AddStmnt(Code::NegF,     n, stk, lop);
      GDCC_BC_AddStmnt(Code::Move,     n, stk, rop);
      GDCC_BC_AddStmnt(Code::AddF,     n, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::NegF,     n, stk, stk);
      GDCC_BC_AddStmnt(Code::Retn,     n, stk);
      GDCC_BC_AddLabel(labelLPos);

      // Is r negative? l - r = l + -r
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, fi.maskSig);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelRPos);
      GDCC_BC_AddStmnt(Code::Move,     n, stk, lop);
      GDCC_BC_AddStmnt(Code::NegF,     n, stk, rop);
      GDCC_BC_AddStmnt(Code::AddF,     n, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::Retn,     n, stk);
      GDCC_BC_AddLabel(labelRPos);

      // Does l have special exponent?
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, lop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, fi.maskExp, labelLEMax, 0, labelLEMin);
      GDCC_BC_AddStmnt(Code::ShRI,     1, expL, stk, fi.bitsMan);

      // Does r have special exponent?
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, fi.maskExp, labelREMax, 0, labelREMin);
      GDCC_BC_AddStmnt(Code::ShRI,     1, expR, stk, fi.bitsMan);

      // Both are normalized.

      // Is l > r?
      GDCC_BC_AddStmnt(Code::CmpI_GT,  1, stk, lop, rop);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelLGTR);

      // Is l < r?
      GDCC_BC_AddStmnt(Code::CmpI_LT,  1, stk, lop, rop);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelLLTR);

      // l == r, return +0.
      GDCC_BC_AddStmnt(Code::Retn, n, 0);

      // l > r

      GDCC_BC_AddLabel(labelLGTR);

      // Calculate exponent difference.
      GDCC_BC_AddStmnt(Code::SubU, 1, tmp.lo, expL, expR);

      // If difference >= total mantissa bits, r is too small to affect l.
      GDCC_BC_AddStmnt(Code::CmpI_GE,  1, stk, tmp.lo, fi.bitsManFull + 1);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelREMin);

      // tmp = l.manfull - (r.manfull >> difference);
      GDCC_BC_AddStmnt(Code::Move, n, stk, lop);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt(Code::Move, n, stk, rop);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt(Code::ShRI, n, stk, stk, tmp.lo);
      GDCC_BC_AddStmnt(Code::SubU, n, tmp, stk, stk);

      // Adjust mantissa to cover hidden bit.
      GDCC_BC_AddStmnt(Code::Bclz, n, expR, tmp);
      GDCC_BC_AddStmnt(Code::SubU, 1, expR, expR, fi.bitsExp);
      GDCC_BC_AddStmnt(Code::SubU, 1, expL, expL, expR);
      GDCC_BC_AddStmnt(Code::ShLU, n, tmp,  tmp,  expR);

      // If exponent <= 0, return +0.
      GDCC_BC_AddStmnt(Code::CmpI_LE,  1, stk, expL, 0);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelPos0);

      // Otherwise, combine exponent and mantissa to form result.
      GDCC_BC_AddStmnt(Code::Move, n, stk, tmp);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk,  fi.maskMan);
      GDCC_BC_AddStmnt(Code::ShLU, 1, stk, expL, fi.bitsMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk,  stk);
      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      // l < r

      GDCC_BC_AddLabel(labelLLTR);

      // Calculate exponent difference.
      GDCC_BC_AddStmnt(Code::SubU, 1, tmp.lo, expR, expL);

      // If difference >= total mantissa bits, l is too small to affect r.
      GDCC_BC_AddStmnt(Code::CmpI_GE,  1, stk, tmp.lo, fi.bitsManFull + 1);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelLEMin);

      // tmp = -(r.manfull - (l.manfull >> difference));
      GDCC_BC_AddStmnt(Code::Move, n, stk, rop);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt(Code::Move, n, stk, lop);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt(Code::ShRI, n, stk, stk, tmp.lo);
      GDCC_BC_AddStmnt(Code::SubU, n, tmp, stk, stk);

      // Adjust mantissa to cover hidden bit.
      GDCC_BC_AddStmnt(Code::Bclz, n, expL, tmp);
      GDCC_BC_AddStmnt(Code::SubU, 1, expL, expL, fi.bitsExp);
      GDCC_BC_AddStmnt(Code::SubU, 1, expR, expR, expL);
      GDCC_BC_AddStmnt(Code::ShLU, n, tmp,  tmp,  expL);

      // If exponent <= 0, return -0.
      GDCC_BC_AddStmnt(Code::CmpI_LE,  1, stk, expR, 0);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelNeg0);

      // Otherwise, combine exponent and mantissa to form result.
      GDCC_BC_AddStmnt(Code::Move, n, stk, tmp);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::ShLU, 1, stk, expR, fi.bitsMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::NegF, n, stk, stk);
      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      // l has max exponent. It is either INF or NaN.
      GDCC_BC_AddLabel(labelLEMax);
      // Check r for max exponent.
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, fi.maskExp, labelLREMax);
      GDCC_BC_AddStmnt(Code::Move,     1, nul, stk);

      // r has min exponent. Therefore, r == 0 and the result is l.
      GDCC_BC_AddLabel(labelREMin);
      GDCC_BC_AddStmnt(Code::Retn, n, lop);

      // r has max exponent. It is either INF or NaN.
      GDCC_BC_AddLabel(labelREMax);
      // l has min exponent. Therefore, l == 0 and the result is -r.
      GDCC_BC_AddLabel(labelLEMin);
      GDCC_BC_AddStmnt(Code::NegF, n, stk, rop);
      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      // l and r have max exponent.
      GDCC_BC_AddLabel(labelLREMax);

      // Is l NaN? If so, return l.
      GDCC_BC_AddStmnt(Code::Move,     n, stk, lop);
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, n, stk, labelREMin);

      // Is r NaN? If so, return r. (Sign inversion is fine.)
      GDCC_BC_AddStmnt(Code::Move,     n, stk, rop);
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, n, stk, labelLEMin);

      // +INF - +INF = NaN.
      for(auto i = n - 1; i--;)
         GDCC_BC_AddStmnt(Code::Move, 1, stk, 0xFFFFFFFF);
      GDCC_BC_AddStmnt(Code::Move,    1, stk, fi.maskExp | fi.maskMan);
      GDCC_BC_AddStmnt(Code::Retn,    n, stk);

      // Return +0.
      GDCC_BC_AddLabel(labelPos0);
      GDCC_BC_AddStmnt(Code::Retn, n, 0);

      // Return -0.
      GDCC_BC_AddLabel(labelNeg0);
      for(auto i = n - 1; i--;)
         GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);
      GDCC_BC_AddStmnt(Code::Move,    1, stk, fi.maskSig);
      GDCC_BC_AddStmnt(Code::Retn,    n, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_SubU_W
   //
   void Info::addFunc_SubU_W(Core::FastU n)
   {
      addFunc_AddU_W(n, IR::Code::SubU, IR::Code::SuXU);
   }
}

// EOF

