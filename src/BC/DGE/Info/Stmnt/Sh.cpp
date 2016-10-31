//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Shift statement handling.
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
   // Info::preStmnt_ShLU_W
   //
   void Info::preStmnt_ShLU_W()
   {
      if(stmnt->op.size <= 1)
         return;

      // TODO: Inline literal shift.

      addFunc_ShLU_W(stmnt->op.size);
   }

   //
   // Info::preStmnt_ShRI_W
   //
   void Info::preStmnt_ShRI_W()
   {
      if(stmnt->op.size <= 1)
         return;

      // TODO: Inline literal shift.

      addFunc_ShRI_W(stmnt->op.size);
   }

   //
   // Info::preStmnt_ShRU_W
   //
   void Info::preStmnt_ShRU_W()
   {
      if(stmnt->op.size <= 1)
         return;

      // TODO: Inline literal shift.

      addFunc_ShRU_W(stmnt->op.size);
   }

   //
   // Info::putStmnt_ShLU_W
   //
   void Info::putStmnt_ShLU_W(char const *code)
   {
      if(stmnt->op.size == 0)
         return putCode("Drop_Nul");

      if(stmnt->op.size == 1)
         return putCode(code);

      // TODO: Inline literal shift.

      putStmntCall(getFuncName(stmnt->op), stmnt->op.size + 1);
   }

   //
   // Info::trStmnt_ShLU_W
   //
   void Info::trStmnt_ShLU_W()
   {
      if(stmnt->op.size <= 1)
         trStmntShift(stmnt->op.size, true);

      // TODO: Inline literal shift.

      trStmntShift(stmnt->op.size, true);
   }
}

// EOF

