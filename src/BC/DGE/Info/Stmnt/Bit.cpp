//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Bitwise statement handling.
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
   // Info::preStmnt_Bclo
   //
   void Info::preStmnt_Bclo()
   {
      auto n = getStmntSizeW();

      if(n <= 1)
         return;

      addFunc_Bclo_W(n);
   }

   //
   // Info::preStmnt_Bclz
   //
   void Info::preStmnt_Bclz()
   {
      auto n = getStmntSizeW();

      if(n <= 1)
         return;

      addFunc_Bclz_W(n);
   }

   //
   // Info::putStmnt_BAnd
   //
   void Info::putStmnt_BAnd(char const *code)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         return putCode(code);

      if(stmnt->args[1].a == IR::ArgBase::Stk)
      {
         putStmntDropTmp(0, n * 2 - 1);

         putStmntPushTmp(n - 1);
         putCode(code);

         for(Core::FastU i = 0, e = n - 1; i != e; ++i)
         {
            putStmntPushTmp(i + n);
            putStmntPushTmp(i);
            putCode(code);
         }
      }
      else
      {
         for(Core::FastU i = 0; i != n; ++i)
         {
            putStmntPushArg(stmnt->args[1], i);
            putStmntPushArg(stmnt->args[2], i);
            putCode(code);
         }
      }
   }

   //
   // Info::putStmnt_BNot
   //
   void Info::putStmnt_BNot()
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return;

      if(n == 1)
         return putCode("BNot");

      bool stk0 = stmnt->args[0].a == IR::ArgBase::Stk;
      bool stk1 = stmnt->args[1].a == IR::ArgBase::Stk;

      if(stk0 && stk1)
      {
         for(auto i = n - 1; i--;)
            putCode("BNot"), putStmntDropTmp(i);
         putCode("BNot");
         putStmntPushTmp(0, n - 1);
      }
      else
      {
         for(Core::FastU i = 0; i != n; ++i)
         {
            if(!stk1) putStmntPushArg(stmnt->args[1], i);
            putCode("BNot");
            if(!stk0) putStmntDropArg(stmnt->args[0], i);
         }
      }
   }

   //
   // Info::putStmnt_Bclz
   //
   void Info::putStmnt_Bclz(char const *code)
   {
      auto n = getStmntSizeW();

      if(n == 0)
         return putCode("Push_Lit", 0);

      if(n == 1)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->code, n), n);
   }

   //
   // Info::trStmnt_BAnd
   //
   void Info::trStmnt_BAnd()
   {
      auto n = getStmntSizeW();

      if(n <= 1)
         trStmntStk3(false);

      CheckArgC(stmnt, 3);

      if(!isPushArg(stmnt->args[1]) || !isPushArg(stmnt->args[2]))
      {
         trStmntStk3(false);
         trStmntTmp(n * 2 - 1);
      }
      else
         moveArgStk_dst(stmnt->args[0]);
   }

   //
   // Info::trStmnt_BNot
   //
   void Info::trStmnt_BNot()
   {
      auto n = getStmntSizeW();

      if(n <= 1)
         trStmntStk2();

      CheckArgC(stmnt, 2);

      bool stk0, stk1;

      if((stk0 = !isDropArg(stmnt->args[0])))
         moveArgStk_dst(stmnt->args[0]);

      if((stk1 = !isPushArg(stmnt->args[1])))
         moveArgStk_src(stmnt->args[1]);

      if(stk0 && stk1)
         trStmntTmp(n - 1);
   }

   //
   // Info::trStmnt_Bclz
   //
   void Info::trStmnt_Bclz()
   {
      trStmntStk2();
   }
}

// EOF

