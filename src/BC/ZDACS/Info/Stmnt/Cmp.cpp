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
   // Info::genStmnt_CmpI
   //
   void Info::genStmnt_CmpI()
   {
      auto n = getStmntSize();

      if(n == 0) return;

      if(n == 1)
         numChunkCODE += 4;
      else
         genStmntCall(1);
   }

   //
   // Info::genStmnt_CmpU_EQ
   //
   void Info::genStmnt_CmpU_EQ()
   {
      auto n = getStmntSize();

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
   // Info::preStmnt_CmpF_EQ
   //
   void Info::preStmnt_CmpF_EQ()
   {
      addFunc_CmpF_EQ_W(getStmntSize());
   }

   //
   // Info::preStmnt_CmpF_GE
   //
   void Info::preStmnt_CmpF_GE()
   {
      addFunc_CmpF_GE_W(getStmntSize());
   }

   //
   // Info::preStmnt_CmpF_GT
   //
   void Info::preStmnt_CmpF_GT()
   {
      addFunc_CmpF_GT_W(getStmntSize());
   }

   //
   // Info::preStmnt_CmpF_LE
   //
   void Info::preStmnt_CmpF_LE()
   {
      addFunc_CmpF_LE_W(getStmntSize());
   }

   //
   // Info::preStmnt_CmpF_LT
   //
   void Info::preStmnt_CmpF_LT()
   {
      addFunc_CmpF_LT_W(getStmntSize());
   }

   //
   // Info::preStmnt_CmpF_NE
   //
   void Info::preStmnt_CmpF_NE()
   {
      addFunc_CmpF_NE_W(getStmntSize());
   }

   //
   // Info::preStmnt_CmpI_GE
   //
   void Info::preStmnt_CmpI_GE()
   {
      auto n = getStmntSize();

      if(n < 2)
         return;

      addFunc_CmpI_GE_W(n);
   }

   //
   // Info::preStmnt_CmpI_GT
   //
   void Info::preStmnt_CmpI_GT()
   {
      auto n = getStmntSize();

      if(n < 2)
         return;

      addFunc_CmpI_GT_W(n);
   }

   //
   // Info::preStmnt_CmpI_LE
   //
   void Info::preStmnt_CmpI_LE()
   {
      auto n = getStmntSize();

      if(n < 2)
         return;

      addFunc_CmpI_LE_W(n);
   }

   //
   // Info::preStmnt_CmpI_LT
   //
   void Info::preStmnt_CmpI_LT()
   {
      auto n = getStmntSize();

      if(n < 2)
         return;

      addFunc_CmpI_LT_W(n);
   }

   //
   // Info::preStmnt_CmpU_EQ
   //
   void Info::preStmnt_CmpU_EQ(IR::Code codeCmp)
   {
      auto n = getStmntSize();

      if(n < 2)
         return;

      if(codeCmp == IR::Code::CmpU_EQ)
         addFunc_CmpU_EQ_W(n);
      else
         addFunc_CmpU_NE_W(n);
   }

   //
   // Info::preStmnt_CmpU_GE
   //
   void Info::preStmnt_CmpU_GE()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
         preStmnt_CmpU1(IR::Code::CmpU_GE, IR::Code::CmpI_GE, true, false);
      else
         addFunc_CmpU_GE_W(n);
   }

   //
   // Info::preStmnt_CmpU_GT
   //
   void Info::preStmnt_CmpU_GT()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
         preStmnt_CmpU1(IR::Code::CmpU_GT, IR::Code::CmpI_GT, true, false);
      else
         addFunc_CmpU_GT_W(n);
   }

   //
   // Info::preStmnt_CmpU_LE
   //
   void Info::preStmnt_CmpU_LE()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
         preStmnt_CmpU1(IR::Code::CmpU_LE, IR::Code::CmpI_LE, false, true);
      else
         addFunc_CmpU_LE_W(n);
   }

   //
   // Info::preStmnt_CmpU_LT
   //
   void Info::preStmnt_CmpU_LT()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
         preStmnt_CmpU1(IR::Code::CmpU_LT, IR::Code::CmpI_LT, false, true);
      else
         addFunc_CmpU_LT_W(n);
   }

   //
   // Info::preStmnt_CmpU1
   //
   void Info::preStmnt_CmpU1(IR::Code code, IR::Code codeCmp, bool resGT, bool resLT)
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

      GDCC_BC_AddStmnt(IR::Code::BAnd,     1, stk, lop, 0x80000000);
      GDCC_BC_AddStmnt(IR::Code::Jcnd_Nil, 1, stk, labelPos);

      GDCC_BC_AddStmnt(IR::Code::BAnd,     1, stk, rop, 0x80000000);
      GDCC_BC_AddStmnt(IR::Code::Jcnd_Tru, 1, stk, labelCmp);
      GDCC_BC_AddStmnt(IR::Code::Retn,     1, resGT);

      GDCC_BC_AddLabel(labelPos);
      GDCC_BC_AddStmnt(IR::Code::BAnd,     1, stk, rop, 0x80000000);
      GDCC_BC_AddStmnt(IR::Code::Jcnd_Nil, 1, stk, labelCmp);
      GDCC_BC_AddStmnt(IR::Code::Retn,     1, resLT);

      GDCC_BC_AddLabel(labelCmp);
      GDCC_BC_AddStmnt(codeCmp,        1, stk, lop, rop);
      GDCC_BC_AddStmnt(IR::Code::Retn, 1, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::putStmnt_CmpI
   //
   void Info::putStmnt_CmpI(Code code)
   {
      auto n = getStmntSize();

      if(n == 0) return;

      if(n == 1)
         putCode(code);
      else
         putStmntCall(1);
   }

   //
   // Info::putStmnt_CmpU_EQ
   //
   void Info::putStmnt_CmpU_EQ()
   {
      auto n = getStmntSize();

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
   // Info::putStmnt_CmpU_NE
   //
   void Info::putStmnt_CmpU_NE()
   {
      auto n = getStmntSize();

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
   // Info::trStmnt_CmpU_EQ
   //
   void Info::trStmnt_CmpU_EQ()
   {
      if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
         moveArgStk_dst(stmnt->args[0]);
      else
         trStmntStk3(false);
   }
}

// EOF

