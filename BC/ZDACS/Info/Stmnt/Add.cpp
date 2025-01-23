//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
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
#include "BC/ZDACS/Module.hpp"

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

      if(n != 1 || stmnt->args[0] != stmnt->args[1])
         return genStmntArg('U', Code::AddU);

      if(isIncUArg(stmnt->args[0]) &&
         stmnt->args[2].a == IR::ArgBase::Lit &&
         stmnt->args[2].aLit.value->isValue() &&
         getWord(stmnt->args[2].aLit) == 1)
      {
         return genStmntIncUArg(stmnt->args[0], 0);
      }

      //
      // genReg
      //
      auto genReg = [this](IR::ArgPtr1 const &a, Code code)
      {
         if(stmnt->args[2].a != IR::ArgBase::Stk)
            genStmntPushArg(stmnt->args[2]);

         genCode(code, getExpAddPtr(a.idx->aLit.value, a.off));
      };

      switch(stmnt->args[0].a)
      {
      case IR::ArgBase::GblReg:
         return genReg(stmnt->args[0].aGblReg, Code::AddU_GblReg);

      case IR::ArgBase::HubReg:
         return genReg(stmnt->args[0].aHubReg, Code::AddU_HubReg);

      case IR::ArgBase::LocReg:
         return genReg(stmnt->args[0].aLocReg, Code::AddU_LocReg);

      case IR::ArgBase::ModReg:
         return genReg(stmnt->args[0].aModReg, Code::AddU_ModReg);

      default:
         break;
      }

      genStmntArg('U', Code::AddU);
   }

   //
   // Info::genStmnt_AddX_U
   //
   void Info::genStmnt_AddX_U()
   {
      Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;

      //
      // genAddXU1
      //
      auto genAddXU1 = [&]()
      {
         genStmntPushArg(stmnt->args[lop], 0);
         genStmntPushArg(stmnt->args[rop], 0);
         genCode(Code::AddU);
         genCode(Code::Copy);
         genStmntPushArg(stmnt->args[lop], 0);
         genStmntCall(getFuncName(IR::CodeBase::CmpLT+'U', 1), 1);
      };

      // No carry.
      if(stmnt->args.size() == 3)
         genAddXU1();

      // With carry.
      else
      {
         auto jump0 = module->chunkCODE.size();
         genCode(Code::Jcnd_Tru, 0);

         genAddXU1();
         auto jump1 = module->chunkCODE.size();
         genCode(Code::Jump_Lit, 0);

         module->chunkCODE[jump0].args[0] = getCodePos();
         genAddXU1();
         genStmntDropTmp(0);
         genCode(Code::Push_Lit, 1);
         genCode(Code::AddU);
         genCode(Code::Copy);
         genCode(Code::LNot);
         genStmntPushTmp(0);
         genCode(Code::BOrI);

         module->chunkCODE[jump1].args[0] = getCodePos();
      }
   }

   //
   // Info::genStmnt_Sub_U
   //
   void Info::genStmnt_Sub_U()
   {
      auto n = getStmntSize();

      if(n != 1 || stmnt->args[0] != stmnt->args[1])
         return genStmntArg('U', Code::SubU);

      if(isDecUArg(stmnt->args[0]) &&
         stmnt->args[2].a == IR::ArgBase::Lit &&
         stmnt->args[2].aLit.value->isValue() &&
         getWord(stmnt->args[2].aLit) == 1)
      {
         return genStmntDecUArg(stmnt->args[0], 0);
      }

      //
      // genReg
      //
      auto genReg = [this](IR::ArgPtr1 const &a, Code code)
      {
         if(stmnt->args[2].a != IR::ArgBase::Stk)
            genStmntPushArg(stmnt->args[2]);

         genCode(code, getExpAddPtr(a.idx->aLit.value, a.off));
      };

      switch(stmnt->args[0].a)
      {
      case IR::ArgBase::GblReg:
         return genReg(stmnt->args[0].aGblReg, Code::SubU_GblReg);

      case IR::ArgBase::HubReg:
         return genReg(stmnt->args[0].aHubReg, Code::SubU_HubReg);

      case IR::ArgBase::LocReg:
         return genReg(stmnt->args[0].aLocReg, Code::SubU_LocReg);

      case IR::ArgBase::ModReg:
         return genReg(stmnt->args[0].aModReg, Code::SubU_ModReg);

      default:
         break;
      }

      genStmntArg('U', Code::SubU);
   }

   //
   // Info::genStmnt_SubX_U
   //
   void Info::genStmnt_SubX_U()
   {
      Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;

      //
      // genSubXU1
      //
      auto genSubXU1 = [&]()
      {
         genStmntPushArg(stmnt->args[lop], 0);
         genStmntPushArg(stmnt->args[rop], 0);
         genCode(Code::SubU);
         genCode(Code::Copy);
         genStmntPushArg(stmnt->args[lop], 0);
         genStmntCall(getFuncName(IR::CodeBase::CmpGT+'U', 1), 1);
         genCode(Code::NegI);
      };

      // No carry.
      if(stmnt->args.size() == 3)
         genSubXU1();

      // With carry.
      else
      {
         auto jump0 = module->chunkCODE.size();
         genCode(Code::Jcnd_Tru, 0);

         genSubXU1();
         auto jump1 = module->chunkCODE.size();
         genCode(Code::Jump_Lit, 0);

         genSubXU1();
         genStmntDropTmp(0);
         genCode(Code::Push_Lit, 1);
         genCode(Code::SubU);
         genCode(Code::Copy);
         genCode(Code::BNot);
         genCode(Code::LNot);
         genCode(Code::NegI);
         genStmntPushTmp(0);
         genCode(Code::BOrI);

         module->chunkCODE[jump0].args[0] = getCodePos();
         module->chunkCODE[jump1].args[0] = getCodePos();
      }
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
   // Info::trStmnt_Add_U
   //
   void Info::trStmnt_Add_U()
   {
      auto n = getStmntSize();

      if(n != 1)
         return trStmntArgBin(false);

      if(stmnt->args[0] != stmnt->args[1])
      {
         if(stmnt->args[0] == stmnt->args[2])
            std::swap(stmnt->args[1], stmnt->args[2]);
         else
            return trStmntArgBin(false);
      }

      switch(stmnt->args[0].a)
      {
      case IR::ArgBase::GblReg:
      case IR::ArgBase::HubReg:
      case IR::ArgBase::LocReg:
      case IR::ArgBase::ModReg:
         return;

      default:
         break;
      }

      trStmntArgBin(false);
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

      if(n != 1 || stmnt->args[0] != stmnt->args[1])
         return trStmntArgBin(true);

      switch(stmnt->args[0].a)
      {
      case IR::ArgBase::GblReg:
      case IR::ArgBase::HubReg:
      case IR::ArgBase::LocReg:
      case IR::ArgBase::ModReg:
         return;

      default:
         break;
      }

      trStmntArgBin(true);
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

