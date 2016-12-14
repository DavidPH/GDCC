//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Compare statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "BC/AddFunc.hpp"

#include "IR/Function.hpp"
#include "IR/Glyph.hpp"

#include "Platform/Platform.hpp"

#include <sstream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::addFunc_Cmp*_*_W
   //
   void Info::addFunc_CmpF_EQ_W(Core::FastU n)
      {addFunc_CmpF_W(n, IR::Code::CmpF_EQ_W, IR::Code::CmpI_EQ_W, IR::Code::CmpI_EQ_W);}
   void Info::addFunc_CmpF_GE_W(Core::FastU n)
      {addFunc_CmpF_W(n, IR::Code::CmpF_GE_W, IR::Code::CmpI_GE_W, IR::Code::CmpI_LE_W);}
   void Info::addFunc_CmpF_GT_W(Core::FastU n)
      {addFunc_CmpF_W(n, IR::Code::CmpF_GT_W, IR::Code::CmpI_GT_W, IR::Code::CmpI_LT_W);}
   void Info::addFunc_CmpF_LE_W(Core::FastU n)
      {addFunc_CmpF_W(n, IR::Code::CmpF_LE_W, IR::Code::CmpI_LE_W, IR::Code::CmpI_GE_W);}
   void Info::addFunc_CmpF_LT_W(Core::FastU n)
      {addFunc_CmpF_W(n, IR::Code::CmpF_LT_W, IR::Code::CmpI_LT_W, IR::Code::CmpI_GT_W);}
   void Info::addFunc_CmpF_NE_W(Core::FastU n)
      {addFunc_CmpF_W(n, IR::Code::CmpF_NE_W, IR::Code::CmpI_NE_W, IR::Code::CmpI_NE_W);}
   void Info::addFunc_CmpI_GE_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpI_GE_W, IR::Code::CmpU_GE_W);}
   void Info::addFunc_CmpI_GT_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpI_GT_W, IR::Code::CmpU_GT_W);}
   void Info::addFunc_CmpI_LE_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpI_LE_W, IR::Code::CmpU_LE_W);}
   void Info::addFunc_CmpI_LT_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpI_LT_W, IR::Code::CmpU_LT_W);}
   void Info::addFunc_CmpU_EQ_W(Core::FastU n)
      {addFunc_CmpU_EQ_W(n, IR::Code::CmpU_EQ_W, IR::Code::BAnd_W);}
   void Info::addFunc_CmpU_GE_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpU_GE_W, IR::Code::CmpU_GE_W);}
   void Info::addFunc_CmpU_GT_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpU_GT_W, IR::Code::CmpU_GT_W);}
   void Info::addFunc_CmpU_LE_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpU_LE_W, IR::Code::CmpU_LE_W);}
   void Info::addFunc_CmpU_LT_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpU_LT_W, IR::Code::CmpU_LT_W);}
   void Info::addFunc_CmpU_NE_W(Core::FastU n)
      {addFunc_CmpU_EQ_W(n, IR::Code::CmpU_NE_W, IR::Code::BOrI_W);}

   //
   // Info::addFunc_CmpF_W
   //
   void Info::addFunc_CmpF_W(Core::FastU n, IR::Code code, IR::Code codePos, IR::Code codeNeg)
   {
      GDCC_BC_AddFuncPre({code, n}, 1, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n);

      GDCC_BC_AddFuncObjReg(lexp, n - 1);
      GDCC_BC_AddFuncObjReg(rexp, n * 2 - 1);

      FloatInfo fi = getFloatInfo(n);

      bool cmpEQ =
         codePos == IR::Code::CmpI_EQ_W ||
         codePos == IR::Code::CmpI_GE_W ||
         codePos == IR::Code::CmpI_LE_W;

      bool cmpGT =
         codePos == IR::Code::CmpI_GE_W ||
         codePos == IR::Code::CmpI_GT_W;

      bool cmpNE =
         codePos == IR::Code::CmpI_NE_W;

      IR::Glyph label0{prog, name + "$0"};
      IR::Glyph label1{prog, name + "$1"};

      IR::Glyph labelCmp{prog, name + "$cmp"};

      IR::Glyph labelLEMax{prog, name + "$lemax"};
      IR::Glyph labelLEMin{prog, name + "$lemin"};
      IR::Glyph labelREMax{prog, name + "$remax"};

      // Check for special cases.

      // Does l have min or max exp?
      GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, lexp, fi.maskExp);
      GDCC_BC_AddStmnt({Code::Jcnd_Tab, 1}, stk, fi.maskExp, labelLEMax, 0, labelLEMin);
      GDCC_BC_AddStmnt({Code::Move_W,   1}, nul, stk);

      // Does r have max exp?
      GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, rexp, fi.maskExp);
      GDCC_BC_AddStmnt({Code::Jcnd_Tab, 1}, stk, fi.maskExp, labelREMax);
      GDCC_BC_AddStmnt({Code::Move_W,   1}, nul, stk);

      GDCC_BC_AddLabel(labelCmp);

      if(codePos == codeNeg)
      {
         GDCC_BC_AddStmnt({codePos,    n}, stk, lop, rop);
         GDCC_BC_AddStmnt({Code::Retn, 1}, stk);
      }
      else
      {
         IR::Glyph labelL1{prog, name + "$l1"};

         GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, lexp, fi.maskSig);
         GDCC_BC_AddStmnt({Code::Jcnd_Tru, 1}, stk, labelL1);

         // l > 0

         // If r < 0, then l > r.
         GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, rexp, fi.maskSig);
         GDCC_BC_AddStmnt({Code::Jcnd_Tru, 1}, stk, cmpGT ? label1 : label0);

         GDCC_BC_AddStmnt({codePos,        n}, stk, lop, rop);
         GDCC_BC_AddStmnt({Code::Retn,     1}, stk);

         // l < 0
         GDCC_BC_AddLabel(labelL1);

         // If r > 0, then l < r.
         GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, rexp, fi.maskSig);
         GDCC_BC_AddStmnt({Code::Jcnd_Nil, 1}, stk, cmpGT ? label0 : label1);

         GDCC_BC_AddStmnt({codeNeg,        n}, stk, lop, rop);
         GDCC_BC_AddStmnt({Code::Retn,     1}, stk);
      }

      GDCC_BC_AddLabel(labelLEMax);

      // If l is NaN, then l != r.
      GDCC_BC_AddStmnt({Code::Move_W,   n}, stk, lop);
      GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru, n}, stk, cmpNE ? label1 : label0);

      // Check for r having max exponent...
      GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, rexp, fi.maskExp);
      GDCC_BC_AddStmnt({Code::Jcnd_Tab, 1}, stk, fi.maskExp, labelREMax);
      GDCC_BC_AddStmnt({Code::Move_W,   1}, nul, stk);

      // ... And if not, jump to normal compare.
      GDCC_BC_AddStmnt({Code::Jump,     0}, labelCmp);

      GDCC_BC_AddLabel(labelLEMin);

      // Check for r having max exponent...
      GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, rexp, fi.maskExp);
      GDCC_BC_AddStmnt({Code::Jcnd_Tab, 1}, stk, fi.maskExp, labelREMax);
      GDCC_BC_AddStmnt({Code::Move_W,   1}, nul, stk);

      // ... And if not, check for l being subnormal...
      GDCC_BC_AddStmnt({Code::Move_W,   n}, stk, lop);
      GDCC_BC_AddStmnt({Code::BAnd_W,   1},              stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru, n}, stk, labelCmp);

      // ... And if not, check for r being zero...
      GDCC_BC_AddStmnt({Code::Move_W,   n}, stk, rop);
      GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, stk, fi.maskMan | fi.maskExp);

      // ... And if it is, l == r, otherwise l != r.
      GDCC_BC_AddStmnt({Code::LNot,     n}, stk, stk);
      if(!cmpEQ) GDCC_BC_AddStmnt({Code::LNot, 1}, stk, stk);
      GDCC_BC_AddStmnt({Code::Retn,     1}, stk);

      GDCC_BC_AddLabel(labelREMax);
      GDCC_BC_AddStmnt({Code::Move_W,   1}, nul, stk);

      // If r is NaN, then l != r.
      GDCC_BC_AddStmnt({Code::Move_W,   n}, stk, rop);
      GDCC_BC_AddStmnt({Code::BAnd_W,   1}, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru, n}, stk, cmpNE ? label1 : label0);

      // Jump to normal compare.
      GDCC_BC_AddStmnt({Code::Jump,     0}, labelCmp);

      GDCC_BC_AddLabel(label0);
      GDCC_BC_AddStmnt({Code::Retn, 1}, 0);

      GDCC_BC_AddLabel(label1);
      GDCC_BC_AddStmnt({Code::Retn, 1}, 1);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_CmpU_EQ_W
   //
   void Info::addFunc_CmpU_EQ_W(Core::FastU n, IR::Code codeCmp, IR::Code codeAnd)
   {
      GDCC_BC_AddFuncPre({codeCmp, n}, 1, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n);

      // First words.
      GDCC_BC_AddStmnt({codeCmp, 1}, stk, lop, rop);

      // Mid words.
      for(Core::FastU i = n - 1; i--;)
      {
         GDCC_BC_AddStmnt({codeCmp, 1}, stk, ++lop, ++rop);
         GDCC_BC_AddStmnt({codeAnd, 1}, stk,   stk,   stk);
      }

      GDCC_BC_AddStmnt({Code::Retn, 1}, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_CmpU_GE_W
   //
   void Info::addFunc_CmpU_GE_W(Core::FastU n, IR::Code codeCmpHi, IR::Code codeCmpLo)
   {
      GDCC_BC_AddFuncPre({codeCmpHi, n}, 1, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n);

      lop += n - 1;
      rop += n - 1;

      // Generate labels.
      Core::Array<IR::Glyph> labelEq{n - 1};
      for(Core::FastU i = n - 1; i;)
      {
         std::ostringstream labelBuf; labelBuf << name << "$eq" << i;
         std::string label = labelBuf.str();
         labelEq[--i] = {prog, {label.data(), label.size()}};
      }

      // First words
      GDCC_BC_AddStmnt({Code::CmpU_EQ_W, 1}, stk, lop, rop);
      GDCC_BC_AddStmnt({Code::Jcnd_Tru,  1}, stk, labelEq[n - 2]);

      GDCC_BC_AddStmnt({codeCmpHi,  1}, stk, lop, rop);
      GDCC_BC_AddStmnt({Code::Retn, 1}, stk);

      // Middle words.
      for(Core::FastU i = n - 2; i;)
      {
         GDCC_BC_AddLabel(labelEq[i]);
         GDCC_BC_AddStmnt({Code::CmpU_EQ_W, 1}, stk, --lop, --rop);
         GDCC_BC_AddStmnt({Code::Jcnd_Tru,  1}, stk, labelEq[--i]);

         GDCC_BC_AddStmnt({codeCmpLo,  1}, stk, lop, rop);
         GDCC_BC_AddStmnt({Code::Retn, 1}, stk);
      }

      // Last words.
      GDCC_BC_AddLabel(labelEq[0]);
      GDCC_BC_AddStmnt({codeCmpLo,  1}, stk, --lop, --rop);
      GDCC_BC_AddStmnt({Code::Retn, 1}, stk);

      GDCC_BC_AddFuncEnd();

   }
}

// EOF

