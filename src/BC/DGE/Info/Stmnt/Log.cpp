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
   // Info::putStmnt_LAnd
   //
   void Info::putStmnt_LAnd(char const *code)
   {
      if(stmnt->op.size == 0)
         putCode("Push_Lit", 0);

      if(stmnt->op.size == 1)
         putCode(code);

      if(stmnt->args[1].a == IR::ArgBase::Stk)
      {
         for(Core::FastU i = stmnt->op.size; --i;)
            putCode("OrIU");
         putStmntDropTmp(0);

         for(Core::FastU i = stmnt->op.size; --i;)
            putCode("OrIU");
         putStmntPushTmp(0);
      }
      else
      {
         putStmntPushArg(stmnt->args[1], 0);
         for(Core::FastU i = stmnt->op.size; --i;)
            putStmntPushArg(stmnt->args[1], i), putCode("OrIU");

         putStmntPushArg(stmnt->args[2], 0);
         for(Core::FastU i = stmnt->op.size; --i;)
            putStmntPushArg(stmnt->args[2], i), putCode("OrIU");
      }

      putCode(code);
   }

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
   // Info::trStmnt_LAnd
   //
   void Info::trStmnt_LAnd()
   {
      if(stmnt->op.size == 0)
         return;

      if(stmnt->op.size == 1)
         return trStmntStk3(1, stmnt->op.size, false);

      if(!isPushArg(stmnt->args[1]) || !isPushArg(stmnt->args[2]))
      {
         trStmntStk3(1, stmnt->op.size, false);
         trStmntTmp(1);
      }
      else
         moveArgStk_dst(stmnt->args[0], 1);
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

