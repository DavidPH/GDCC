//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Division statement handling.
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
   // Info::putStmnt_DivU_W
   //
   void Info::putStmnt_DivU_W(char const *code, IR::Code codeX, bool mod)
   {
      if(stmnt->op.size == 0)
         return;

      if(stmnt->op.size == 1)
         return putCode(code);

      putStmntCall(getFuncName({codeX, stmnt->op.size}), stmnt->op.size * 2);
      if(mod) putStmntDropTmp(0, stmnt->op.size);
      for(Core::FastU i = stmnt->op.size; i--;) putCode("Drop_Nul");
      if(mod) putStmntPushTmp(0, stmnt->op.size);
   }

   //
   // Info::trStmnt_DiXU_W
   //
   void Info::trStmnt_DiXU_W()
   {
      trStmntStk3(stmnt->op.size * 2, stmnt->op.size, true);
   }

   //
   // Info::trStmnt_ModU_W
   //
   void Info::trStmnt_ModU_W()
   {
      trStmntStk3(stmnt->op.size, stmnt->op.size, true);

      if(stmnt->op.size > 1)
         trStmntTmp(stmnt->op.size);
   }
}

// EOF

