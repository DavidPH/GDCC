//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
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
      switch(stmnt->code)
      {
      case IR::Code::AddF: preStmnt_AddF(); break;
      case IR::Code::AddI: preStmnt_AddI(); break;
      case IR::Code::AddU: preStmnt_AddU(); break;
      case IR::Code::Bclo: preStmnt_Bclo(); break;
      case IR::Code::Bclz: preStmnt_Bclz(); break;

      case IR::Code::CmpF_EQ: preStmnt_CmpF_EQ(); break;
      case IR::Code::CmpF_GE: preStmnt_CmpF_GE(); break;
      case IR::Code::CmpF_GT: preStmnt_CmpF_GT(); break;
      case IR::Code::CmpF_LE: preStmnt_CmpF_LE(); break;
      case IR::Code::CmpF_LT: preStmnt_CmpF_LT(); break;
      case IR::Code::CmpF_NE: preStmnt_CmpF_NE(); break;
      case IR::Code::CmpI_EQ: preStmnt_CmpI_EQ(); break;
      case IR::Code::CmpI_GE: preStmnt_CmpI_GE(); break;
      case IR::Code::CmpI_GT: preStmnt_CmpI_GT(); break;
      case IR::Code::CmpI_LE: preStmnt_CmpI_LE(); break;
      case IR::Code::CmpI_LT: preStmnt_CmpI_LT(); break;
      case IR::Code::CmpI_NE: preStmnt_CmpI_NE(); break;
      case IR::Code::CmpU_EQ: preStmnt_CmpU_EQ(); break;
      case IR::Code::CmpU_GE: preStmnt_CmpU_GE(); break;
      case IR::Code::CmpU_GT: preStmnt_CmpU_GT(); break;
      case IR::Code::CmpU_LE: preStmnt_CmpU_LE(); break;
      case IR::Code::CmpU_LT: preStmnt_CmpU_LT(); break;
      case IR::Code::CmpU_NE: preStmnt_CmpU_NE(); break;

      case IR::Code::DiXI: preStmnt_DiXI(); break;
      case IR::Code::DiXU: preStmnt_DiXU(); break;
      case IR::Code::DivF: preStmnt_DivF(); break;
      case IR::Code::DivI: preStmnt_DivI(); break;
      case IR::Code::DivK: preStmnt_DivK(); break;
      case IR::Code::DivU: preStmnt_DivU(); break;
      case IR::Code::DivX: preStmnt_DivX(); break;
      case IR::Code::ModI: preStmnt_ModI(); break;
      case IR::Code::ModU: preStmnt_ModU(); break;
      case IR::Code::MuXU: preStmnt_MuXU(); break;
      case IR::Code::MulF: preStmnt_MulF(); break;
      case IR::Code::MulI: preStmnt_MulI(); break;
      case IR::Code::MulK: preStmnt_MulK(); break;
      case IR::Code::MulU: preStmnt_MulU(); break;
      case IR::Code::MulX: preStmnt_MulX(); break;
      case IR::Code::NegI: preStmnt_NegI(); break;
      case IR::Code::ShLF: preStmnt_ShLF(); break;
      case IR::Code::ShLU: preStmnt_ShLU(); break;
      case IR::Code::ShRF: preStmnt_ShRF(); break;
      case IR::Code::ShRI: preStmnt_ShRI(); break;
      case IR::Code::ShRU: preStmnt_ShRU(); break;
      case IR::Code::SubF: preStmnt_SubF(); break;
      case IR::Code::SubI: preStmnt_SubI(); break;
      case IR::Code::SubU: preStmnt_SubU(); break;

      default:
         break;
      }
   }

   //
   // Info::preStmntStkBin
   //
   void Info::preStmntStkBin(Core::FastU min, AddFunc add)
   {
      auto n = getStmntSizeW();

      if(n < min)
         return;

      (this->*add)(n);
   }
}

// EOF

