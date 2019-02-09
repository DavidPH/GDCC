//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Doominati statement translations.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "IR/Exception.hpp"
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
      switch(stmnt->code.base)
      {
      case IR::CodeBase::Nop: break;

      case IR::CodeBase::Add:      trStmnt_Add(); break;
      case IR::CodeBase::AddX:     trStmnt_AddX(); break;
      case IR::CodeBase::BAnd:     trStmnt_BAnd(); break;
      case IR::CodeBase::BNot:     trStmnt_BNot(); break;
      case IR::CodeBase::BOrI:     trStmnt_BOrI(); break;
      case IR::CodeBase::BOrX:     trStmnt_BOrX(); break;
      case IR::CodeBase::Bclo:     trStmnt_Bclo(); break;
      case IR::CodeBase::Bclz:     trStmnt_Bclz(); break;
      case IR::CodeBase::CmpEQ:    trStmnt_CmpEQ(); break;
      case IR::CodeBase::CmpGE:    trStmnt_CmpGE(); break;
      case IR::CodeBase::CmpGT:    trStmnt_CmpGT(); break;
      case IR::CodeBase::CmpLE:    trStmnt_CmpLE(); break;
      case IR::CodeBase::CmpLT:    trStmnt_CmpLT(); break;
      case IR::CodeBase::CmpNE:    trStmnt_CmpNE(); break;
      case IR::CodeBase::Call:     trStmnt_Call(); break;
      case IR::CodeBase::Cnat:     trStmnt_Cnat(); break;
      case IR::CodeBase::Copy:     trStmnt_Copy(); break;
      case IR::CodeBase::Div:      trStmnt_Div(); break;
      case IR::CodeBase::DivX:     trStmnt_DivX(); break;
      case IR::CodeBase::Jcnd_Nil: trStmnt_Jcnd_Nil(); break;
      case IR::CodeBase::Jcnd_Tab: trStmnt_Jcnd_Tab(); break;
      case IR::CodeBase::Jcnd_Tru: trStmnt_Jcnd_Tru(); break;
      case IR::CodeBase::Jfar_Pro: trStmnt_Jfar_Pro(); break;
      case IR::CodeBase::Jfar_Set: trStmnt_Jfar_Set(); break;
      case IR::CodeBase::Jfar_Sta: trStmnt_Jfar_Sta(); break;
      case IR::CodeBase::Jump:     trStmnt_Jump(); break;
      case IR::CodeBase::LAnd:     trStmnt_LAnd(); break;
      case IR::CodeBase::LNot:     trStmnt_LNot(); break;
      case IR::CodeBase::LOrI:     trStmnt_LOrI(); break;
      case IR::CodeBase::Mod:      trStmnt_Mod(); break;
      case IR::CodeBase::Move:     trStmnt_Move(); break;
      case IR::CodeBase::Mul:      trStmnt_Mul(); break;
      case IR::CodeBase::MulX:     trStmnt_MulX(); break;
      case IR::CodeBase::Neg:      trStmnt_Neg(); break;
      case IR::CodeBase::Pltn:     trStmnt_Pltn(); break;
      case IR::CodeBase::Retn:     trStmnt_Retn(); break;
      case IR::CodeBase::Rjnk:     trStmnt_Rjnk(); break;
      case IR::CodeBase::ShL:      trStmnt_ShL(); break;
      case IR::CodeBase::ShR:      trStmnt_ShR(); break;
      case IR::CodeBase::Sub:      trStmnt_Sub(); break;
      case IR::CodeBase::SubX:     trStmnt_SubX(); break;
      case IR::CodeBase::Swap:     trStmnt_Swap(); break;

      default:
         IR::ErrorCode(stmnt, "unsupported tr");
      }
   }

   //
   // Info::trStmnt_Pltn
   //
   void Info::trStmnt_Pltn()
   {
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

