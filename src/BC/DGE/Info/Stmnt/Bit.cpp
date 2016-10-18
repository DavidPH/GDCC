//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
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
   // Info::preStmnt_Bclo_W
   //
   void Info::preStmnt_Bclo_W()
   {
      if(stmnt->op.size <= 1)
         return;

      addFunc_Bclo_W(stmnt->op.size);
   }

   //
   // Info::preStmnt_Bclz_W
   //
   void Info::preStmnt_Bclz_W()
   {
      if(stmnt->op.size <= 1)
         return;

      addFunc_Bclz_W(stmnt->op.size);
   }

   //
   // Info::putStmnt_BAnd_W
   //
   void Info::putStmnt_BAnd_W(char const *code)
   {
      if(stmnt->op.size == 0)
         return;

      if(stmnt->op.size == 1)
         return putCode(code);

      if(stmnt->args[1].a == IR::ArgBase::Stk)
      {
         putStmntDropTmp(0, stmnt->op.size * 2 - 1);

         putStmntPushTmp(stmnt->op.size - 1);
         putCode(code);

         for(Core::FastU i = 0, e = stmnt->op.size - 1; i != e; ++i)
         {
            putStmntPushTmp(i + stmnt->op.size);
            putStmntPushTmp(i);
            putCode(code);
         }
      }
      else
      {
         for(Core::FastU i = 0; i != stmnt->op.size; ++i)
         {
            putStmntPushArg(stmnt->args[1], i);
            putStmntPushArg(stmnt->args[2], i);
            putCode(code);
         }
      }
   }

   //
   // Info::putStmnt_BNot_W
   //
   void Info::putStmnt_BNot_W()
   {
      if(stmnt->op.size == 0)
         return;

      if(stmnt->op.size == 1)
         return putCode("BNot");

      bool stk0 = stmnt->args[0].a == IR::ArgBase::Stk;
      bool stk1 = stmnt->args[1].a == IR::ArgBase::Stk;

      if(stk0 && stk1)
      {
         for(auto i = stmnt->op.size - 1; i--;)
            putCode("BNot"), putStmntDropTmp(i);
         putCode("BNot");
         putStmntPushTmp(0, stmnt->op.size - 1);
      }
      else
      {
         for(Core::FastU i = 0; i != stmnt->op.size; ++i)
         {
            if(!stk1) putStmntPushArg(stmnt->args[1], i);
            putCode("BNot");
            if(!stk0) putStmntDropArg(stmnt->args[0], i);
         }
      }
   }

   //
   // Info::putStmnt_Bclz_W
   //
   void Info::putStmnt_Bclz_W(char const *code)
   {
      if(stmnt->op.size == 0)
         return putCode("Push_Lit", 0);

      if(stmnt->op.size == 1)
         return putCode(code);

      putStmntCall(getFuncName(stmnt->op), stmnt->op.size);
   }

   //
   // Info::trStmnt_BAnd_W
   //
   void Info::trStmnt_BAnd_W()
   {
      if(stmnt->op.size <= 1)
         trStmntStk3(stmnt->op.size, stmnt->op.size, false);

      CheckArgC(stmnt, 3);

      if(!isPushArg(stmnt->args[1]) || !isPushArg(stmnt->args[2]))
      {
         trStmntStk3(stmnt->op.size, stmnt->op.size, false);
         trStmntTmp(stmnt->op.size * 2 - 1);
      }
      else
         moveArgStk_dst(stmnt->args[0], stmnt->op.size);
   }

   //
   // Info::trStmnt_BNot_W
   //
   void Info::trStmnt_BNot_W()
   {
      if(stmnt->op.size <= 1)
         trStmntStk2(stmnt->op.size, stmnt->op.size);

      CheckArgC(stmnt, 2);

      bool stk0, stk1;

      if((stk0 = !isDropArg(stmnt->args[0])))
         moveArgStk_dst(stmnt->args[0], stmnt->op.size);

      if((stk1 = !isPushArg(stmnt->args[1])))
         moveArgStk_src(stmnt->args[1], stmnt->op.size);

      if(stk0 && stk1)
         trStmntTmp(stmnt->op.size - 1);
   }

   //
   // Info::trStmnt_Bclz_W
   //
   void Info::trStmnt_Bclz_W()
   {
      trStmntStk2(1, stmnt->op.size);
   }
}

// EOF

