//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Division statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "BC/AddFunc.hpp"

#include "IR/Function.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::genStmnt_DiXI
   //
   void Info::genStmnt_DiXI()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
      {
         if(stmnt->args[1].a == IR::ArgBase::Stk &&
            stmnt->args[2].a == IR::ArgBase::Stk)
         {
            numChunkCODE += 56;
         }
         else
         {
            numChunkCODE += 8
               + lenPushArg(stmnt->args[1], 0, 1) * 2
               + lenPushArg(stmnt->args[2], 0, 1) * 2;
         }

         return;
      }

      genStmntCall(n * 2);
   }

   //
   // Info::genStmnt_DivI
   //
   void Info::genStmnt_DivI()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
      {
         numChunkCODE += 4;
         return;
      }

      genStmntCall(n * 2);

      numChunkCODE += n * 4;
   }

   //
   // Info::genStmnt_DivU
   //
   void Info::genStmnt_DivU()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      genStmntCall(n * 2);

      numChunkCODE += n * 4;
   }

   //
   // Info::genStmnt_DivX
   //
   void Info::genStmnt_DivX()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
      {
         numChunkCODE += 4;
         return;
      }

      genStmntCall(n);
   }

   //
   // Info::genStmnt_ModI
   //
   void Info::genStmnt_ModI()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
      {
         numChunkCODE += 4;
         return;
      }

      genStmntCall(n * 2);

      numChunkCODE += n * 20;
   }

   //
   // Info::genStmnt_ModU
   //
   void Info::genStmnt_ModU()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      genStmntCall(n * 2);

      numChunkCODE += n * 20;
   }

   //
   // Info::preStmnt_DiXI
   //
   void Info::preStmnt_DiXI()
   {
      auto n = getStmntSize();

      if(n <= 1)
         return;

      addFunc_DiXI_W(n);
   }

   //
   // Info::preStmnt_DiXU
   //
   void Info::preStmnt_DiXU()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
         return preStmnt_DiXU1();

      addFunc_DiXU_W(n);
   }

   //
   // Info::preStmnt_DiXU1
   //
   void Info::preStmnt_DiXU1()
   {
      GDCC_BC_AddFuncPre(Code::DiXU, 1, 2, 2, 3, __FILE__);
      GDCC_BC_AddFuncObjBin(1, 1);

      IR::Glyph labelL0     {prog, name + "$l0"};
      IR::Glyph labelL0R0   {prog, name + "$l0r0"};
      IR::Glyph labelL1R0   {prog, name + "$l1r0"};
      IR::Glyph labelL1R00  {prog, name + "$l1r00"};
      IR::Glyph labelL1R01_1{prog, name + "$l1r01_1"};
      IR::Glyph labelL1R01_r{prog, name + "$l1r01_r"};
      IR::Glyph labelL1R1_lt{prog, name + "$l1r1_lt"};

      GDCC_BC_AddFuncObjReg(tmp, 1, 2);

      GDCC_BC_AddStmnt(IR::Code::BAnd,     1, stk, lop, 0x80000000);
      GDCC_BC_AddStmnt(IR::Code::Jcnd_Nil, 1, stk, labelL0);

      // l has high bit set.
      GDCC_BC_AddStmnt(IR::Code::BAnd,     1, stk, rop, 0x80000000);
      GDCC_BC_AddStmnt(IR::Code::Jcnd_Nil, 1, stk, labelL1R0);

      // l and r have high bit set.
      // If l < r, result is {0, l}, otherwise result is {1, l - r}.
      GDCC_BC_AddStmnt(IR::Code::CmpI_LT,  1, stk, lop, rop);
      GDCC_BC_AddStmnt(IR::Code::Jcnd_Tru, 1, stk, labelL1R1_lt);

      GDCC_BC_AddStmnt(IR::Code::Move, 1, stk, 1);
      GDCC_BC_AddStmnt(IR::Code::SubU, 1, stk, lop, rop);
      GDCC_BC_AddStmnt(IR::Code::Retn, 2, stk);

      GDCC_BC_AddLabel(labelL1R1_lt);
      GDCC_BC_AddStmnt(IR::Code::Move, 1, stk, 1);
      GDCC_BC_AddStmnt(IR::Code::Move, 1, stk, lop);
      GDCC_BC_AddStmnt(IR::Code::Retn, 2, stk);

      // l has high bit set, r does not.
      GDCC_BC_AddLabel(labelL1R0);
      GDCC_BC_AddStmnt(IR::Code::BAnd,     1, stk, rop, 1);
      GDCC_BC_AddStmnt(IR::Code::Jcnd_Nil, 1, stk, labelL1R00);

      // r has low bit set.

      // Halve l and divide by r, then double result.
      // If the result of the division is 0, then return {1, l - r}.
      GDCC_BC_AddStmnt(IR::Code::ShRU,     1, stk, lop, 1);
      GDCC_BC_AddStmnt(IR::Code::DivI,     1, stk, stk, rop);
      GDCC_BC_AddStmnt(IR::Code::Jcnd_Tab, 1, stk, 0, labelL1R01_1);
      GDCC_BC_AddStmnt(IR::Code::ShLU,     1, tmp, stk, 1);

      // If l - res * r >= r, add one to result.
      GDCC_BC_AddStmnt(IR::Code::Move,     1, stk, lop);
      GDCC_BC_AddStmnt(IR::Code::MulU,     1, stk, rop, tmp);
      GDCC_BC_AddStmnt(IR::Code::SubU,     1, stk, stk, stk);
      GDCC_BC_AddStmnt(IR::Code::CmpI_GE,  1, stk, stk, rop);
      GDCC_BC_AddStmnt(IR::Code::Jcnd_Nil, 1, stk, labelL1R01_r);
      GDCC_BC_AddStmnt(IR::Code::AddU,     1, tmp, tmp, 1);

      GDCC_BC_AddLabel(labelL1R01_r);
      GDCC_BC_AddStmnt(IR::Code::Move, 1, stk, tmp);

      GDCC_BC_AddStmnt(IR::Code::Move, 1, stk, lop);
      GDCC_BC_AddStmnt(IR::Code::MulU, 1, stk, tmp, rop);
      GDCC_BC_AddStmnt(IR::Code::SubU, 1, stk, stk, stk);

      GDCC_BC_AddStmnt(IR::Code::Retn, 2, stk);

      GDCC_BC_AddLabel(labelL1R01_1);
      GDCC_BC_AddStmnt(IR::Code::Move, 1, stk, 1);
      GDCC_BC_AddStmnt(IR::Code::SubU, 1, stk, lop, rop);
      GDCC_BC_AddStmnt(IR::Code::Retn, 2, stk);

      // r has low bit unset.
      // Therefore, div result is (l >> 1) / (r >> 1).
      // Therefore, mod result is l - quot * r.
      GDCC_BC_AddLabel(labelL1R00);
      GDCC_BC_AddStmnt(IR::Code::ShRU, 1, stk, lop, 1);
      GDCC_BC_AddStmnt(IR::Code::ShRI, 1, stk, rop, 1);
      GDCC_BC_AddStmnt(IR::Code::DivI, 1, tmp, stk, stk);
      GDCC_BC_AddStmnt(IR::Code::Move, 1, stk, tmp);

      GDCC_BC_AddStmnt(IR::Code::Move, 1, stk, lop);
      GDCC_BC_AddStmnt(IR::Code::MulU, 1, stk, tmp, rop);
      GDCC_BC_AddStmnt(IR::Code::SubU, 1, stk, stk, stk);

      GDCC_BC_AddStmnt(IR::Code::Retn, 2, stk);

      // l does not have high bit set.
      GDCC_BC_AddLabel(labelL0);
      GDCC_BC_AddStmnt(IR::Code::BAnd,     1, stk, rop, 0x80000000);
      GDCC_BC_AddStmnt(IR::Code::Jcnd_Nil, 1, stk, labelL0R0);

      // l does not have high bit set, r does.
      // Therefore, l < r, result is {0, l}.
      GDCC_BC_AddStmnt(IR::Code::Move, 1, stk, 0);
      GDCC_BC_AddStmnt(IR::Code::Move, 1, stk, lop);
      GDCC_BC_AddStmnt(IR::Code::Retn, 2, stk);

      // l and r do not have high bit set.
      // Therefore, signed division will work.
      GDCC_BC_AddLabel(labelL0R0);
      GDCC_BC_AddStmnt(IR::Code::DivI, 1, stk, lop, rop);
      GDCC_BC_AddStmnt(IR::Code::ModI, 1, stk, lop, rop);
      GDCC_BC_AddStmnt(IR::Code::Retn, 2, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::preStmnt_DivA
   //
   void Info::preStmnt_DivA()
   {
      if(auto n = getStmntSize())
         addFunc_DivA_W(n);
   }

   //
   // Info::preStmnt_DivF
   //
   void Info::preStmnt_DivF()
   {
      if(auto n = getStmntSize())
         addFunc_DivF_W(n);
   }

   //
   // Info::preStmnt_DivR
   //
   void Info::preStmnt_DivR()
   {
      if(auto n = getStmntSize())
         addFunc_DivR_W(n);
   }

   //
   // Info::preStmnt_DivX
   //
   void Info::preStmnt_DivX(IR::Code code)
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      bool sign = code == IR::Code::DiXI;

      if(n == 1)
      {
         if(sign)
            return;
      }

      if(sign)
         addFunc_DivX_W(n);
      else
         addFunc_DivK_W(n);
   }

   //
   // Info::putStmnt_DiXI
   //
   void Info::putStmnt_DiXI()
   {
      auto n = getStmntSize();

      if(n != 1)
         return putStmntCall(getCallName(stmnt->code, n), n * 2);

      if(stmnt->args[1].a == IR::ArgBase::Stk &&
         stmnt->args[2].a == IR::ArgBase::Stk)
      {
         putCode(Code::Drop_LocReg, func->localReg + 1);
         putCode(Code::Drop_LocReg, func->localReg + 0);

         putCode(Code::Push_LocReg, func->localReg + 0);
         putCode(Code::Push_LocReg, func->localReg + 1);
         putCode(Code::DivI);

         putCode(Code::Push_LocReg, func->localReg + 0);
         putCode(Code::Push_LocReg, func->localReg + 1);
         putCode(Code::ModI);
      }
      else
      {
         putStmntPushArg(stmnt->args[1], 0);
         putStmntPushArg(stmnt->args[2], 0);
         putCode(Code::DivI);

         putStmntPushArg(stmnt->args[1], 0);
         putStmntPushArg(stmnt->args[2], 0);
         putCode(Code::ModI);
      }
   }

   //
   // Info::putStmnt_DivI
   //
   void Info::putStmnt_DivI()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
      {
         putCode(Code::DivI);
         return;
      }

      putStmntCall(getCallName(IR::Code::DiXI, n), n * 2);

      for(Core::FastU i = n; i--;)
         putCode(Code::Drop_Nul);
   }

   //
   // Info::putStmnt_DivU
   //
   void Info::putStmnt_DivU()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      putStmntCall(getCallName(IR::Code::DiXU, n), n * 2);

      for(Core::FastU i = n; i--;)
         putCode(Code::Drop_Nul);
   }

   //
   // Info::putStmnt_DivX
   //
   void Info::putStmnt_DivX()
   {
      auto n = getStmntSize();

      if(n == 1)
         putCode(Code::DivX);
      else
         putStmntCall(getCallName(stmnt->code, n), n);
   }

   //
   // Info::putStmnt_ModI
   //
   void Info::putStmnt_ModI()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
      {
         putCode(Code::ModI);
         return;
      }

      putStmntCall(getCallName(IR::Code::DiXI, n), n * 2);

      for(Core::FastU i = n; i--;)
         putCode(Code::Drop_LocReg, func->localReg + i);

      for(Core::FastU i = n; i--;)
         putCode(Code::Drop_Nul);

      for(Core::FastU i = 0; i != n; ++i)
         putCode(Code::Push_LocReg, func->localReg + i);
   }

   //
   // Info::putStmnt_ModU
   //
   void Info::putStmnt_ModU()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      putStmntCall(getCallName(IR::Code::DiXU, n), n * 2);

      for(Core::FastU i = n; i--;)
         putCode(Code::Drop_LocReg, func->localReg + i);

      for(Core::FastU i = n; i--;)
         putCode(Code::Drop_Nul);

      for(Core::FastU i = 0; i != n; ++i)
         putCode(Code::Push_LocReg, func->localReg + i);
   }

   //
   // Info::trStmnt_DiXI
   //
   void Info::trStmnt_DiXI()
   {
      auto n = getStmntSize();

      if(n != 1)
      {
         trStmntStk3(true);
         return;
      }

      if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
      {
         moveArgStk_dst(stmnt->args[0]);
      }
      else
      {
         func->setLocalTmp(n * 2);
         trStmntStk3(true);
      }
   }

   //
   // Info::trStmnt_DiXU
   //
   void Info::trStmnt_DiXU()
   {
      trStmntStk3(true);
   }

   //
   // Info::trStmnt_ModI
   //
   void Info::trStmnt_ModI()
   {
      auto n = getStmntSize();

      if(n != 1)
         func->setLocalTmp(n);

      trStmntStk3(true);
   }

   //
   // Info::trStmnt_ModU
   //
   void Info::trStmnt_ModU()
   {
      auto n = getStmntSize();

      func->setLocalTmp(n);
      trStmntStk3(true);
   }
}

// EOF

