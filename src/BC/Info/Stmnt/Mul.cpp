//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
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
#include "IR/Glyph.hpp"


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
      GDCC_BC_AddFuncPre(Code::MuXU, n, n * 2, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      GDCC_BC_AddStmnt(Code::MuXU, 2, stk, lop[0], rop[0]);
      GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);

      Core::FastU colEnd = n * 2 - 1;
      for(Core::FastU col = 1; col != colEnd; ++col)
      {
         if(colEnd - col > 1)
            GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);

         Core::FastU sizeAdd = col < colEnd - 1 ? 3 : 2;

         Core::FastU rowEnd = std::min(col + 1, n);
         Core::FastU lo     = std::min(col, n - 1);
         Core::FastU ro     = col < n ? 0 : col - n + 1;
         for(; ro != rowEnd; --lo, ++ro)
         {
            GDCC_BC_AddStmnt(Code::MuXU, 2, stk, lop[lo], rop[ro]);

            if(sizeAdd == 3)
               GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);

            GDCC_BC_AddStmnt(Code::AddU, sizeAdd, stk, stk, stk);
         }
      }

      GDCC_BC_AddStmnt(Code::Retn, n * 2, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_MulF_W
   //
   void Info::addFunc_MulF_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre(Code::MulF, n, n, n * 2, n * 3 + 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      GDCC_BC_AddFuncObjReg(tmp, n, n * 2);
      GDCC_BC_AddFuncObjReg(exp, 1, n * 3 + 0);
      GDCC_BC_AddFuncObjReg(sig, 1, n * 3 + 1);

      FloatInfo fi = getFloatInfo(n);

      IR::Glyph labelCapMan{prog, name + "$capman"};
      IR::Glyph labelRet0  {prog, name + "$ret0"};
      IR::Glyph labelRetL  {prog, name + "$retl"};
      IR::Glyph labelRetR  {prog, name + "$retr"};
      IR::Glyph labelRetINF{prog, name + "$retinf"};

      // Determine result sign.
      GDCC_BC_AddStmnt(Code::BOrX, 1, stk, lop.hi, rop.hi);
      GDCC_BC_AddStmnt(Code::BAnd, 1, sig, stk, fi.maskSig);

      // Clear operand signs.
      GDCC_BC_AddStmnt(Code::BAnd, 1, lop.hi, lop.hi, fi.maskExp | fi.maskMan);
      GDCC_BC_AddStmnt(Code::BAnd, 1, rop.hi, rop.hi, fi.maskExp | fi.maskMan);

      // Check for l being NaN before other special exponent checks.
      if(n > 1)
      {
         GDCC_BC_AddStmnt(Code::Move,    1, stk, lop[0]);
         for(Core::FastU i = 1, e = n - 1; i != e; ++i)
            GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, lop[i]);

         GDCC_BC_AddStmnt(Code::CmpI_GT, 1, stk, lop.hi, fi.maskExp);
         GDCC_BC_AddStmnt(Code::LAnd,    1, stk, stk,    stk);
      }
      else
         GDCC_BC_AddStmnt(Code::CmpI_GT, 1, stk, lop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru,   1, stk, labelRetL);

      // Check for special operands and calculate result exponent.
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, fi.maskExp, labelRetR, 0, labelRetR);
      GDCC_BC_AddStmnt(Code::ShRI,     1, exp, stk, fi.bitsMan);
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, lop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, fi.maskExp, labelRetL, 0, labelRetL);
      GDCC_BC_AddStmnt(Code::ShRI,     1, stk, stk, fi.bitsMan);
      GDCC_BC_AddStmnt(Code::AddU,     1, exp, exp, stk);
      GDCC_BC_AddStmnt(Code::SubU,     1, exp, exp, fi.offExp);

      // Perform expanding multiply.
      GDCC_BC_AddStmnt(Code::Move, n, stk, lop);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt(Code::Move, n, stk, rop);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt(Code::MuXU, n, Stk(2), stk, stk);

      // Discard the lower bits.
      GDCC_BC_AddStmnt(Code::ShRI, 1, Stk(n + 1), Stk(n + 1), fi.bitsMan);
      GDCC_BC_AddStmnt(Code::Move, 1, nul, stk);
      GDCC_BC_AddStmnt(Code::Move, n, tmp, stk);
      if(n > 1)
         GDCC_BC_AddStmnt(Code::Move, n - 1, nul, stk);

      // Check high bit for extra shift.
      GDCC_BC_AddStmnt(Code::CmpI_GT,  1, stk, tmp.hi, fi.maskMan * 2 + 1);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelCapMan);

      GDCC_BC_AddStmnt(Code::ShRI,     1, tmp, tmp, 1);
      GDCC_BC_AddStmnt(Code::AddU,     1, exp, exp, 1);

      GDCC_BC_AddLabel(labelCapMan);

      // Check exponent range.
      GDCC_BC_AddStmnt(Code::CmpI_GE,  1, stk, exp, fi.maxExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelRetINF);
      GDCC_BC_AddStmnt(Code::CmpI_LE,  1, stk, exp, 0);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelRet0);

      // return result.
      GDCC_BC_AddStmnt(Code::Move, n, stk, tmp);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::ShLU, 1, stk, exp, fi.bitsMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, stk);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, sig);
      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      // Return l with sign.
      GDCC_BC_AddLabel(labelRetL);
      GDCC_BC_AddStmnt(Code::Move, n, stk, lop);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, sig);
      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      // Return r with sign.
      GDCC_BC_AddLabel(labelRetR);
      GDCC_BC_AddStmnt(Code::Move, n, stk, rop);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, sig);
      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      // Return infinity with sign.
      GDCC_BC_AddLabel(labelRetINF);
      if(n > 1)
         GDCC_BC_AddStmnt(Code::Move, n - 1, stk, 0);
      GDCC_BC_AddStmnt(Code::BOrI,    1,     stk, sig, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Retn,    n,     stk);

      // Return zero with sign.
      GDCC_BC_AddLabel(labelRet0);
      if(n > 1)
         GDCC_BC_AddStmnt(Code::Move, n - 1, stk, 0);
      GDCC_BC_AddStmnt(Code::Move,    1,     stk, sig);
      GDCC_BC_AddStmnt(Code::Retn,    n,     stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_MulK_W
   //
   void Info::addFunc_MulK_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre(Code::MulK, n, n, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      FixedInfo fi = getFixedInfo(n, false);

      Core::FastU nf = fi.wordsF;
      Core::FastU nm = n + nf;

      if(nm == n * 2)
      {
         GDCC_BC_AddStmnt(Code::MuXU, nm, stk, lop, rop);
         GDCC_BC_AddStmnt(Code::ShRU, nm, stk, stk, fi.bitsF);
         GDCC_BC_AddStmnt(Code::Move, nf, nul, stk);
         GDCC_BC_AddStmnt(Code::Retn, n,  stk);
      }
      else
      {
         GDCC_BC_AddStmnt(Code::Move, n,  stk, lop);
         GDCC_BC_AddStmnt(Code::Move, nf, stk, 0);
         GDCC_BC_AddStmnt(Code::Move, n,  stk, rop);
         GDCC_BC_AddStmnt(Code::Move, nf, stk, 0);

         GDCC_BC_AddStmnt(Code::MulU, nm, stk, stk, stk);

         GDCC_BC_AddStmnt(Code::Move, n,  lop, stk);
         GDCC_BC_AddStmnt(Code::Move, nf, nul, stk);

         GDCC_BC_AddStmnt(Code::Retn, n,  lop);
      }

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_MulU_W
   //
   void Info::addFunc_MulU_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre(Code::MulU, n, n, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      GDCC_BC_AddStmnt(Code::MuXU, 2, stk, lop[0], rop[0]);

      if(n > 2)
         GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);

      for(Core::FastU col = 1; col != n; ++col)
      {
         if(n - col >= 3)
            GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);

         Core::FastU sizeAdd = std::min<Core::FastU>(3, n - col);
         Core::FastU sizeMul = col < n - 1 ? 2 : 1;
         IR::Code    codeMul = sizeMul == 2 ? IR::Code::MuXU : IR::Code::MulU;

         Core::FastU rowEnd = col + 1;
         Core::FastU lo = col;
         Core::FastU ro = 0;
         for(; ro != rowEnd; --lo, ++ro)
         {
            GDCC_BC_AddStmnt(codeMul, sizeMul, stk, lop[lo], rop[ro]);

            if(sizeAdd == 3)
               GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);

            GDCC_BC_AddStmnt(Code::AddU, sizeAdd, stk, stk, stk);
         }
      }

      GDCC_BC_AddStmnt(Code::Retn, n, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_MulX_W
   //
   void Info::addFunc_MulX_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre(Code::MulX, n, n, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      FixedInfo fi = getFixedInfo(n, true);

      Core::FastU nf = fi.wordsF;
      Core::FastU nm = n + nf;

      GDCC_BC_AddStmnt(Code::Move, n, stk, lop);
      GDCC_BC_AddStmnt(Code::ShRI, 1, stk, lop.hi, 31);
      for(Core::FastU i = nf - 1; i--;)
         GDCC_BC_AddStmnt(Code::Copy, 1, stk, stk);

      GDCC_BC_AddStmnt(Code::Move, n, stk, rop);
      GDCC_BC_AddStmnt(Code::ShRI, 1, stk, rop.hi, 31);
      for(Core::FastU i = nf - 1; i--;)
         GDCC_BC_AddStmnt(Code::Copy, 1, stk, stk);

      GDCC_BC_AddStmnt(Code::MulI, nm, stk, stk, stk);

      if(fi.bitsF % 32)
      {
         GDCC_BC_AddStmnt(Code::ShRI, 1,  Stk(nm), Stk(nm), fi.bitsF);
         GDCC_BC_AddStmnt(Code::Move, nf, nul, stk);

         GDCC_BC_AddStmnt(Code::Retn, n,  stk);
      }
      else
      {
         GDCC_BC_AddStmnt(Code::Move, n,  lop, stk);
         GDCC_BC_AddStmnt(Code::Move, nf, nul, stk);

         GDCC_BC_AddStmnt(Code::Retn, n,  lop);
      }

      GDCC_BC_AddFuncEnd();
   }
}

// EOF

