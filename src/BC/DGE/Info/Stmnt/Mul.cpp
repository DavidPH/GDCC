//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Multiplication statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::putStmnt_MulU_W
   //
   void Info::putStmnt_MulU_W()
   {
      if(stmnt->op.size == 0)
         return;

      if(stmnt->op.size == 1)
         return putCode("MulU");

      putStmntCall(getFuncName({IR::Code::MulU_W, stmnt->op.size}), stmnt->op.size * 2);
   }

   //
   // Info::trStmnt_MuXU_W
   //
   void Info::trStmnt_MuXU_W()
   {
      trStmntStk3(stmnt->op.size * 2, stmnt->op.size, false);
   }
}

// EOF

