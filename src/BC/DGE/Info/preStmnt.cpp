//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Doominati statement preparation.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "IR/CallType.hpp"
#include "IR/Linkage.hpp"
#include "IR/Program.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::preStmnt
   //
   void Info::preStmnt()
   {
      switch(stmnt->op.code)
      {
      case IR::Code::AddF_W: preStmnt_AddF_W(); break;
      case IR::Code::AddI_W: preStmnt_AddI_W(); break;
      case IR::Code::AddU_W: preStmnt_AddU_W(); break;
      case IR::Code::Bclo_W: preStmnt_Bclo_W(); break;
      case IR::Code::Bclz_W: preStmnt_Bclz_W(); break;

      case IR::Code::CmpF_EQ_W: preStmnt_CmpF_EQ_W(); break;
      case IR::Code::CmpF_GE_W: preStmnt_CmpF_GE_W(); break;
      case IR::Code::CmpF_GT_W: preStmnt_CmpF_LT_W(); break;
      case IR::Code::CmpF_LE_W: preStmnt_CmpF_LE_W(); break;
      case IR::Code::CmpF_LT_W: preStmnt_CmpF_LT_W(); break;
      case IR::Code::CmpF_NE_W: preStmnt_CmpF_NE_W(); break;
      case IR::Code::CmpI_EQ_W: preStmnt_CmpI_EQ_W(); break;
      case IR::Code::CmpI_GE_W: preStmnt_CmpI_GE_W(); break;
      case IR::Code::CmpI_GT_W: preStmnt_CmpI_LT_W(); break;
      case IR::Code::CmpI_LE_W: preStmnt_CmpI_LE_W(); break;
      case IR::Code::CmpI_LT_W: preStmnt_CmpI_LT_W(); break;
      case IR::Code::CmpI_NE_W: preStmnt_CmpI_NE_W(); break;
      case IR::Code::CmpU_EQ_W: preStmnt_CmpU_EQ_W(); break;
      case IR::Code::CmpU_GE_W: preStmnt_CmpU_GE_W(); break;
      case IR::Code::CmpU_GT_W: preStmnt_CmpU_GT_W(); break;
      case IR::Code::CmpU_LE_W: preStmnt_CmpU_LE_W(); break;
      case IR::Code::CmpU_LT_W: preStmnt_CmpU_LT_W(); break;
      case IR::Code::CmpU_NE_W: preStmnt_CmpU_NE_W(); break;

      case IR::Code::DiXI_W: preStmnt_DiXI_W(); break;
      case IR::Code::DiXU_W: preStmnt_DiXU_W(); break;
      case IR::Code::DivF_W: preStmnt_DivF_W(); break;
      case IR::Code::DivI_W: preStmnt_DivI_W(); break;
      case IR::Code::DivU_W: preStmnt_DivU_W(); break;
      case IR::Code::ModI_W: preStmnt_ModI_W(); break;
      case IR::Code::ModU_W: preStmnt_ModU_W(); break;
      case IR::Code::MuXU_W: preStmnt_MuXU_W(); break;
      case IR::Code::MulI_W: preStmnt_MulI_W(); break;
      case IR::Code::MulU_W: preStmnt_MulU_W(); break;
      case IR::Code::NegI_W: preStmnt_NegI_W(); break;
      case IR::Code::ShLU_W: preStmnt_ShLU_W(); break;
      case IR::Code::ShRI_W: preStmnt_ShRI_W(); break;
      case IR::Code::ShRU_W: preStmnt_ShRU_W(); break;
      case IR::Code::SubF_W: preStmnt_SubF_W(); break;
      case IR::Code::SubI_W: preStmnt_SubI_W(); break;
      case IR::Code::SubU_W: preStmnt_SubU_W(); break;

      default:
         break;
      }
   }

   //
   // Info::preStmntSykBin
   //
   void Info::preStmntStkBin(Core::FastU min, AddFunc add)
   {
      if(stmnt->op.size < min)
         return;

      (this->*add)(stmnt->op.size);
   }
}

// EOF

