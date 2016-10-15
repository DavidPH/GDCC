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
      case IR::Code::AddI_W:
      case IR::Code::AddU_W: preStmnt_AddU_W(); break;
      case IR::Code::Bclo_W: preStmnt_Bclo_W(); break;
      case IR::Code::Bclz_W: preStmnt_Bclz_W(); break;

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

      case IR::Code::MuXU_W: preStmnt_MuXU_W(); break;
      case IR::Code::MulI_W:
      case IR::Code::MulU_W: preStmnt_MulU_W(); break;
      case IR::Code::SubI_W:
      case IR::Code::SubU_W: preStmnt_SubU_W(); break;

      default:
         break;
      }
   }
}

// EOF

