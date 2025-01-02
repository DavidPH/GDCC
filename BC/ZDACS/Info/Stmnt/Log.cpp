//-----------------------------------------------------------------------------
//
// Copyright (C) 2019-2024 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Logical statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "IR/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::chkStmnt_LAnd
   //
   void Info::chkStmnt_LAnd()
   {
      if(getStmntSize() < 1)
         IR::ErrorCode(stmnt, "unsupported size");
   }

   //
   // Info::genStmnt_LAnd
   //
   void Info::genStmnt_LAnd(Code code)
   {
      auto n = getStmntSize();

      if(n == 0)
         return genCode(Code::Push_Lit, 0);

      if(n == 1)
         return genCode(code);

      if(stmnt->args[1].a == IR::ArgBase::Stk)
      {
         for(Core::FastU i = n; --i;)
            genCode(Code::BOrI);
         genStmntDropTmp(0);

         for(Core::FastU i = n; --i;)
            genCode(Code::BOrI);
         genStmntPushTmp(0);
      }
      else
      {
         genStmntPushArg(stmnt->args[1], 0);
         for(Core::FastU i = n; --i;)
            genStmntPushArg(stmnt->args[1], i), genCode(Code::BOrI);

         genStmntPushArg(stmnt->args[2], 0);
         for(Core::FastU i = n; --i;)
            genStmntPushArg(stmnt->args[2], i), genCode(Code::BOrI);
      }

      genCode(code);
   }

   //
   // Info::genStmnt_LNot
   //
   void Info::genStmnt_LNot()
   {
      auto n = getStmntSize();

      if(auto i = n) while(--i) genCode(Code::BOrI);
      genCode(Code::LNot);
   }

   //
   // Info::trStmnt_LAnd
   //
   void Info::trStmnt_LAnd()
   {
      auto n = getStmntSize();

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

