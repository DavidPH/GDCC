//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Logical statement handling.
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
   // Info::putStmnt_LNot
   //
   void Info::putStmnt_LNot()
   {
      if(stmnt->op.size == 0)
         return;

      for(auto n = stmnt->op.size; --n;)
         putCode("OrIU");

      putCode("LNot");
   }

   //
   // Info::trStmnt_LNot
   //
   void Info::trStmnt_LNot()
   {
      trStmntStk2(1, stmnt->op.size);
   }
}

// EOF

