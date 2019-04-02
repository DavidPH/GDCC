//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
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
      auto n = getStmntSizeW();

      if(n == 0)
         return putCode("Push_Lit", 0);

      if(n == 1)
         return putCode(code);

      if(stmnt->args[1].a == IR::ArgBase::Stk)
      {
         for(Core::FastU i = n; --i;)
            putCode("BOrI");
         putStmntDropTmp(0);

         for(Core::FastU i = n; --i;)
            putCode("BOrI");
         putStmntPushTmp(0);
      }
      else
      {
         putStmntPushArg(stmnt->args[1], 0);
         for(Core::FastU i = n; --i;)
            putStmntPushArg(stmnt->args[1], i), putCode("BOrI");

         putStmntPushArg(stmnt->args[2], 0);
         for(Core::FastU i = n; --i;)
            putStmntPushArg(stmnt->args[2], i), putCode("BOrI");
      }

      putCode(code);
   }

   //
   // Info::putStmnt_LNot
   //
   void Info::putStmnt_LNot()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      for(auto i = n; --i;)
         putCode("BOrI");

      putCode("LNot");
   }

   //
   // Info::trStmnt_LAnd
   //
   void Info::trStmnt_LAnd()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         return trStmntStk3(false);

      if(!isPushArg(stmnt->args[1]) || !isPushArg(stmnt->args[2]))
      {
         trStmntStk3(false);
         trStmntTmp(1);
      }
      else
         moveArgStk_dst(stmnt->args[0]);
   }

   //
   // Info::trStmnt_LNot
   //
   void Info::trStmnt_LNot()
   {
      trStmntStk2();
   }
}

// EOF

