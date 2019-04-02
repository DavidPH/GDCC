//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Negate statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "Core/Exception.hpp"

#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   GDCC_BC_CodeTypeSwitchFn(gen, Neg, FIU)

   GDCC_BC_CodeTypeSwitchFn(put, Neg, FIU)

   GDCC_BC_CodeTypeSwitchFn(tr, Neg, FIU)

   //
   // Info::genStmnt_Neg_F
   //
   void Info::genStmnt_Neg_F()
   {
      numChunkCODE += 12;
   }

   //
   // Info::genStmnt_Neg_I
   //
   void Info::genStmnt_Neg_I()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
      {
         numChunkCODE += 4;

         if(stmnt->args[0].a != IR::ArgBase::Stk)
            numChunkCODE += lenDropArg(stmnt->args[0], 0);

         return;
      }

      if(stmnt->args[0].a == IR::ArgBase::Stk)
      {
         numChunkCODE += (n - 1) * 24 + 8 +
            n * 20;
      }
      else
      {
         numChunkCODE += (n - 1) * 12 + 4 +
            lenDropArg(stmnt->args[0], 0, n) +
            lenIncUArg(stmnt->args[0], 0, n) +
            lenPushArg(stmnt->args[0], 0, n - 1);
      }
   }

   //
   // Info::putStmnt_Neg_F
   //
   void Info::putStmnt_Neg_F()
   {
      putCode(Code::Push_Lit, 0x80000000);
      putCode(Code::BOrX);
   }

   //
   // Info::putStmnt_Neg_I
   //
   void Info::putStmnt_Neg_I()
   {
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(n == 1)
      {
         putCode(Code::NegI);

         if(stmnt->args[0].a != IR::ArgBase::Stk)
            putStmntDropArg(stmnt->args[0], 0);

         return;
      }

      if(stmnt->args[0].a == IR::ArgBase::Stk)
      {
         for(auto i = n; i--;)
         {
            putCode(Code::BNot);
            putCode(Code::Drop_LocReg, func->localReg + i);
         }

         putCode(Code::IncU_LocReg, func->localReg + 0);

         for(Core::FastU i = 0, e = n - 1; i != e; ++i)
         {
            putCode(Code::Push_LocReg, func->localReg + i);
            putCode(Code::Jcnd_Tru,    putPos + (e - i) * 24 - 8);
            putCode(Code::IncU_LocReg, func->localReg + i + 1);
         }

         for(Core::FastU i = 0; i != n; ++i)
            putCode(Code::Push_LocReg, func->localReg + i);
      }
      else
      {
         for(auto i = n; i--;)
         {
            putCode(Code::BNot);
            putStmntDropArg(stmnt->args[0], i);
         }

         putStmntIncUArg(stmnt->args[0], 0);

         for(Core::FastU i = 0, e = n - 1; i != e; ++i)
         {
            Core::FastU jump = (e - i) * 8 + // Jcnd_Tru
               lenIncUArg(stmnt->args[0], i + 1, e + 1) +
               lenPushArg(stmnt->args[0], i + 1, e);

            putStmntPushArg(stmnt->args[0], i);
            putCode(Code::Jcnd_Tru, putPos + jump);
            putStmntIncUArg(stmnt->args[0], i + 1);
         }
      }
   }

   //
   // Info::trStmnt_Neg_I
   //
   void Info::trStmnt_Neg_I()
   {
      auto n = getStmntSize();

      if(!isCopyArg(stmnt->args[0]) || !isFastArg(stmnt->args[0]))
      {
         if(n > 1)
            func->setLocalTmp(n);

         moveArgStk_dst(stmnt->args[0]);
      }

      moveArgStk_src(stmnt->args[1]);
   }
}

// EOF

