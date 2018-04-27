//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
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
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         return putCode("MulU");

      putStmntCall(getFuncName(IR::Code::MulU_W, n), n * 2);
   }

   //
   // Info::trStmnt_MuXU_W
   //
   void Info::trStmnt_MuXU_W()
   {
      trStmntStk3(false);
   }
}

// EOF

