//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
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

#include <sstream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::addFunc_Cmp*_*
   //
   void Info::addFunc_CmpEQ_FW(Core::FastU n)
      {addFunc_Cmp_FW(n, IR::CodeBase::CmpEQ+'F', IR::CodeBase::CmpEQ+'I', IR::CodeBase::CmpEQ+'I');}
   void Info::addFunc_CmpGE_FW(Core::FastU n)
      {addFunc_Cmp_FW(n, IR::CodeBase::CmpGE+'F', IR::CodeBase::CmpGE+'I', IR::CodeBase::CmpLE+'I');}
   void Info::addFunc_CmpGT_FW(Core::FastU n)
      {addFunc_Cmp_FW(n, IR::CodeBase::CmpGT+'F', IR::CodeBase::CmpGT+'I', IR::CodeBase::CmpLT+'I');}
   void Info::addFunc_CmpLE_FW(Core::FastU n)
      {addFunc_Cmp_FW(n, IR::CodeBase::CmpLE+'F', IR::CodeBase::CmpLE+'I', IR::CodeBase::CmpGE+'I');}
   void Info::addFunc_CmpLT_FW(Core::FastU n)
      {addFunc_Cmp_FW(n, IR::CodeBase::CmpLT+'F', IR::CodeBase::CmpLT+'I', IR::CodeBase::CmpGT+'I');}
   void Info::addFunc_CmpNE_FW(Core::FastU n)
      {addFunc_Cmp_FW(n, IR::CodeBase::CmpNE+'F', IR::CodeBase::CmpNE+'I', IR::CodeBase::CmpNE+'I');}
   void Info::addFunc_CmpGE_IW(Core::FastU n)
      {addFunc_CmpGE_UW(n, IR::CodeBase::CmpGE+'I', IR::CodeBase::CmpGE+'U');}
   void Info::addFunc_CmpGT_IW(Core::FastU n)
      {addFunc_CmpGE_UW(n, IR::CodeBase::CmpGT+'I', IR::CodeBase::CmpGT+'U');}
   void Info::addFunc_CmpLE_IW(Core::FastU n)
      {addFunc_CmpGE_UW(n, IR::CodeBase::CmpLE+'I', IR::CodeBase::CmpLE+'U');}
   void Info::addFunc_CmpLT_IW(Core::FastU n)
      {addFunc_CmpGE_UW(n, IR::CodeBase::CmpLT+'I', IR::CodeBase::CmpLT+'U');}
   void Info::addFunc_CmpEQ_UW(Core::FastU n)
      {addFunc_CmpEQ_UW(n, IR::CodeBase::CmpEQ+'U', IR::CodeBase::BAnd);}
   void Info::addFunc_CmpGE_UW(Core::FastU n)
      {addFunc_CmpGE_UW(n, IR::CodeBase::CmpGE+'U', IR::CodeBase::CmpGE+'U');}
   void Info::addFunc_CmpGT_UW(Core::FastU n)
      {addFunc_CmpGE_UW(n, IR::CodeBase::CmpGT+'U', IR::CodeBase::CmpGT+'U');}
   void Info::addFunc_CmpLE_UW(Core::FastU n)
      {addFunc_CmpGE_UW(n, IR::CodeBase::CmpLE+'U', IR::CodeBase::CmpLE+'U');}
   void Info::addFunc_CmpLT_UW(Core::FastU n)
      {addFunc_CmpGE_UW(n, IR::CodeBase::CmpLT+'U', IR::CodeBase::CmpLT+'U');}
   void Info::addFunc_CmpNE_UW(Core::FastU n)
      {addFunc_CmpEQ_UW(n, IR::CodeBase::CmpNE+'U', IR::CodeBase::BOrI);}

   //
   // Info::addFunc_Cmp_FW
   //
   void Info::addFunc_Cmp_FW(Core::FastU n, IR::Code code, IR::Code codePos, IR::Code codeNeg)
   {
      GDCC_BC_AddFuncPre((code, n), 1, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      FloatInfo fi = getFloatInfo(n, true);

      bool cmpGT =
         codePos == IR::CodeBase::CmpGE+'I' ||
         codePos == IR::CodeBase::CmpGT+'I';

      bool cmpNE =
         codePos == IR::CodeBase::CmpNE+'I';

      IR::Glyph label0{prog, name + "$0"};
      IR::Glyph label1{prog, name + "$1"};

      IR::Glyph labelCmp{prog, name + "$cmp"};

      IR::Glyph labelLEMax{prog, name + "$lemax"};
      IR::Glyph labelLEMin{prog, name + "$lemin"};
      IR::Glyph labelREMax{prog, name + "$remax"};

      // Check for special cases.

      // Does l have min or max exp?
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, lop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, fi.maskExp, labelLEMax, 0, labelLEMin);
      GDCC_BC_AddStmnt(Code::Move,     1, nul, stk);

      // Does r have max exp?
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, fi.maskExp, labelREMax);
      GDCC_BC_AddStmnt(Code::Move,     1, nul, stk);

      GDCC_BC_AddLabel(labelCmp);

      if(codePos == codeNeg)
      {
         GDCC_BC_AddStmnt(codePos,    1, stk, lop, rop);
         GDCC_BC_AddStmnt(Code::Retn, 1, stk);
      }
      else
      {
         IR::Glyph labelL1{prog, name + "$l1"};

         GDCC_BC_AddStmnt(Code::BAnd,     1, stk, lop.hi, fi.maskSig);
         GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelL1);

         // l > 0

         // If r < 0, then l > r.
         GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, fi.maskSig);
         GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, cmpGT ? label1 : label0);

         GDCC_BC_AddStmnt(codePos,        1, stk, lop, rop);
         GDCC_BC_AddStmnt(Code::Retn,     1, stk);

         // l < 0
         GDCC_BC_AddLabel(labelL1);

         // If r > 0, then l < r.
         GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, fi.maskSig);
         GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, cmpGT ? label0 : label1);

         GDCC_BC_AddStmnt(codeNeg,        1, stk, lop, rop);
         GDCC_BC_AddStmnt(Code::Retn,     1, stk);
      }

      GDCC_BC_AddLabel(labelLEMax);

      // If l is NaN, then l != r.
      GDCC_BC_AddStmnt(Code::Move,     n, stk, lop);
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, cmpNE ? label1 : label0);

      // Check for r having max exponent...
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, fi.maskExp, labelREMax);
      GDCC_BC_AddStmnt(Code::Move,     1, nul, stk);

      // ... And if not, jump to normal compare.
      GDCC_BC_AddStmnt(Code::Jump,     1, labelCmp);

      GDCC_BC_AddLabel(labelLEMin);

      // Check for r having max exponent...
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop.hi, fi.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, fi.maskExp, labelREMax);
      GDCC_BC_AddStmnt(Code::Move,     1, nul, stk);

      // ... And if not, jump to normal compare.
      GDCC_BC_AddStmnt(Code::Jump,     1, labelCmp);

      GDCC_BC_AddLabel(labelREMax);
      GDCC_BC_AddStmnt(Code::Move,     1, nul, stk);

      // If r is NaN, then l != r.
      GDCC_BC_AddStmnt(Code::Move,     n, stk, rop);
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, stk, fi.maskMan);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, n, stk, cmpNE ? label1 : label0);

      // Jump to normal compare.
      GDCC_BC_AddStmnt(Code::Jump,     1, labelCmp);

      GDCC_BC_AddLabel(label0);
      GDCC_BC_AddStmnt(Code::Retn, 1, 0);

      GDCC_BC_AddLabel(label1);
      GDCC_BC_AddStmnt(Code::Retn, 1, 1);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_CmpEQ_UW
   //
   void Info::addFunc_CmpEQ_UW(Core::FastU n, IR::Code codeCmp, IR::Code codeAnd)
   {
      GDCC_BC_AddFuncPre((codeCmp, n), 1, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      // First words.
      GDCC_BC_AddStmnt(codeCmp, 1, stk, lop.lo, rop.lo);

      // Mid words.
      for(Core::FastU i = 1; i != n; ++i)
      {
         GDCC_BC_AddStmnt(codeCmp, 1, stk, lop[i], rop[i]);
         GDCC_BC_AddStmnt(codeAnd, 1, stk, stk,    stk);
      }

      GDCC_BC_AddStmnt(Code::Retn, 1, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_CmpGE_UW
   //
   void Info::addFunc_CmpGE_UW(Core::FastU n, IR::Code codeCmpHi, IR::Code codeCmpLo)
   {
      GDCC_BC_AddFuncPre((codeCmpHi, n), 1, n * 2, n * 2, __FILE__);
      GDCC_BC_AddFuncObjBin(n, n);

      // Generate labels.
      Core::Array<IR::Glyph> labelEq{n - 1};
      for(Core::FastU i = n - 1; i;)
      {
         std::ostringstream labelBuf; labelBuf << name << "$eq" << i;
         std::string label = labelBuf.str();
         labelEq[--i] = {prog, {label.data(), label.size()}};
      }

      // First words
      GDCC_BC_AddStmnt(Code::CmpEQ+U,  1, stk, lop.hi, rop.hi);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelEq[n - 2]);

      GDCC_BC_AddStmnt(codeCmpHi,  1, stk, lop.hi, rop.hi);
      GDCC_BC_AddStmnt(Code::Retn, 1, stk);

      // Middle words.
      for(Core::FastU i = n - 2; i; --i)
      {
         GDCC_BC_AddLabel(labelEq[i]);
         GDCC_BC_AddStmnt(Code::CmpEQ+U,  1, stk, lop[i], rop[i]);
         GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelEq[i - 1]);

         GDCC_BC_AddStmnt(codeCmpLo,  1, stk, lop[i], rop[i]);
         GDCC_BC_AddStmnt(Code::Retn, 1, stk);
      }

      // Last words.
      GDCC_BC_AddLabel(labelEq[0]);
      GDCC_BC_AddStmnt(codeCmpLo,  1, stk, lop.lo, rop.lo);
      GDCC_BC_AddStmnt(Code::Retn, 1, stk);

      GDCC_BC_AddFuncEnd();

   }
}

// EOF

