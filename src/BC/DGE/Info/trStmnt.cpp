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
      case IR::Code::AddI_W:
      case IR::Code::AddU_W:
      case IR::Code::AndU_W:
      case IR::Code::CmpI_EQ_W:
      case IR::Code::CmpI_NE_W:
      case IR::Code::CmpU_EQ_W:
      case IR::Code::CmpU_NE_W:
      case IR::Code::MulU_W:
      case IR::Code::OrIU_W:
      case IR::Code::OrXU_W:
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
      case IR::Code::SubI_W:
      case IR::Code::SubU_W:
         trStmntStk3(stmnt->op.size, stmnt->op.size, true);
         break;

      case IR::Code::Call: trStmnt_Call(); break;
      case IR::Code::Cnat: trStmnt_Cnat(); break;

      case IR::Code::Jcnd_Nil:
      case IR::Code::Jcnd_Tru:
         CheckArgC(stmnt, 2);
         moveArgStk_src(stmnt->args[0], stmnt->op.size);
         CheckArgB(stmnt, 1, IR::ArgBase::Lit);
         break;

      case IR::Code::Jfar: trStmnt_Jfar(); break;
      case IR::Code::Jump: trStmnt_Jump(); break;

      case IR::Code::Move_B: trStmnt_Move_B(); break;
      case IR::Code::Move_W: trStmnt_Move_W(); break;

      case IR::Code::NotU_W: trStmntStk2(1, stmnt->op.size); break;

      case IR::Code::Pltn:
         CheckArgC(stmnt, 2);
         moveArgStk_dst(stmnt->args[0], stmnt->op.size);
         moveArgStk_src(stmnt->args[1], stmnt->op.size);
         break;

      case IR::Code::Retn: trStmnt_Retn(); break;

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

