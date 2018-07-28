//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Doominati statement translations.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::trStmnt
   //
   void Info::trStmnt()
   {
      switch(stmnt->code)
      {
      case IR::Code::Nop: break;

      case IR::Code::AdXU: trStmnt_AdXU(); break;
      case IR::Code::AddF: trStmnt_AddF(); break;
      case IR::Code::AddI: trStmnt_AddI(); break;
      case IR::Code::AddU: trStmnt_AddU(); break;
      case IR::Code::BAnd: trStmnt_BAnd(); break;
      case IR::Code::BNot: trStmnt_BNot(); break;
      case IR::Code::BOrI: trStmnt_BOrI(); break;
      case IR::Code::BOrX: trStmnt_BOrX(); break;
      case IR::Code::Bclo: trStmnt_Bclo(); break;
      case IR::Code::Bclz: trStmnt_Bclz(); break;

      case IR::Code::CmpF_EQ: trStmnt_CmpF_EQ(); break;
      case IR::Code::CmpF_GE: trStmnt_CmpF_GE(); break;
      case IR::Code::CmpF_GT: trStmnt_CmpF_GT(); break;
      case IR::Code::CmpF_LE: trStmnt_CmpF_LE(); break;
      case IR::Code::CmpF_LT: trStmnt_CmpF_LT(); break;
      case IR::Code::CmpF_NE: trStmnt_CmpF_NE(); break;
      case IR::Code::CmpI_EQ: trStmnt_CmpI_EQ(); break;
      case IR::Code::CmpI_GE: trStmnt_CmpI_GE(); break;
      case IR::Code::CmpI_GT: trStmnt_CmpI_GT(); break;
      case IR::Code::CmpI_LE: trStmnt_CmpI_LE(); break;
      case IR::Code::CmpI_LT: trStmnt_CmpI_LT(); break;
      case IR::Code::CmpI_NE: trStmnt_CmpI_NE(); break;
      case IR::Code::CmpU_EQ: trStmnt_CmpU_EQ(); break;
      case IR::Code::CmpU_GE: trStmnt_CmpU_GE(); break;
      case IR::Code::CmpU_GT: trStmnt_CmpU_GT(); break;
      case IR::Code::CmpU_LE: trStmnt_CmpU_LE(); break;
      case IR::Code::CmpU_LT: trStmnt_CmpU_LT(); break;
      case IR::Code::CmpU_NE: trStmnt_CmpU_NE(); break;

      case IR::Code::Call: trStmnt_Call(); break;
      case IR::Code::Cnat: trStmnt_Cnat(); break;

      case IR::Code::Copy: trStmnt_Copy(); break;
      case IR::Code::DiXI: trStmnt_DiXI(); break;
      case IR::Code::DiXU: trStmnt_DiXU(); break;
      case IR::Code::DivF: trStmnt_DivF(); break;
      case IR::Code::DivI: trStmnt_DivI(); break;
      case IR::Code::DivK: trStmnt_DivK(); break;
      case IR::Code::DivU: trStmnt_DivU(); break;
      case IR::Code::DivX: trStmnt_DivX(); break;

      case IR::Code::Jcnd_Nil: trStmnt_Jcnd_Nil(); break;
      case IR::Code::Jcnd_Tab: trStmnt_Jcnd_Tab(); break;
      case IR::Code::Jcnd_Tru: trStmnt_Jcnd_Tru(); break;

      case IR::Code::Jfar: trStmnt_Jfar(); break;
      case IR::Code::Jset: trStmnt_Jset(); break;
      case IR::Code::Jump: trStmnt_Jump(); break;
      case IR::Code::LAnd: trStmnt_LAnd(); break;
      case IR::Code::LNot: trStmnt_LNot(); break;
      case IR::Code::LOrI: trStmnt_LOrI(); break;

      case IR::Code::ModI: trStmnt_ModI(); break;
      case IR::Code::ModU: trStmnt_ModU(); break;
      case IR::Code::Move: trStmnt_Move(); break;
      case IR::Code::MuXU: trStmnt_MuXU(); break;
      case IR::Code::MulF: trStmnt_MulF(); break;
      case IR::Code::MulI: trStmnt_MulI(); break;
      case IR::Code::MulK: trStmnt_MulK(); break;
      case IR::Code::MulU: trStmnt_MulU(); break;
      case IR::Code::MulX: trStmnt_MulX(); break;
      case IR::Code::NegF: trStmnt_NegF(); break;
      case IR::Code::NegI: trStmnt_NegI(); break;

      case IR::Code::Pltn: trStmnt_Pltn(); break;
      case IR::Code::Retn: trStmnt_Retn(); break;

      case IR::Code::ShLF: trStmnt_ShLF(); break;
      case IR::Code::ShLU: trStmnt_ShLU(); break;
      case IR::Code::ShRF: trStmnt_ShRF(); break;
      case IR::Code::ShRI: trStmnt_ShRI(); break;
      case IR::Code::ShRU: trStmnt_ShRU(); break;
      case IR::Code::SuXU: trStmnt_SuXU(); break;
      case IR::Code::SubF: trStmnt_SubF(); break;
      case IR::Code::SubI: trStmnt_SubI(); break;
      case IR::Code::SubU: trStmnt_SubU(); break;
      case IR::Code::Swap: trStmnt_Swap(); break;

      default:
         Core::Error(stmnt->pos, "DGE cannot tr Code: ", stmnt->code);
      }
   }

   //
   // Info::trStmnt_Pltn
   //
   void Info::trStmnt_Pltn()
   {
      CheckArgC(stmnt, 2);
      moveArgStk_dst(stmnt->args[0]);
      moveArgStk_src(stmnt->args[1]);
   }

   //
   // Info::trStmntStkBin
   //
   void Info::trStmntStkBin(bool ordered)
   {
      trStmntStk3(ordered);
   }

   //
   // Info::trStmntStkCmp
   //
   void Info::trStmntStkCmp(bool ordered)
   {
      trStmntStk3(ordered);
   }

   //
   // Info::trStmntStkShi
   //
   void Info::trStmntStkShi()
   {
      trStmntShift(true);
   }

   //
   // Info::trStmntStkUna
   //
   void Info::trStmntStkUna()
   {
      trStmntStk2();
   }

   //
   // Info::trStmntTmp
   //
   void Info::trStmntTmp(Core::FastU n)
   {
      func->setLocalTmp(n);
   }
}

// EOF

