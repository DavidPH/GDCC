//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
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
   GDCC_BC_CodeTypeSwitchFn(gen, CmpEQ, FIU)
   GDCC_BC_CodeTypeSwitchFn(gen, CmpGE, FIU)
   GDCC_BC_CodeTypeSwitchFn(gen, CmpGT, FIU)
   GDCC_BC_CodeTypeSwitchFn(gen, CmpLE, FIU)
   GDCC_BC_CodeTypeSwitchFn(gen, CmpLT, FIU)
   GDCC_BC_CodeTypeSwitchFn(gen, CmpNE, FIU)

   GDCC_BC_CodeTypeSwitchFn(pre, CmpEQ, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, CmpGE, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, CmpGT, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, CmpLE, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, CmpLT, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, CmpNE, FIU)

   GDCC_BC_CodeTypeSwitchFn(put, CmpEQ, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, CmpGE, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, CmpGT, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, CmpLE, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, CmpLT, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, CmpNE, FIU)

   GDCC_BC_CodeTypeSwitchFn(tr, CmpEQ, FIU)
   GDCC_BC_CodeTypeSwitchFn(tr, CmpNE, FIU)

   //
   // Info::addFunc_Cmp_UW1
   //
   void Info::addFunc_Cmp_UW1(IR::CodeBase base)
   {
      // Unsigned word compare logic:
      //
      // if(l & 0x80000000) {if(!(r & 0x80000000)) return +1;}
      // else               {if(  r & 0x80000000 ) return -1;}
      // return cmp((int)l, (int)r)

      GDCC_BC_AddFuncPre((base+U, 1), 1, 2, 2, __FILE__);
      GDCC_BC_AddFuncObjBin(1, 1);

      IR::Glyph labelPos{prog, name + "$pos"};
      IR::Glyph labelCmp{prog, name + "$cmp"};

      bool resGT = base == IR::CodeBase::CmpGE || base == IR::CodeBase::CmpGT;
      bool resLT = base == IR::CodeBase::CmpLE || base == IR::CodeBase::CmpLT;

      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, lop, 0x80000000);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelPos);

      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop, 0x80000000);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelCmp);
      GDCC_BC_AddStmnt(Code::Retn,     1, resGT);

      GDCC_BC_AddLabel(labelPos);
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, rop, 0x80000000);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelCmp);
      GDCC_BC_AddStmnt(Code::Retn,     1, resLT);

      GDCC_BC_AddLabel(labelCmp);
      GDCC_BC_AddStmnt(base+I,         1, stk, lop, rop);
      GDCC_BC_AddStmnt(Code::Retn,     1, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::genStmnt_CmpEQ_U
   //
   void Info::genStmnt_CmpEQ_U()
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
   // Info::preStmntStkCmpU
   //
   void Info::preStmntStkCmpU(AddFunc add)
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
         addFunc_Cmp_UW1(stmnt->code.base);
      else
         (this->*add)(n);
   }

   //
   // Info::putStmnt_CmpEQ_U
   //
   void Info::putStmnt_CmpEQ_U()
   {
      putStmnt_CmpEQ_U(Code::CmpU_EQ, Code::BAnd, IR::CodeBase::CmpEQ+'U');
   }

   //
   // Info::putStmnt_CmpEQ_U
   //
   void Info::putStmnt_CmpEQ_U(Code codeCmp, Code codeAnd, IR::Code codeCall)
   {
      auto n = getStmntSize();

      if(n == 0) return;

      if(stmnt->args[1].a != IR::ArgBase::Stk ||
         stmnt->args[1].a != IR::ArgBase::Stk)
      {
         putStmntPushArg(stmnt->args[1], 0);
         putStmntPushArg(stmnt->args[2], 0);
         putCode(codeCmp);

         for(Core::FastU i = 1; i != n; ++i)
         {
            putStmntPushArg(stmnt->args[1], i);
            putStmntPushArg(stmnt->args[2], i);
            putCode(codeCmp);
            putCode(codeAnd);
         }
      }
      else if(n == 1)
         putCode(codeCmp);
      else
         putStmntCall(getFuncName(codeCall, n), 1);
   }

   //
   // Info::putStmnt_CmpNE_U
   //
   void Info::putStmnt_CmpNE_U()
   {
      putStmnt_CmpEQ_U(Code::CmpU_NE, Code::BOrI, IR::CodeBase::CmpNE+'U');
   }

   //
   // Info::trStmnt_CmpEQ_U
   //
   void Info::trStmnt_CmpEQ_U()
   {
      if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
         moveArgStk_dst(stmnt->args[0]);
      else
         trStmntStkBin(false);
   }
}

// EOF

