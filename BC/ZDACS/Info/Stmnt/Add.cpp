//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Addition and subtraction statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "IR/Exception.hpp"
#include "IR/Program.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   GDCC_BC_CodeTypeSwitchFn(gen, Add, FIU)
   GDCC_BC_CodeTypeSwitchFn(gen, AddX, Ux)
   GDCC_BC_CodeTypeSwitchFn(gen, Sub, FIU)
   GDCC_BC_CodeTypeSwitchFn(gen, SubX, Ux)

   GDCC_BC_CodeTypeSwitchFn(pre, Add, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, AddX, Ux)
   GDCC_BC_CodeTypeSwitchFn(pre, Sub, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, SubX, Ux)

   GDCC_BC_CodeTypeSwitchFn(put, Add, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, AddX, Ux)
   GDCC_BC_CodeTypeSwitchFn(put, Sub, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, SubX, Ux)

   GDCC_BC_CodeTypeSwitchFn(tr, Add, FIU)
   GDCC_BC_CodeTypeSwitchFn(tr, Sub, FIU)

   //
   // Info::chkStmnt_AddX
   //
   void Info::chkStmnt_AddX()
   {
      if(getStmntSize() != 1)
         IR::ErrorCode(stmnt, "unsupported size");
   }

   //
   // Info::genStmnt_Add_U
   //
   void Info::genStmnt_Add_U()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n != 1)
         return genStmntCall(n);

      if(stmnt->args[0].a == IR::ArgBase::Stk)
         numChunkCODE += 4;
      else
         numChunkCODE += 8;
   }

   //
   // Info::genStmnt_AddX_U
   //
   void Info::genStmnt_AddX_U()
   {
      Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;
      Core::FastU lenAddXU1 =
         lenPushArg(stmnt->args[lop], 0) * 2 +
         lenPushArg(stmnt->args[rop], 0) + 16;

      // No carry.
      if(stmnt->args.size() == 3)
         numChunkCODE += lenAddXU1;

      // With carry.
      else
         numChunkCODE += lenAddXU1 * 2 + 56;
   }

   //
   // Info::genStmnt_SubX_U
   //
   void Info::genStmnt_SubX_U()
   {
      Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;
      Core::FastU lenSubXU1 =
         lenPushArg(stmnt->args[lop], 0) * 2 +
         lenPushArg(stmnt->args[rop], 0) + 20;

      // No carry.
      if(stmnt->args.size() == 3)
         numChunkCODE += lenSubXU1;

      // With carry.
      else
         numChunkCODE += lenSubXU1 * 2 + 64;
   }

   //
   // Info::preStmnt_AddX_U
   //
   void Info::preStmnt_AddX_U()
   {
      addFunc_Cmp_UW1(IR::CodeBase::CmpLT);
   }

   //
   // Info::preStmnt_SubX_U
   //
   void Info::preStmnt_SubX_U()
   {
      addFunc_Cmp_UW1(IR::CodeBase::CmpGT);
   }

   //
   // Info::putStmnt_Add_U
   //
   void Info::putStmnt_Add_U()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n != 1)
         return putStmntCall(getFuncName(IR::CodeBase::Add+'U', n), n);

      //
      // putReg
      //
      auto putReg = [this](IR::ArgPtr1 const &a, Code add, Code inc)
      {
         putCode(stmnt->args[2].a == IR::ArgBase::Lit ? inc : add);
         putWord(getWord(a.idx->aLit) + a.off);
      };

      switch(stmnt->args[0].a)
      {
      case IR::ArgBase::GblReg:
         putReg(stmnt->args[0].aGblReg, Code::AddU_GblReg, Code::IncU_GblReg);
         break;

      case IR::ArgBase::HubReg:
         putReg(stmnt->args[0].aHubReg, Code::AddU_HubReg, Code::IncU_HubReg);
         break;

      case IR::ArgBase::LocReg:
         putReg(stmnt->args[0].aLocReg, Code::AddU_LocReg, Code::IncU_LocReg);
         break;

      case IR::ArgBase::ModReg:
         putReg(stmnt->args[0].aModReg, Code::AddU_ModReg, Code::IncU_ModReg);
         break;

      default:
         putCode(Code::AddU);
         break;
      }
   }

   //
   // Info::putStmnt_AddX_U
   //
   void Info::putStmnt_AddX_U()
   {
      Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;

      //
      // putAddXU1
      //
      auto putAddXU1 = [&]()
      {
         putStmntPushArg(stmnt->args[lop], 0);
         putStmntPushArg(stmnt->args[rop], 0);
         putCode(Code::AddU);
         putCode(Code::Copy);
         putStmntPushArg(stmnt->args[lop], 0);
         putStmntCall(getFuncName(IR::CodeBase::CmpLT+'U', 1), 1);
      };

      // No carry.
      if(stmnt->args.size() == 3)
         putAddXU1();

      // With carry.
      else
      {
         Core::FastU lenAddXU1 =
            lenPushArg(stmnt->args[lop], 0) * 2 +
            lenPushArg(stmnt->args[rop], 0) + 16;

         Core::FastU lenCarry0 = lenAddXU1 +  8;
         Core::FastU lenCarry1 = lenAddXU1 + 40;

         putCode(Code::Jcnd_Tru, putPos + lenCarry0 + 8);

         putAddXU1();
         putCode(Code::Jump_Lit, putPos + lenCarry1 + 8);

         putAddXU1();
         putCode(Code::Drop_LocReg, func->localReg + 0);
         putCode(Code::Push_Lit,    1);
         putCode(Code::AddU);
         putCode(Code::Copy);
         putCode(Code::LNot);
         putCode(Code::Push_LocReg, func->localReg + 0);
         putCode(Code::BOrI);
      }
   }

   //
   // Info::putStmnt_Sub_U
   //
   void Info::putStmnt_Sub_U()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n != 1)
         return putStmntCall(getFuncName(IR::CodeBase::Sub+'U', n), n);

      //
      // putReg
      //
      auto putReg = [this](IR::ArgPtr1 const &a, Code sub, Code dec)
      {
         putCode(stmnt->args[2].a == IR::ArgBase::Lit ? dec : sub);
         putWord(getWord(a.idx->aLit) + a.off);
      };

      switch(stmnt->args[0].a)
      {
      case IR::ArgBase::GblReg:
         putReg(stmnt->args[0].aGblReg, Code::SubU_GblReg, Code::DecU_GblReg);
         break;

      case IR::ArgBase::HubReg:
         putReg(stmnt->args[0].aHubReg, Code::SubU_HubReg, Code::DecU_HubReg);
         break;

      case IR::ArgBase::LocReg:
         putReg(stmnt->args[0].aLocReg, Code::SubU_LocReg, Code::DecU_LocReg);
         break;

      case IR::ArgBase::ModReg:
         putReg(stmnt->args[0].aModReg, Code::SubU_ModReg, Code::DecU_ModReg);
         break;

      default:
         putCode(Code::SubU);
         break;
      }
   }

   //
   // Info::putStmnt_SubX_U
   //
   void Info::putStmnt_SubX_U()
   {
      Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;

      //
      // putSubXU1
      //
      auto putSubXU1 = [&]()
      {
         putStmntPushArg(stmnt->args[lop], 0);
         putStmntPushArg(stmnt->args[rop], 0);
         putCode(Code::SubU);
         putCode(Code::Copy);
         putStmntPushArg(stmnt->args[lop], 0);
         putStmntCall(getFuncName(IR::CodeBase::CmpGT+'U', 1), 1);
         putCode(Code::NegI);
      };

      // No carry.
      if(stmnt->args.size() == 3)
         putSubXU1();

      // With carry.
      else
      {
         Core::FastU lenSubXU1 =
            lenPushArg(stmnt->args[lop], 0) * 2 +
            lenPushArg(stmnt->args[rop], 0) + 20;

         Core::FastU lenCarry0 = lenSubXU1 +  8;
         Core::FastU lenCarry1 = lenSubXU1 + 48;

         putCode(Code::Jcnd_Tru, putPos + lenCarry0 + 8);

         putSubXU1();
         putCode(Code::Jump_Lit, putPos + lenCarry1 + 8);

         putSubXU1();
         putCode(Code::Drop_LocReg, func->localReg + 0);
         putCode(Code::Push_Lit,    1);
         putCode(Code::SubU);
         putCode(Code::Copy);
         putCode(Code::BNot);
         putCode(Code::LNot);
         putCode(Code::NegI);
         putCode(Code::Push_LocReg, func->localReg + 0);
         putCode(Code::BOrI);
      }
   }

   //
   // Info::trStmnt_Add_U
   //
   void Info::trStmnt_Add_U()
   {
      auto n = getStmntSize();

      if(n != 1)
         return trStmntStk3(false);

      if(stmnt->args[0] == stmnt->args[1]) switch(stmnt->args[0].a)
      {
      case IR::ArgBase::GblReg:
      case IR::ArgBase::HubReg:
      case IR::ArgBase::LocReg:
      case IR::ArgBase::ModReg:
         if(stmnt->args[2].a != IR::ArgBase::Lit ||
            !stmnt->args[2].aLit.value->isValue() ||
            getWord(stmnt->args[2].aLit) != 1)
         {
            moveArgStk_src(stmnt->args[2]);
         }

         return;

      default:
         break;
      }
      else if(stmnt->args[0] == stmnt->args[2]) switch(stmnt->args[0].a)
      {
      case IR::ArgBase::GblReg:
      case IR::ArgBase::HubReg:
      case IR::ArgBase::LocReg:
      case IR::ArgBase::ModReg:
         std::swap(stmnt->args[1], stmnt->args[2]);

         if(stmnt->args[2].a != IR::ArgBase::Lit ||
            !stmnt->args[2].aLit.value->isValue() ||
            getWord(stmnt->args[2].aLit) != 1)
         {
            moveArgStk_src(stmnt->args[2]);
         }

         return;

      default:
         break;
      }

      trStmntStk3(false);
   }

   //
   // Info::trStmnt_AddX
   //
   void Info::trStmnt_AddX()
   {
      moveArgStk_dst(stmnt->args[0]);

      if(stmnt->args.size() > 3)
      {
         func->setLocalTmp(1);
         moveArgStk_src(stmnt->args[1]);
      }
   }

   //
   // Info::trStmnt_Sub_U
   //
   void Info::trStmnt_Sub_U()
   {
      auto n = getStmntSize();

      if(n != 1)
         return trStmntStk3(true);

      if(stmnt->args[0] == stmnt->args[1]) switch(stmnt->args[0].a)
      {
      case IR::ArgBase::GblReg:
      case IR::ArgBase::HubReg:
      case IR::ArgBase::LocReg:
      case IR::ArgBase::ModReg:
         if(stmnt->args[2].a != IR::ArgBase::Lit ||
            !stmnt->args[2].aLit.value->isValue() ||
            getWord(stmnt->args[2].aLit) != 1)
         {
            moveArgStk_src(stmnt->args[2]);
         }

         return;

      default:
         break;
      }

      trStmntStk3(true);
   }

   //
   // Info::trStmnt_SubX
   //
   void Info::trStmnt_SubX()
   {
      moveArgStk_dst(stmnt->args[0]);

      if(stmnt->args.size() > 3)
      {
         func->setLocalTmp(1);
         moveArgStk_src(stmnt->args[1]);
      }
   }
}

// EOF

