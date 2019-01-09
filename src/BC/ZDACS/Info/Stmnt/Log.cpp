//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 David Hill
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
   void Info::genStmnt_LAnd()
   {
      auto n = getStmntSize();

      if(n == 0)
         numChunkCODE += 8;

      else if(n == 1)
         numChunkCODE += 4;

      else
      {
         if(stmnt->args[1].a == IR::ArgBase::Stk)
            numChunkCODE += (n - 1) * 8 + lenDropTmp(0) + lenPushTmp(0);

         else
         {
            numChunkCODE += (n - 1) * 8
               + lenPushArg(stmnt->args[1], 0, n)
               + lenPushArg(stmnt->args[2], 0, n);
         }

         numChunkCODE += 4;
      }
   }

   //
   // Info::genStmnt_LNot
   //
   void Info::genStmnt_LNot()
   {
      numChunkCODE += getStmntSize() * 4;
   }

   //
   // Info::putStmnt_LAnd
   //
   void Info::putStmnt_LAnd(Code code)
   {
      auto n = getStmntSize();

      if(n == 0)
         return putCode(Code::Push_Lit, 0);

      if(n == 1)
         return putCode(code);

      if(stmnt->args[1].a == IR::ArgBase::Stk)
      {
         for(Core::FastU i = n; --i;)
            putCode(Code::BOrI);
         putStmntDropTmp(0);

         for(Core::FastU i = n; --i;)
            putCode(Code::BOrI);
         putStmntPushTmp(0);
      }
      else
      {
         putStmntPushArg(stmnt->args[1], 0);
         for(Core::FastU i = n; --i;)
            putStmntPushArg(stmnt->args[1], i), putCode(Code::BOrI);

         putStmntPushArg(stmnt->args[2], 0);
         for(Core::FastU i = n; --i;)
            putStmntPushArg(stmnt->args[2], i), putCode(Code::BOrI);
      }

      putCode(code);
   }

   //
   // Info::putStmnt_LNot
   //
   void Info::putStmnt_LNot()
   {
      auto n = getStmntSize();

      if(auto i = n) while(--i) putCode(Code::BOrI);
      putCode(Code::LNot);
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

