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

#include <sstream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::addFunc_Cmp*_*_W
   //
   void Info::addFunc_CmpI_GE_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpI_GE_W, IR::Code::CmpU_GE_W);}
   void Info::addFunc_CmpI_GT_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpI_GT_W, IR::Code::CmpU_GT_W);}
   void Info::addFunc_CmpI_LE_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpI_LE_W, IR::Code::CmpU_LE_W);}
   void Info::addFunc_CmpI_LT_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpI_LT_W, IR::Code::CmpU_LT_W);}
   void Info::addFunc_CmpU_EQ_W(Core::FastU n)
      {addFunc_CmpU_EQ_W(n, IR::Code::CmpU_EQ_W, IR::Code::AndU_W);}
   void Info::addFunc_CmpU_GE_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpU_GE_W, IR::Code::CmpU_GE_W);}
   void Info::addFunc_CmpU_GT_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpU_GT_W, IR::Code::CmpU_GT_W);}
   void Info::addFunc_CmpU_LE_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpU_LE_W, IR::Code::CmpU_LE_W);}
   void Info::addFunc_CmpU_LT_W(Core::FastU n)
      {addFunc_CmpU_GE_W(n, IR::Code::CmpU_LT_W, IR::Code::CmpU_LT_W);}
   void Info::addFunc_CmpU_NE_W(Core::FastU n)
      {addFunc_CmpU_EQ_W(n, IR::Code::CmpU_NE_W, IR::Code::OrIU_W);}

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
         newFunc->block.addLabel(labelEq[i]);
         GDCC_BC_AddStmnt({Code::CmpU_EQ_W, 1}, stk, --lop, --rop);
         GDCC_BC_AddStmnt({Code::Jcnd_Tru,  1}, stk, labelEq[--i]);

         GDCC_BC_AddStmnt({codeCmpLo,  1}, stk, lop, rop);
         GDCC_BC_AddStmnt({Code::Retn, 1}, stk);
      }

      // Last words.
      newFunc->block.addLabel(labelEq[0]);
      GDCC_BC_AddStmnt({codeCmpLo,  1}, stk, --lop, --rop);
      GDCC_BC_AddStmnt({Code::Retn, 1}, stk);

      GDCC_BC_AddFuncEnd();

   }
}

// EOF

