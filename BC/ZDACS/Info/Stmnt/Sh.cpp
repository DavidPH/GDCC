//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Shift statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "IR/Exception.hpp"
#include "IR/Function.hpp"
#include "IR/Glyph.hpp"

#include <sstream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   GDCC_BC_CodeTypeSwitchFn(gen, ShL, FIU)
   GDCC_BC_CodeTypeSwitchFn(gen, ShR, FIU)

   GDCC_BC_CodeTypeSwitchFn(pre, ShL, FIU)
   GDCC_BC_CodeTypeSwitchFn(pre, ShR, FIU)

   GDCC_BC_CodeTypeSwitchFn(put, ShL, FIU)
   GDCC_BC_CodeTypeSwitchFn(put, ShR, FIU)

   GDCC_BC_CodeTypeSwitchFn(tr, ShL, FIU)
   GDCC_BC_CodeTypeSwitchFn(tr, ShR, FIU)

   //
   // Info::genStmnt_ShL_F
   //
   void Info::genStmnt_ShL_F()
   {
      auto n = getStmntSize();

      if(n == 0)
         return (void)(numChunkCODE += 4);

      genStmntCall(n);
   }

   //
   // Info::genStmnt_ShL_U
   //
   void Info::genStmnt_ShL_U()
   {
      auto n = getStmntSize();

      if(n <= 1)
         return (void)(numChunkCODE += 4);

      if(stmnt->args[2].a != IR::ArgBase::Lit)
         return genStmntCall(n);

      Core::FastU shift = getWord(stmnt->args[2].aLit) % (32 * n);

      // Shift of 0 is a no-op.
      if(!shift) return;

      Core::FastU shiftWords = shift / 32;
      Core::FastU keepWords  = n - shiftWords;

      numChunkCODE += shiftWords * 12 + keepWords * 8;

      if(shift % 32)
         numChunkCODE += 20 + (keepWords - 1) * 56;
      else
         numChunkCODE += keepWords * 8;
   }

   //
   // Info::genStmnt_ShR_I
   //
   void Info::genStmnt_ShR_I()
   {
      auto n = getStmntSize();

      if(n <= 1)
         return (void)(numChunkCODE += 4);

      if(stmnt->args[2].a != IR::ArgBase::Lit)
         return genStmntCall(n);

      Core::FastU shift = getWord(stmnt->args[2].aLit) % (32 * n);

      // Shift of 0 is a no-op.
      if(!shift) return;

      Core::FastU shiftWords = shift / 32;
      Core::FastU keepWords  = n - shiftWords;

      numChunkCODE += shiftWords * 4 + keepWords * 8;

      if(shift % 32)
         numChunkCODE += (keepWords - 1) * 56 + 20;
      else
         numChunkCODE += keepWords * 8;

      if(shiftWords)
         numChunkCODE += 20 + (shiftWords - 1) * 4;
   }

   //
   // Info::genStmnt_ShR_U
   //
   void Info::genStmnt_ShR_U()
   {
      auto n = getStmntSize();

      if(n == 0)
         return (void)(numChunkCODE += 4);

      if(n == 1)
         return genStmnt_ShR_U1();

      if(stmnt->args[2].a != IR::ArgBase::Lit)
         return genStmntCall(n);

      Core::FastU shift = getWord(stmnt->args[2].aLit) % (32 * n);

      // Shift of 0 is a no-op.
      if(!shift) return;

      Core::FastU shiftWords = shift / 32;
      Core::FastU keepWords  = n - shiftWords;

      numChunkCODE += shiftWords * 4 + keepWords * 8;

      if(shift % 32)
         numChunkCODE += (keepWords - 1) * 56 + 32;
      else
         numChunkCODE += keepWords * 8;

      numChunkCODE += shiftWords * 8;
   }

   //
   // Info::genStmnt_ShR_U1
   //
   void Info::genStmnt_ShR_U1()
   {
      if(stmnt->args[2].a == IR::ArgBase::Lit)
         numChunkCODE += 24;
      else if(stmnt->args[2].a == IR::ArgBase::Stk)
         numChunkCODE += 68;
      else
         numChunkCODE += 48 + lenPushArg(stmnt->args[2], 0) * 2;
   }

   //
   // Info::putStmnt_ShL_F
   //
   void Info::putStmnt_ShL_F()
   {
      auto n = getStmntSize();

      if(n == 0)
         return putCode(Code::Drop_Nul);

      putStmntCall(getFuncName(stmnt->code, n), n);
   }

   //
   // Info::putStmnt_ShL_U
   //
   void Info::putStmnt_ShL_U()
   {
      auto n = getStmntSize();

      if(n == 0)
         return putCode(Code::Drop_Nul);

      if(n == 1)
         return putCode(Code::ShLU);

      if(stmnt->args[2].a != IR::ArgBase::Lit)
         return putStmntCall(getFuncName(IR::CodeBase::ShL+'U', n), n);

      Core::FastU shift = getWord(stmnt->args[2].aLit) % (32 * n);

      // Shift of 0 is a no-op.
      if(!shift) return;

      Core::FastU shiftBits  = shift % 32;
      Core::FastU shiftWords = shift / 32;
      Core::FastU keepWords  = n - shiftWords;

      for(Core::FastU i = shiftWords; i--;)
         putCode(Code::Drop_Nul);

      for(Core::FastU i = keepWords; i--;)
         putCode(Code::Drop_LocReg, func->localReg + i);

      for(Core::FastU i = shiftWords; i--;)
         putCode(Code::Push_Lit, 0);

      if(shiftBits)
      {
         // ret[0] = l[0] << r
         putCode(Code::Push_LocReg, func->localReg + 0);
         putCode(Code::Push_Lit,    shiftBits);
         putCode(Code::ShLU);

         for(Core::FastU i = 1; i != keepWords; ++i)
         {
            // ret[i] = (l[i] << r) | (l[i-1] >> (32 - r))

            putCode(Code::Push_LocReg, func->localReg + i);
            putCode(Code::Push_Lit,    shiftBits);
            putCode(Code::ShLU);

            putCode(Code::Push_LocReg, func->localReg + i - 1);
            putStmntShiftRU(32 - shiftBits);

            putCode(Code::BOrI);
         }
      }
      else
      {
         // ret[i] = l[i]
         for(Core::FastU i = 0; i != keepWords; ++i)
            putCode(Code::Push_LocReg, func->localReg + i);
      }
   }

   //
   // Info::putStmnt_ShR_I
   //
   void Info::putStmnt_ShR_I()
   {
      auto n = getStmntSize();

      if(n == 0)
         return putCode(Code::Drop_Nul);

      if(n == 1)
         return putCode(Code::ShRI);

      if(stmnt->args[2].a != IR::ArgBase::Lit)
         return putStmntCall(getFuncName(IR::CodeBase::ShR+'I', n), n);

      Core::FastU shift = getWord(stmnt->args[2].aLit) % (32 * n);

      // Shift of 0 is a no-op.
      if(!shift) return;

      Core::FastU shiftBits  = shift % 32;
      Core::FastU shiftWords = shift / 32;
      Core::FastU keepWords  = n - shiftWords;

      for(Core::FastU i = keepWords; i--;)
         putCode(Code::Drop_LocReg, func->localReg + i);

      for(Core::FastU i = shiftWords; i--;)
         putCode(Code::Drop_Nul);

      if(shiftBits)
      {
         for(Core::FastU i = 0; i != keepWords - 1; ++i)
         {
            // ret[i] = (l[i] >> r) | (l[i+1] << (32 - r))

            putCode(Code::Push_LocReg, func->localReg + i);
            putStmntShiftRU(shiftBits);

            putCode(Code::Push_LocReg, func->localReg + i + 1);
            putCode(Code::Push_Lit,    32 - shiftBits);
            putCode(Code::ShLU);

            putCode(Code::BOrI);
         }

         // ret[N-1] = l[N-1] >> r
         putCode(Code::Push_LocReg, func->localReg + keepWords - 1);
         putCode(Code::Push_Lit,    shiftBits);
         putCode(Code::ShRI);
      }
      else
      {
         // ret[i] = l[i]
         for(Core::FastU i = 0; i != keepWords; ++i)
            putCode(Code::Push_LocReg, func->localReg + i);
      }

      if(shiftWords)
      {
         putCode(Code::Push_LocReg, func->localReg + keepWords - 1);
         putCode(Code::Push_Lit,    31);
         putCode(Code::ShRI);

         for(Core::FastU i = shiftWords - 1; i--;)
            putCode(Code::Copy);
      }
   }

   //
   // Info::putStmnt_ShR_U
   //
   void Info::putStmnt_ShR_U()
   {
      auto n = getStmntSize();

      if(n == 0)
         return putCode(Code::Drop_Nul);

      if(n == 1)
         return putStmnt_ShR_U1();

      if(stmnt->args[2].a != IR::ArgBase::Lit)
         return putStmntCall(getFuncName(IR::CodeBase::ShR+'U', n), n);

      Core::FastU shift = getWord(stmnt->args[2].aLit) % (32 * n);

      // Shift of 0 is a no-op.
      if(!shift) return;

      Core::FastU shiftBits  = shift % 32;
      Core::FastU shiftWords = shift / 32;
      Core::FastU keepWords  = n - shiftWords;

      for(Core::FastU i = keepWords; i--;)
         putCode(Code::Drop_LocReg, func->localReg + i);

      for(Core::FastU i = shiftWords; i--;)
         putCode(Code::Drop_Nul);

      if(shiftBits)
      {
         for(Core::FastU i = 0; i != keepWords - 1; ++i)
         {
            // ret[i] = (l[i] >> r) | (l[i+1] << (32 - r))

            putCode(Code::Push_LocReg, func->localReg + i);
            putStmntShiftRU(shiftBits);

            putCode(Code::Push_LocReg, func->localReg + i + 1);
            putCode(Code::Push_Lit,    32 - shiftBits);
            putCode(Code::ShLU);

            putCode(Code::BOrI);
         }

         // ret[N-1] = l[N-1] >> r
         putCode(Code::Push_LocReg, func->localReg + keepWords - 1);
         putStmntShiftRU(shiftBits);
      }
      else
      {
         // ret[i] = l[i]
         for(Core::FastU i = 0; i != keepWords; ++i)
            putCode(Code::Push_LocReg, func->localReg + i);
      }

      for(Core::FastU i = shiftWords; i--;)
         putCode(Code::Push_Lit, 0);
   }

   //
   // Info::putStmnt_ShR_U1
   //
   void Info::putStmnt_ShR_U1()
   {
      if(stmnt->args[2].a == IR::ArgBase::Lit)
      {
         putStmntShiftRU(getWord(stmnt->args[2].aLit));
      }
      else if(stmnt->args[2].a == IR::ArgBase::Stk)
      {
         // If shift is 0, jump to end.
         putCode(Code::Jcnd_Lit);
         putWord(0);
         putWord(putPos + 60);

         putCode(Code::Drop_LocReg);
         putWord(func->localReg + 0);
         putCode(Code::Push_LocReg);
         putWord(func->localReg + 0);
         putCode(Code::ShRI);
         putCode(Code::Push_Lit);
         putWord(0x80000000);
         putCode(Code::DecU_LocReg);
         putWord(func->localReg + 0);
         putCode(Code::Push_LocReg);
         putWord(func->localReg + 0);
         putCode(Code::ShRI);
         putCode(Code::BNot);
         putCode(Code::BAnd);
      }
      else
      {
         putStmntPushArg(stmnt->args[2], 0);

         // If shift is 0, jump to end.
         putCode(Code::Jcnd_Lit);
         putWord(0);
         putWord(putPos + 40 + lenPushArg(stmnt->args[2], 0));

         putCode(Code::ShRI);
         putCode(Code::Push_Lit, 0x80000000);
         putStmntPushArg(stmnt->args[2], 0);
         putCode(Code::Push_Lit, 1);
         putCode(Code::SubU);
         putCode(Code::ShRI);
         putCode(Code::BNot);
         putCode(Code::BAnd);
      }
   }

   //
   // Info::putStmntShiftRU
   //
   void Info::putStmntShiftRU(Core::FastU shift)
   {
      putCode(Code::Push_Lit, shift % 32);
      putCode(Code::ShRI);
      putCode(Code::Push_Lit, 0xFFFFFFFF >> (shift % 32));
      putCode(Code::BAnd);
   }

   //
   // Info::trStmnt_ShL_F
   //
   void Info::trStmnt_ShL_F()
   {
      auto n = getStmntSize();

      if(!trStmntShift(true))
         return;

      if(n <= 2)
         return;

      func->setLocalTmp(1);
   }

   //
   // Info::trStmnt_ShL_U
   //
   void Info::trStmnt_ShL_U()
   {
      auto n = getStmntSize();

      if(n <= 1)
         return (void)trStmntShift(true);

      if(!trStmntShift(false))
         return;

      Core::FastU shift = getWord(stmnt->args[2].aLit) % (32 * n);

      if(shift)
         func->setLocalTmp(n - shift / 32);
   }

   //
   // Info::trStmnt_ShR_I
   //
   void Info::trStmnt_ShR_I()
   {
      auto n = getStmntSize();

      if(n <= 1)
         return (void)trStmntShift(true);

      if(!trStmntShift(false))
         return;

      Core::FastU shift = getWord(stmnt->args[2].aLit) % (32 * n);

      if(shift)
         func->setLocalTmp(n - shift / 32);
   }

   //
   // Info::trStmnt_ShR_U
   //
   void Info::trStmnt_ShR_U()
   {
      auto n = getStmntSize();

      if(n == 0)
         return (void)trStmntShift(true);

      if(n == 1)
         return trStmnt_ShR_U1();

      if(!trStmntShift(false))
         return;

      Core::FastU shift = getWord(stmnt->args[2].aLit) % (32 * n);

      if(shift)
         func->setLocalTmp(n - shift / 32);
   }

   //
   // Info::trStmnt_ShR_U1
   //
   void Info::trStmnt_ShR_U1()
   {
      if(stmnt->args[1].a != IR::ArgBase::Stk &&
         stmnt->args[2].a == IR::ArgBase::Stk)
         IR::ErrorCode(stmnt, "disorder");

      moveArgStk_dst(stmnt->args[0]);
      moveArgStk_src(stmnt->args[1]);

      if(!isPushArg(stmnt->args[2]) || !isFastArg(stmnt->args[2]))
      {
         func->setLocalTmp(1);
         moveArgStk_src(stmnt->args[2]);
      }
   }
}

// EOF

