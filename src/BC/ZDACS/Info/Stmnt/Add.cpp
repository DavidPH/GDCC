//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
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
   //
   // Info::chkStmnt_AdXU
   //
   void Info::chkStmnt_AdXU()
   {
      if(getStmntSize() != 1)
         IR::ErrorCode(stmnt, "unsupported size");
   }

   //
   // Info::genStmnt_AdXU
   //
   void Info::genStmnt_AdXU()
   {
      Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;
      Core::FastU lenAdXU1 =
         lenPushArg(stmnt->args[lop], 0) * 2 +
         lenPushArg(stmnt->args[rop], 0) + 16;

      // No carry.
      if(stmnt->args.size() == 3)
         numChunkCODE += lenAdXU1;

      // With carry.
      else
         numChunkCODE += lenAdXU1 * 2 + 56;
   }

   //
   // Info::genStmnt_AddU
   //
   void Info::genStmnt_AddU()
   {
      auto n = getStmntSize();

      if(n != 1)
      {
         genStmntCall(n);
         return;
      }

      if(stmnt->args[0].a == IR::ArgBase::Stk)
         numChunkCODE += 4;
      else
         numChunkCODE += 8;
   }

   //
   // Info::genStmnt_SuXU
   //
   void Info::genStmnt_SuXU()
   {
      Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;
      Core::FastU lenSuXU1 =
         lenPushArg(stmnt->args[lop], 0) * 2 +
         lenPushArg(stmnt->args[rop], 0) + 20;

      // No carry.
      if(stmnt->args.size() == 3)
         numChunkCODE += lenSuXU1;

      // With carry.
      else
         numChunkCODE += lenSuXU1 * 2 + 64;
   }

   //
   // Info::preStmnt_AdXU
   //
   void Info::preStmnt_AdXU()
   {
      preStmnt_CmpU1(IR::Code::CmpU_LT, IR::Code::CmpI_LT, false, true);
   }

   //
   // Info::preStmnt_AddF
   //
   void Info::preStmnt_AddF()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      addFunc_AddF_W(n);
   }

   //
   // Info::preStmnt_AddU
   //
   void Info::preStmnt_AddU()
   {
      auto n = getStmntSize();

      if(n <= 1)
         return;

      addFunc_AddU_W(n);
   }

   //
   // Info::preStmnt_SuXU
   //
   void Info::preStmnt_SuXU()
   {
      preStmnt_CmpU1(IR::Code::CmpU_GT, IR::Code::CmpI_GT, true, false);
   }

   //
   // Info::preStmnt_SubF
   //
   void Info::preStmnt_SubF()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      addFunc_SubF_W(n);
   }

   //
   // Info::preStmnt_SubU
   //
   void Info::preStmnt_SubU()
   {
      auto n = getStmntSize();

      if(n <= 1)
         return;

      addFunc_SubU_W(n);
   }

   //
   // Info::putStmnt_AdXU
   //
   void Info::putStmnt_AdXU()
   {
      Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;

      //
      // putAdXU1
      //
      auto putAdXU1 = [&]()
      {
         putStmntPushArg(stmnt->args[lop], 0);
         putStmntPushArg(stmnt->args[rop], 0);
         putCode(Code::AddU);
         putCode(Code::Copy);
         putStmntPushArg(stmnt->args[lop], 0);
         putStmntCall(getCallName(IR::Code::CmpU_LT, 1), 1);
      };

      // No carry.
      if(stmnt->args.size() == 3)
         putAdXU1();

      // With carry.
      else
      {
         Core::FastU lenAdXU1 =
            lenPushArg(stmnt->args[lop], 0) * 2 +
            lenPushArg(stmnt->args[rop], 0) + 16;

         Core::FastU lenCarry0 = lenAdXU1 +  8;
         Core::FastU lenCarry1 = lenAdXU1 + 40;

         putCode(Code::Jcnd_Tru, putPos + lenCarry0 + 8);

         putAdXU1();
         putCode(Code::Jump_Lit, putPos + lenCarry1 + 8);

         putAdXU1();
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
   // Info::putStmnt_AddU
   //
   void Info::putStmnt_AddU()
   {
      auto n = getStmntSize();

      if(n != 1)
      {
         putStmntCall(n);
         return;
      }

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
   // Info::putStmnt_SuXU
   //
   void Info::putStmnt_SuXU()
   {
      Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;

      //
      // putSuXU1
      //
      auto putSuXU1 = [&]()
      {
         putStmntPushArg(stmnt->args[lop], 0);
         putStmntPushArg(stmnt->args[rop], 0);
         putCode(Code::SubU);
         putCode(Code::Copy);
         putStmntPushArg(stmnt->args[lop], 0);
         putStmntCall(getCallName(IR::Code::CmpU_GT, 1), 1);
         putCode(Code::NegI);
      };

      // No carry.
      if(stmnt->args.size() == 3)
         putSuXU1();

      // With carry.
      else
      {
         Core::FastU lenSuXU1 =
            lenPushArg(stmnt->args[lop], 0) * 2 +
            lenPushArg(stmnt->args[rop], 0) + 20;

         Core::FastU lenCarry0 = lenSuXU1 +  8;
         Core::FastU lenCarry1 = lenSuXU1 + 48;

         putCode(Code::Jcnd_Tru, putPos + lenCarry0 + 8);

         putSuXU1();
         putCode(Code::Jump_Lit, putPos + lenCarry1 + 8);

         putSuXU1();
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
   // Info::putStmnt_SubU
   //
   void Info::putStmnt_SubU()
   {
      auto n = getStmntSize();

      if(n != 1)
      {
         putStmntCall(n);
         return;
      }

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
   // Info::trStmnt_AdXU
   //
   void Info::trStmnt_AdXU()
   {
      moveArgStk_dst(stmnt->args[0]);

      if(stmnt->args.size() > 3)
      {
         func->setLocalTmp(1);
         moveArgStk_src(stmnt->args[1]);
      }
   }

   //
   // Info::trStmnt_AddU
   //
   void Info::trStmnt_AddU()
   {
      auto n = getStmntSize();

      if(n != 1)
      {
         trStmntStk3(false);
         return;
      }

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
   // Info::trStmnt_SuXU
   //
   void Info::trStmnt_SuXU()
   {
      moveArgStk_dst(stmnt->args[0]);

      if(stmnt->args.size() > 3)
      {
         func->setLocalTmp(1);
         moveArgStk_src(stmnt->args[1]);
      }
   }

   //
   // Info::trStmnt_SubU
   //
   void Info::trStmnt_SubU()
   {
      auto n = getStmntSize();

      if(n != 1)
      {
         trStmntStk3(true);
         return;
      }

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
}

// EOF

