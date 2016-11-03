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
      GDCC_BC_AddFuncPre({Code::MuXU_W, n}, n * 2, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n);

      GDCC_BC_AddStmnt({Code::MuXU_W, 1}, stk, lop, rop);
      GDCC_BC_AddStmnt({Code::Move_W, 1}, stk, 0);

      Core::FastU colEnd = n * 2 - 1;
      for(Core::FastU col = 1; col != colEnd; ++col)
      {
         if(colEnd - col > 1)
            GDCC_BC_AddStmnt({Code::Move_W, 1}, stk, 0);

         Core::FastU sizeAdd = col < colEnd - 1 ? 3 : 2;

         Core::FastU rowEnd = std::min(col + 1, n);
         lop.off = std::min(col, n - 1);
         rop.off = col < n ? 0 : col - n + 1;
         for(; rop.off != rowEnd; --lop, ++rop)
         {
            GDCC_BC_AddStmnt({Code::MuXU_W, 1}, stk, lop, rop);

            if(sizeAdd == 3)
               GDCC_BC_AddStmnt({Code::Move_W, 1}, stk, 0);

            GDCC_BC_AddStmnt({Code::AddU_W, sizeAdd}, stk, stk, stk);
         }
      }

      GDCC_BC_AddStmnt({Code::Retn, n * 2}, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_MulF_W
   //
   void Info::addFunc_MulF_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre({Code::MulF_W, n}, n, n * 2, n * 3 + 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n);

      IR::Arg_LocReg lhi{GDCC_BC_ArgLit(n * 1 - 1)};
      IR::Arg_LocReg rhi{GDCC_BC_ArgLit(n * 2 - 1)};
      IR::Arg_LocReg tmp{GDCC_BC_ArgLit(n * 2)};
      IR::Arg_LocReg thi{GDCC_BC_ArgLit(n * 3 - 1)};
      IR::Arg_LocReg exp{GDCC_BC_ArgLit(n * 3 + 0)};
      IR::Arg_LocReg sig{GDCC_BC_ArgLit(n * 3 + 1)};

      FloatInfo fi = getFloatInfo(n);

      IR::Glyph labelCapMan{prog, name + "$capman"};
      IR::Glyph labelRet0  {prog, name + "$ret0"};
      IR::Glyph labelRetL  {prog, name + "$retl"};
      IR::Glyph labelRetR  {prog, name + "$retr"};
      IR::Glyph labelRetINF{prog, name + "$retinf"};

      // Determine result sign.
      GDCC_BC_AddStmnt({Code::BOrX_W, 1}, stk, lhi, rhi);
      GDCC_BC_AddStmnt({Code::BAnd_W, 1}, sig, stk, fi.maskSig);

      // Clear operand signs.
      GDCC_BC_AddStmnt({Code::BAnd_W, 1}, lhi, lhi, fi.maskExp | fi.maskMan);
      GDCC_BC_AddStmnt({Code::BAnd_W, 1}, rhi, rhi, fi.maskExp | fi.maskMan);

      // Check for l being NaN before other special exponent checks.
      if(n > 1)
      {
         GDCC_BC_AddStmnt({Code::Move_W,    1}, stk, lop);
         for(Core::FastU i = 1, e = n - 1; i != e; ++i)
            GDCC_BC_AddStmnt({Code::BOrI_W, 1}, stk, stk, lop + i);

         GDCC_BC_AddStmnt({Code::CmpI_GT_W, 1}, stk, lhi, fi.maskExp);
         GDCC_BC_AddStmnt({Code::LAnd,      1}, stk, stk, stk);
      }
      else
         GDCC_BC_AddStmnt({Code::CmpI_GT_W, 1}, stk, lhi, fi.maskExp);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru,     1}, stk, labelRetL);

      // Check for special operands and calculate result exponent.
      GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, rhi, fi.maskExp);
      GDCC_BC_AddStmnt({Code::Jcnd_Tab, 1}, stk, fi.maskExp, labelRetR, 0, labelRetR);
      GDCC_BC_AddStmnt({Code::ShRI_W,   1}, exp, stk, fi.bitsMan);
      GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, lhi, fi.maskExp);
      GDCC_BC_AddStmnt({Code::Jcnd_Tab, 1}, stk, fi.maskExp, labelRetL, 0, labelRetL);
      GDCC_BC_AddStmnt({Code::ShRI_W,   1}, stk, stk, fi.bitsMan);
      GDCC_BC_AddStmnt({Code::AddU_W,   1}, exp, exp, stk);
      GDCC_BC_AddStmnt({Code::SubU_W,   1}, exp, exp, fi.offExp);

      // Perform expanding multiply.
      GDCC_BC_AddStmnt({Code::Move_W, n}, stk, lop);
      GDCC_BC_AddStmnt({Code::BAnd_W, 1}, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt({Code::BOrI_W, 1}, stk, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt({Code::Move_W, n}, stk, rop);
      GDCC_BC_AddStmnt({Code::BAnd_W, 1}, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt({Code::BOrI_W, 1}, stk, stk, fi.maskMan + 1);
      GDCC_BC_AddStmnt({Code::MuXU_W, n}, stk, stk, stk);

      // Discard the lower bits.
      GDCC_BC_AddStmnt({Code::ShRI_W, n + 1}, stk, stk, fi.bitsMan);
      GDCC_BC_AddStmnt({Code::Move_W, 1},     nul, stk);
      GDCC_BC_AddStmnt({Code::Move_W, n},     tmp, stk);
      if(n > 1)
         GDCC_BC_AddStmnt({Code::Move_W, n - 1}, nul, stk);

      // Check high bit for extra shift.
      GDCC_BC_AddStmnt({Code::CmpI_GT_W, 1}, stk, thi, fi.maskMan * 2 + 1);
      GDCC_BC_AddStmnt({Code::Jcnd_Nil,  1}, stk, labelCapMan);

      GDCC_BC_AddStmnt({Code::ShRI_W, n}, tmp, tmp, 1);
      GDCC_BC_AddStmnt({Code::AddU_W, 1}, exp, exp, 1);

      GDCC_BC_AddLabel(labelCapMan);

      // Check exponent range.
      GDCC_BC_AddStmnt({Code::CmpI_GE_W, 1}, stk, exp, fi.maxExp);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru,  1}, stk, labelRetINF);
      GDCC_BC_AddStmnt({Code::CmpI_LE_W, 1}, stk, exp, 0);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru,  1}, stk, labelRet0);

      // return result.
      GDCC_BC_AddStmnt({Code::Move_W, n}, stk, tmp);
      GDCC_BC_AddStmnt({Code::BAnd_W, 1}, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt({Code::ShLU_W, 1}, stk, exp, fi.bitsMan);
      GDCC_BC_AddStmnt({Code::BOrI_W, 1}, stk, stk, stk);
      GDCC_BC_AddStmnt({Code::BOrI_W, 1}, stk, stk, sig);
      GDCC_BC_AddStmnt({Code::Retn,   n}, stk);

      // Return l with sign.
      GDCC_BC_AddLabel(labelRetL);
      GDCC_BC_AddStmnt({Code::Move_W, n}, stk, lop);
      GDCC_BC_AddStmnt({Code::BOrI_W, 1}, stk, stk, sig);
      GDCC_BC_AddStmnt({Code::Retn,   n}, stk);

      // Return r with sign.
      GDCC_BC_AddLabel(labelRetR);
      GDCC_BC_AddStmnt({Code::Move_W, n}, stk, rop);
      GDCC_BC_AddStmnt({Code::BOrI_W, 1}, stk, stk, sig);
      GDCC_BC_AddStmnt({Code::Retn,   n}, stk);

      // Return infinity with sign.
      GDCC_BC_AddLabel(labelRetINF);
      if(n > 1)
         GDCC_BC_AddStmnt({Code::Move_W, n - 1}, stk, 0);
      GDCC_BC_AddStmnt({Code::BOrI_W,    1},     stk, sig, fi.maskExp);
      GDCC_BC_AddStmnt({Code::Retn,      n},     stk);

      // Return zero with sign.
      GDCC_BC_AddLabel(labelRet0);
      if(n > 1)
         GDCC_BC_AddStmnt({Code::Move_W, n - 1}, stk, 0);
      GDCC_BC_AddStmnt({Code::Move_W,    1},     stk, sig);
      GDCC_BC_AddStmnt({Code::Retn,      n},     stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_MulK_W
   //
   void Info::addFunc_MulK_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre({Code::MulK_W, n}, n, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n);

      FixedInfo fi = getFixedInfo(n, false);

      Core::FastU nf = fi.wordsF;
      Core::FastU nm = n + nf;

      if(nm == n * 2)
      {
         GDCC_BC_AddStmnt({Code::MuXU_W, n},  stk, lop, rop);
         GDCC_BC_AddStmnt({Code::ShRU_W, nm}, stk, stk, fi.bitsF);
         GDCC_BC_AddStmnt({Code::Move_W, nf}, nul, stk);
         GDCC_BC_AddStmnt({Code::Retn,   n},  stk);
      }
      else
      {
         GDCC_BC_AddStmnt({Code::Move_W, n},  stk, lop);
         GDCC_BC_AddStmnt({Code::Move_W, nf}, stk, 0);
         GDCC_BC_AddStmnt({Code::Move_W, n},  stk, rop);
         GDCC_BC_AddStmnt({Code::Move_W, nf}, stk, 0);

         GDCC_BC_AddStmnt({Code::MulU_W, nm}, stk, stk, stk);

         GDCC_BC_AddStmnt({Code::Move_W, n},  lop, stk);
         GDCC_BC_AddStmnt({Code::Move_W, nf}, nul, stk);

         GDCC_BC_AddStmnt({Code::Retn,   n},  lop);
      }

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_MulU_W
   //
   void Info::addFunc_MulU_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre({Code::MulU_W, n}, n, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n);

      GDCC_BC_AddStmnt({Code::MuXU_W, 1}, stk, lop, rop);

      if(n > 2)
         GDCC_BC_AddStmnt({Code::Move_W, 1}, stk, 0);

      for(Core::FastU col = 1; col != n; ++col)
      {
         if(n - col > 3)
            GDCC_BC_AddStmnt({Code::Move_W, 1}, stk, 0);

         Core::FastU sizeAdd = std::min<Core::FastU>(3, n - col);
         IR::Code    codeMul = col < n - 1 ? IR::Code::MuXU_W : IR::Code::MulU_W;

         Core::FastU rowEnd = col + 1;
         lop.off = col;
         rop.off = 0;
         for(; rop.off != rowEnd; --lop, ++rop)
         {
            GDCC_BC_AddStmnt({codeMul, 1}, stk, lop, rop);

            if(sizeAdd == 3)
               GDCC_BC_AddStmnt({Code::Move_W, 1}, stk, 0);

            GDCC_BC_AddStmnt({Code::AddU_W, sizeAdd}, stk, stk, stk);
         }
      }

      GDCC_BC_AddStmnt({Code::Retn, n}, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_MulX_W
   //
   void Info::addFunc_MulX_W(Core::FastU n)
   {
      GDCC_BC_AddFuncPre({Code::MulX_W, n}, n, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n);

      FixedInfo fi = getFixedInfo(n, true);

      Core::FastU nf = fi.wordsF;
      Core::FastU nm = n + nf;

      GDCC_BC_AddStmnt({Code::Move_W, n}, stk, lop);
      GDCC_BC_AddStmnt({Code::ShRI_W, 1}, stk, lop + (n - 1), 31);
      for(Core::FastU i = nf - 1; i--;)
         GDCC_BC_AddStmnt({Code::Copy_W, 1}, stk, stk);

      GDCC_BC_AddStmnt({Code::Move_W, n}, stk, rop);
      GDCC_BC_AddStmnt({Code::ShRI_W, 1}, stk, rop + (n - 1), 31);
      for(Core::FastU i = nf - 1; i--;)
         GDCC_BC_AddStmnt({Code::Copy_W, 1}, stk, stk);

      GDCC_BC_AddStmnt({Code::MulI_W, nm}, stk, stk, stk);

      if(fi.bitsF % 32)
      {
         GDCC_BC_AddStmnt({Code::ShRI_W, nm}, stk, stk, fi.bitsF);
         GDCC_BC_AddStmnt({Code::Move_W, nf}, nul, stk);

         GDCC_BC_AddStmnt({Code::Retn,   n},  stk);
      }
      else
      {
         GDCC_BC_AddStmnt({Code::Move_W, n},  lop, stk);
         GDCC_BC_AddStmnt({Code::Move_W, nf}, nul, stk);

         GDCC_BC_AddStmnt({Code::Retn,  n},   lop);
      }

      GDCC_BC_AddFuncEnd();
   }
}

// EOF

