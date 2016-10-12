//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
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

#include "IR/Statement.hpp"

#include <iostream>


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
      switch(stmnt->op.code)
      {
      case IR::Code::AdXU_W: trStmnt_AdXU_W(); break;
      case IR::Code::AddI_W:
      case IR::Code::AddU_W: trStmnt_AddU_W(); break;
      case IR::Code::AndU_W: trStmnt_AndU_W(); break;

      case IR::Code::CmpI_EQ_W:
      case IR::Code::CmpI_NE_W:
      case IR::Code::CmpU_EQ_W:
      case IR::Code::CmpU_NE_W:
         trStmntStk3(stmnt->op.size, stmnt->op.size, false);
         break;

      case IR::Code::CmpI_GE_W:
      case IR::Code::CmpI_GT_W:
      case IR::Code::CmpI_LE_W:
      case IR::Code::CmpI_LT_W:
      case IR::Code::CmpU_GE_W:
      case IR::Code::CmpU_GT_W:
      case IR::Code::CmpU_LE_W:
      case IR::Code::CmpU_LT_W:
      case IR::Code::DivI_W:
      case IR::Code::DivU_W:
      case IR::Code::ModI_W:
      case IR::Code::ModU_W:
         trStmntStk3(stmnt->op.size, stmnt->op.size, true);
         break;

      case IR::Code::Call: trStmnt_Call(); break;
      case IR::Code::Cnat: trStmnt_Cnat(); break;

      case IR::Code::Jcnd_Nil: trStmnt_Jcnd_Nil(); break;
      case IR::Code::Jcnd_Tru: trStmnt_Jcnd_Tru(); break;

      case IR::Code::Jfar: trStmnt_Jfar(); break;
      case IR::Code::Jump: trStmnt_Jump(); break;
      case IR::Code::LNot: trStmnt_LNot(); break;

      case IR::Code::Move_B: trStmnt_Move_B(); break;
      case IR::Code::Move_W: trStmnt_Move_W(); break;
      case IR::Code::MuXU_W: trStmnt_MuXU_W(); break;
      case IR::Code::MulI_W:
      case IR::Code::MulU_W: trStmnt_MulU_W(); break;
      case IR::Code::OrIU_W: trStmnt_OrIU_W(); break;
      case IR::Code::OrXU_W: trStmnt_OrXU_W(); break;

      case IR::Code::Pltn:
         CheckArgC(stmnt, 2);
         moveArgStk_dst(stmnt->args[0], stmnt->op.size);
         moveArgStk_src(stmnt->args[1], stmnt->op.size);
         break;

      case IR::Code::Retn: trStmnt_Retn(); break;

      case IR::Code::SuXU_W: trStmnt_SuXU_W(); break;
      case IR::Code::SubI_W:
      case IR::Code::SubU_W: trStmnt_SubU_W(); break;

      case IR::Code::Swap_W:
         CheckArgC(stmnt, 2);
         CheckArgB(stmnt, 0, IR::ArgBase::Stk);
         CheckArgB(stmnt, 1, IR::ArgBase::Stk);
         break;

      default:
         std::cerr << "ERROR: " << stmnt->pos
            << ": cannot translate Code for DGE: " << stmnt->op << '\n';
         throw EXIT_FAILURE;
      }
   }
}

// EOF

