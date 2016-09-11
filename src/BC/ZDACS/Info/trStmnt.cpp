//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR statement translation.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Statement.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      namespace ZDACS
      {
         //
         // Info::trStmnt
         //
         void Info::trStmnt()
         {
            switch(stmnt->op.code)
            {
            case IR::Code::Nop: break;

            case IR::Code::AdXU_W: trStmnt_AdXU_W(); break;

            case IR::Code::AddF_W:
            case IR::Code::MulF_W:
            case IR::Code::MulI_W:
            case IR::Code::MulK_W:
            case IR::Code::MulU_W:
            case IR::Code::MulX_W:
               trStmntStk3(stmnt->op.size, stmnt->op.size, false);
               break;

            case IR::Code::AddI_W:
            case IR::Code::AddU_W: trStmnt_AddU_W(); break;

            case IR::Code::AndU_W: trStmntBitwise(); break;

            case IR::Code::Bclo_W: trStmnt_Bclz_W(); break;
            case IR::Code::Bclz_W: trStmnt_Bclz_W(); break;
            case IR::Code::Bges_W: trStmnt_Bget_W(); break;
            case IR::Code::Bget_W: trStmnt_Bget_W(); break;
            case IR::Code::Bset_W: trStmnt_Bset_W(); break;

            case IR::Code::Call: trStmnt_Call(); break;

            case IR::Code::Casm: trStmnt_Casm(); break;

            case IR::Code::CmpF_EQ_W:
            case IR::Code::CmpF_NE_W:
               trStmntStk3(1, stmnt->op.size, false);
               break;

            case IR::Code::CmpF_GE_W:
            case IR::Code::CmpF_GT_W:
            case IR::Code::CmpF_LE_W:
            case IR::Code::CmpF_LT_W:
               trStmntStk3(1, stmnt->op.size, true);
               break;

            case IR::Code::CmpI_EQ_W:
            case IR::Code::CmpI_NE_W:
            case IR::Code::CmpU_EQ_W:
            case IR::Code::CmpU_NE_W:
               trStmnt_CmpU_EQ_W();
               break;

            case IR::Code::CmpI_GE_W:
            case IR::Code::CmpI_GT_W:
            case IR::Code::CmpI_LE_W:
            case IR::Code::CmpI_LT_W:
            case IR::Code::CmpU_GE_W:
            case IR::Code::CmpU_GT_W:
            case IR::Code::CmpU_LE_W:
            case IR::Code::CmpU_LT_W:
               trStmntStk3(1, stmnt->op.size, true);
               break;

            case IR::Code::Cnat: trStmnt_Cnat(); break;

            case IR::Code::Copy_W:
               CheckArgC(stmnt, 2);
               CheckArgB(stmnt, 0, IR::ArgBase::Stk);
               CheckArgB(stmnt, 1, IR::ArgBase::Stk);
               break;

            case IR::Code::Cscr_IA: trStmnt_Cscr_IA(); break;
            case IR::Code::Cscr_IS: trStmnt_Cscr_IS(); break;
            case IR::Code::Cscr_SA: trStmnt_Cscr_SA(); break;
            case IR::Code::Cscr_SS: trStmnt_Cscr_SS(); break;

            case IR::Code::Cspe: trStmnt_Cspe(); break;

            case IR::Code::DiXI_W: trStmnt_DiXI_W(); break;
            case IR::Code::DiXU_W: trStmnt_DiXU_W(); break;

            case IR::Code::DivF_W:
            case IR::Code::DivI_W:
            case IR::Code::DivK_W:
            case IR::Code::DivU_W:
            case IR::Code::DivX_W:
            case IR::Code::SubF_W:
               trStmntStk3(stmnt->op.size, stmnt->op.size, true);
               break;

            case IR::Code::InvU_W: trStmnt_InvU_W(); break;

            case IR::Code::Jcnd_Nil: trStmnt_Jcnd_Nil(); break;
            case IR::Code::Jcnd_Tab: trStmnt_Jcnd_Tab(); break;
            case IR::Code::Jcnd_Tru: trStmnt_Jcnd_Tru(); break;

            case IR::Code::Jdyn:
               CheckArgC(stmnt, 1);
               moveArgStk_src(stmnt->args[0], 1);
               break;

            case IR::Code::Jfar: trStmnt_Jfar(); break;

            case IR::Code::Jset: trStmnt_Jset(); break;

            case IR::Code::Jump: trStmnt_Jump(); break;

            case IR::Code::LAnd:
            case IR::Code::LOrI:
               trStmntStk3(1, stmnt->op.size, false);
               break;

            case IR::Code::ModI_W: trStmnt_ModI_W(); break;
            case IR::Code::ModU_W: trStmnt_ModU_W(); break;

            case IR::Code::Move_W: trStmnt_Move_W(); break;

            case IR::Code::MuXU_W:
               trStmntStk3(stmnt->op.size * 2, stmnt->op.size, false);
               break;

            case IR::Code::NegF_W: trStmntStk2(stmnt->op.size, stmnt->op.size); break;
            case IR::Code::NegI_W: trStmnt_NegI_W(); break;

            case IR::Code::NotU_W: trStmntStk2(1, stmnt->op.size); break;

            case IR::Code::OrIU_W: trStmntBitwise(); break;
            case IR::Code::OrXU_W: trStmntBitwise(); break;

            case IR::Code::Pltn: trStmntStk2(1, 1); break;

            case IR::Code::Retn: trStmnt_Retn(); break;

            case IR::Code::ShLF_W: trStmnt_ShLF_W(); break;
            case IR::Code::ShLU_W: trStmnt_ShLU_W(); break;
            case IR::Code::ShRF_W: trStmnt_ShLF_W(); break;
            case IR::Code::ShRI_W: trStmnt_ShRI_W(); break;
            case IR::Code::ShRU_W: trStmnt_ShRU_W(); break;

            case IR::Code::SuXU_W: trStmnt_SuXU_W(); break;

            case IR::Code::SubI_W:
            case IR::Code::SubU_W: trStmnt_SubU_W(); break;

            case IR::Code::Swap_W: trStmnt_Swap_W(); break;

            case IR::Code::Xcod_SID: break;

            default:
               std::cerr << "ERROR: " << stmnt->pos
                  << ": cannot translate Code for ZDACS: " << stmnt->op << '\n';
               throw EXIT_FAILURE;
            }
         }
      }
   }
}

// EOF

