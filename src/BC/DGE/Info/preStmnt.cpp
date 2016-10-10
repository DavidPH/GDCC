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

