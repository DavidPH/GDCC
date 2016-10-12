//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Compare statement handling.
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
   // Info::preStmnt_CmpU_EQ_W
   //
   void Info::preStmnt_CmpU_EQ_W(void (Info::*fn)(Core::FastU))
   {
      if(stmnt->op.size <= 1)
         return;

      // TODO: Inline multi-word push-args.

      (this->*fn)(stmnt->op.size);
   }

   //
   // Info::preStmnt_CmpU_GE_W
   //
   void Info::preStmnt_CmpU_GE_W(void (Info::*fn)(Core::FastU))
   {
      if(stmnt->op.size <= 1)
         return;

      (this->*fn)(stmnt->op.size);
   }

   //
   // Info::putStmnt_CmpU_EQ_W
   //
   void Info::putStmnt_CmpU_EQ_W(IR::Code code)
   {
      if(stmnt->op.size == 0)
         return putCode("Push_Lit", code == IR::Code::CmpU_EQ_W);

      if(stmnt->op.size == 1)
         return putCode(code == IR::Code::CmpU_EQ_W ? "CmpU_EQ" : "CmpU_NE");

      // TODO: Inline multi-word push-args.

      putStmntCall(getFuncName({code, stmnt->op.size}), stmnt->op.size * 2);
   }

   //
   // Info::putStmnt_CmpU_GE_W
   //
   void Info::putStmnt_CmpU_GE_W(char const *code)
   {
      if(stmnt->op.size == 0)
         return putCode("Push_Lit", code[6] == 'E');

      if(stmnt->op.size == 1)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->op), stmnt->op.size * 2);
   }

   //
   // Info::trStmnt_CmpU_EQ_W
   //
   void Info::trStmnt_CmpU_EQ_W()
   {
      // TODO: Inline multi-word push-args.

      trStmntStk3(1, stmnt->op.size, false);
   }

   //
   // Info::trStmnt_CmpU_GE_W
   //
   void Info::trStmnt_CmpU_GE_W()
   {
      trStmntStk3(1, stmnt->op.size, true);
   }
}

// EOF

