//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Compare statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "BC/AddFunc.hpp"

#include "Core/Exception.hpp"

#include "IR/Function.hpp"
#include "IR/Glyph.hpp"

#include <sstream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::genStmnt_CmpI_W
   //
   void Info::genStmnt_CmpI_W()
   {
      auto n = getStmntSizeW();

      if(n == 0) return;

      if(n == 1)
         numChunkCODE += 4;
      else
         genStmntCall(1);
   }

   //
   // Info::genStmnt_CmpU_EQ_W
   //
   void Info::genStmnt_CmpU_EQ_W()
   {
      auto n = getStmntSizeW();

      if(n == 0) return;

      if(stmnt->args[1].a != IR::ArgBase::Stk ||
         stmnt->args[1].a != IR::ArgBase::Stk)
      {
         numChunkCODE += n * 8 - 4 +
            lenPushArg(stmnt->args[1], 0, n) +
            lenPushArg(stmnt->args[2], 0, n);
      }
      else if(n == 1)
         numChunkCODE += 4;
      else
         genStmntCall(1);
   }

   //
   // Info::preStmnt_CmpF_EQ_W
   //
   void Info::preStmnt_CmpF_EQ_W()
   {
      addFunc_CmpF_EQ_W(getStmntSizeW());
   }

   //
   // Info::preStmnt_CmpF_GE_W
   //
   void Info::preStmnt_CmpF_GE_W()
   {
      addFunc_CmpF_GE_W(getStmntSizeW());
   }

   //
   // Info::preStmnt_CmpF_GT_W
   //
   void Info::preStmnt_CmpF_GT_W()
   {
      addFunc_CmpF_GT_W(getStmntSizeW());
   }

   //
   // Info::preStmnt_CmpF_LE_W
   //
   void Info::preStmnt_CmpF_LE_W()
   {
      addFunc_CmpF_LE_W(getStmntSizeW());
   }

   //
   // Info::preStmnt_CmpF_LT_W
   //
   void Info::preStmnt_CmpF_LT_W()
   {
      addFunc_CmpF_LT_W(getStmntSizeW());
   }

   //
   // Info::preStmnt_CmpF_NE_W
   //
   void Info::preStmnt_CmpF_NE_W()
   {
      addFunc_CmpF_NE_W(getStmntSizeW());
   }

   //
   // Info::preStmnt_CmpI_GE_W
   //
   void Info::preStmnt_CmpI_GE_W()
   {
      auto n = getStmntSizeW();

      if(n < 2)
         return;

      addFunc_CmpI_GE_W(n);
   }

   //
   // Info::preStmnt_CmpI_GT_W
   //
   void Info::preStmnt_CmpI_GT_W()
   {
      auto n = getStmntSizeW();

      if(n < 2)
         return;

      addFunc_CmpI_GT_W(n);
   }

   //
   // Info::preStmnt_CmpI_LE_W
   //
   void Info::preStmnt_CmpI_LE_W()
   {
      auto n = getStmntSizeW();

      if(n < 2)
         return;

      addFunc_CmpI_LE_W(n);
   }

   //
   // Info::preStmnt_CmpI_LT_W
   //
   void Info::preStmnt_CmpI_LT_W()
   {
      auto n = getStmntSizeW();

      if(n < 2)
         return;

      addFunc_CmpI_LT_W(n);
   }

   //
   // Info::preStmnt_CmpU_EQ_W
   //
   void Info::preStmnt_CmpU_EQ_W(IR::Code codeCmp)
   {
      auto n = getStmntSizeW();

      if(n < 2)
         return;

      if(codeCmp == IR::Code::CmpU_EQ_W)
         addFunc_CmpU_EQ_W(n);
      else
         addFunc_CmpU_NE_W(n);
   }

   //
   // Info::preStmnt_CmpU_GE_W
   //
   void Info::preStmnt_CmpU_GE_W()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         preStmnt_CmpU_W1(IR::Code::CmpU_GE_W, IR::Code::CmpI_GE_W, true, false);
      else
         addFunc_CmpU_GE_W(n);
   }

   //
   // Info::preStmnt_CmpU_GT_W
   //
   void Info::preStmnt_CmpU_GT_W()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         preStmnt_CmpU_W1(IR::Code::CmpU_GT_W, IR::Code::CmpI_GT_W, true, false);
      else
         addFunc_CmpU_GT_W(n);
   }

   //
   // Info::preStmnt_CmpU_LE_W
   //
   void Info::preStmnt_CmpU_LE_W()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         preStmnt_CmpU_W1(IR::Code::CmpU_LE_W, IR::Code::CmpI_LE_W, false, true);
      else
         addFunc_CmpU_LE_W(n);
   }

   //
   // Info::preStmnt_CmpU_LT_W
   //
   void Info::preStmnt_CmpU_LT_W()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         preStmnt_CmpU_W1(IR::Code::CmpU_LT_W, IR::Code::CmpI_LT_W, false, true);
      else
         addFunc_CmpU_LT_W(n);
   }

   //
   // Info::preStmnt_CmpU_W1
   //
   void Info::preStmnt_CmpU_W1(IR::Code code, IR::Code codeCmp, bool resGT, bool resLT)
   {
      // Unsigned word compare logic:
      //
      // if(l & 0x80000000) {if(!(r & 0x80000000)) return +1;}
      // else               {if(  r & 0x80000000 ) return -1;}
      // return cmp((int)l, (int)r)

      GDCC_BC_AddFuncPre(code, 1, 1, 2, 2, __FILE__);
      GDCC_BC_AddFuncObjBin(1, 1);

      IR::Glyph labelPos{prog, name + "$pos"};
      IR::Glyph labelCmp{prog, name + "$cmp"};

      GDCC_BC_AddStmnt(IR::Code::BAnd_W,   1, stk, lop, 0x80000000);
      GDCC_BC_AddStmnt(IR::Code::Jcnd_Nil, 1, stk, labelPos);

      GDCC_BC_AddStmnt(IR::Code::BAnd_W,   1, stk, rop, 0x80000000);
      GDCC_BC_AddStmnt(IR::Code::Jcnd_Tru, 1, stk, labelCmp);
      GDCC_BC_AddStmnt(IR::Code::Retn,     1, resGT);

      GDCC_BC_AddLabel(labelPos);
      GDCC_BC_AddStmnt(IR::Code::BAnd_W,   1, stk, rop, 0x80000000);
      GDCC_BC_AddStmnt(IR::Code::Jcnd_Nil, 1, stk, labelCmp);
      GDCC_BC_AddStmnt(IR::Code::Retn,     1, resLT);

      GDCC_BC_AddLabel(labelCmp);
      GDCC_BC_AddStmnt(codeCmp,        1, stk, lop, rop);
      GDCC_BC_AddStmnt(IR::Code::Retn, 1, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::putStmnt_CmpI_W
   //
   void Info::putStmnt_CmpI_W(Code code)
   {
      auto n = getStmntSizeW();

      if(n == 0) return;

      if(n == 1)
         putCode(code);
      else
         putStmntCall(1);
   }

   //
   // Info::putStmnt_CmpU_EQ_W
   //
   void Info::putStmnt_CmpU_EQ_W()
   {
      auto n = getStmntSizeW();

      if(n == 0) return;

      if(stmnt->args[1].a != IR::ArgBase::Stk ||
         stmnt->args[1].a != IR::ArgBase::Stk)
      {
         putStmntPushArg(stmnt->args[1], 0);
         putStmntPushArg(stmnt->args[2], 0);
         putCode(Code::CmpU_EQ);

         for(Core::FastU i = 1; i != n; ++i)
         {
            putStmntPushArg(stmnt->args[1], i);
            putStmntPushArg(stmnt->args[2], i);
            putCode(Code::CmpU_EQ);
            putCode(Code::BAnd);
         }
      }
      else if(n == 1)
         putCode(Code::CmpU_EQ);
      else
         putStmntCall(1);
   }

   //
   // Info::putStmnt_CmpU_NE_W
   //
   void Info::putStmnt_CmpU_NE_W()
   {
      auto n = getStmntSizeW();

      if(n == 0) return;

      if(stmnt->args[1].a != IR::ArgBase::Stk ||
         stmnt->args[1].a != IR::ArgBase::Stk)
      {
         putStmntPushArg(stmnt->args[1], 0);
         putStmntPushArg(stmnt->args[2], 0);
         putCode(Code::CmpU_NE);

         for(Core::FastU i = 1; i != n; ++i)
         {
            putStmntPushArg(stmnt->args[1], i);
            putStmntPushArg(stmnt->args[2], i);
            putCode(Code::CmpU_NE);
            putCode(Code::BOrI);
         }
      }
      else if(n == 1)
         putCode(Code::CmpU_NE);
      else
         putStmntCall(1);
   }

   //
   // Info::trStmnt_CmpU_EQ_W
   //
   void Info::trStmnt_CmpU_EQ_W()
   {
      CheckArgC(stmnt, 3);

      if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
         moveArgStk_dst(stmnt->args[0]);
      else
         trStmntStk3(false);
   }
}

// EOF

