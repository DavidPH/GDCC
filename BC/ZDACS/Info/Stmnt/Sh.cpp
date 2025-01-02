//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2024 David Hill
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
#include "BC/ZDACS/Module.hpp"

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

   GDCC_BC_CodeTypeSwitchFn(tr, ShL, FIU)
   GDCC_BC_CodeTypeSwitchFn(tr, ShR, FIU)

   //
   // Info::genStmnt_ShL_F
   //
   void Info::genStmnt_ShL_F()
   {
      auto n = getStmntSize();

      if(n == 0)
         return genCode(Code::Drop_Nul);

      genStmntCall(getFuncName(stmnt->code, n), n);
   }

   //
   // Info::genStmnt_ShL_U
   //
   void Info::genStmnt_ShL_U()
   {
      auto n = getStmntSize();

      if(n == 0)
         return genCode(Code::Drop_Nul);

      if(n == 1)
         return genCode(Code::ShLU);

      if(stmnt->args[2].a != IR::ArgBase::Lit)
         return genStmntCall(getFuncName(IR::CodeBase::ShL+'U', n), n);

      // TODO 2024-12-30: Only do this if expression can be resolved.
      Core::FastU shift = getWord(stmnt->args[2].aLit) % (32 * n);

      // Shift of 0 is a no-op.
      if(!shift) return;

      Core::FastU shiftBits  = shift % 32;
      Core::FastU shiftWords = shift / 32;
      Core::FastU keepWords  = n - shiftWords;

      for(Core::FastU i = shiftWords; i--;)
         genCode(Code::Drop_Nul);

      for(Core::FastU i = keepWords; i--;)
         genStmntDropTmp(i);

      for(Core::FastU i = shiftWords; i--;)
         genCode(Code::Push_Lit, 0);

      if(shiftBits)
      {
         // ret[0] = l[0] << r
         genStmntPushTmp(0);
         genCode(Code::Push_Lit,    shiftBits);
         genCode(Code::ShLU);

         for(Core::FastU i = 1; i != keepWords; ++i)
         {
            // ret[i] = (l[i] << r) | (l[i-1] >> (32 - r))

            genStmntPushTmp(i);
            genCode(Code::Push_Lit, shiftBits);
            genCode(Code::ShLU);

            genStmntPushTmp(i - 1);
            genStmntShiftRU(32 - shiftBits);

            genCode(Code::BOrI);
         }
      }
      else
      {
         // ret[i] = l[i]
         for(Core::FastU i = 0; i != keepWords; ++i)
            genStmntPushTmp(i);
      }
   }

   //
   // Info::genStmnt_ShR_I
   //
   void Info::genStmnt_ShR_I()
   {
      auto n = getStmntSize();

      if(n == 0)
         return genCode(Code::Drop_Nul);

      if(n == 1)
         return genCode(Code::ShRI);

      if(stmnt->args[2].a != IR::ArgBase::Lit)
         return genStmntCall(getFuncName(IR::CodeBase::ShR+'I', n), n);

      // TODO 2024-12-30: Only do this if expression can be resolved.
      Core::FastU shift = getWord(stmnt->args[2].aLit) % (32 * n);

      // Shift of 0 is a no-op.
      if(!shift) return;

      Core::FastU shiftBits  = shift % 32;
      Core::FastU shiftWords = shift / 32;
      Core::FastU keepWords  = n - shiftWords;

      for(Core::FastU i = keepWords; i--;)
         genStmntDropTmp(i);

      for(Core::FastU i = shiftWords; i--;)
         genCode(Code::Drop_Nul);

      if(shiftBits)
      {
         for(Core::FastU i = 0; i != keepWords - 1; ++i)
         {
            // ret[i] = (l[i] >> r) | (l[i+1] << (32 - r))

            genStmntPushTmp(i);
            genStmntShiftRU(shiftBits);

            genStmntPushTmp(i + 1);
            genCode(Code::Push_Lit, 32 - shiftBits);
            genCode(Code::ShLU);

            genCode(Code::BOrI);
         }

         // ret[N-1] = l[N-1] >> r
         genStmntPushTmp(keepWords - 1);
         genCode(Code::Push_Lit, shiftBits);
         genCode(Code::ShRI);
      }
      else
      {
         // ret[i] = l[i]
         for(Core::FastU i = 0; i != keepWords; ++i)
            genCode(Code::Push_LocReg, func->localReg + i);
      }

      if(shiftWords)
      {
         genStmntPushTmp(keepWords - 1);
         genCode(Code::Push_Lit, 31);
         genCode(Code::ShRI);

         for(Core::FastU i = shiftWords - 1; i--;)
            genCode(Code::Copy);
      }
   }

   //
   // Info::genStmnt_ShR_U
   //
   void Info::genStmnt_ShR_U()
   {
      auto n = getStmntSize();

      if(n == 0)
         return genCode(Code::Drop_Nul);

      if(n == 1)
         return genStmnt_ShR_U1();

      if(stmnt->args[2].a != IR::ArgBase::Lit)
         return genStmntCall(getFuncName(IR::CodeBase::ShR+'U', n), n);

      // TODO 2024-12-30: Only do this if expression can be resolved.
      Core::FastU shift = getWord(stmnt->args[2].aLit) % (32 * n);

      // Shift of 0 is a no-op.
      if(!shift) return;

      Core::FastU shiftBits  = shift % 32;
      Core::FastU shiftWords = shift / 32;
      Core::FastU keepWords  = n - shiftWords;

      for(Core::FastU i = keepWords; i--;)
         genStmntDropTmp(i);

      for(Core::FastU i = shiftWords; i--;)
         genCode(Code::Drop_Nul);

      if(shiftBits)
      {
         for(Core::FastU i = 0; i != keepWords - 1; ++i)
         {
            // ret[i] = (l[i] >> r) | (l[i+1] << (32 - r))

            genStmntPushTmp(i);
            genStmntShiftRU(shiftBits);

            genStmntPushTmp(i + 1);
            genCode(Code::Push_Lit, 32 - shiftBits);
            genCode(Code::ShLU);

            genCode(Code::BOrI);
         }

         // ret[N-1] = l[N-1] >> r
         genStmntPushTmp(keepWords - 1);
         genStmntShiftRU(shiftBits);
      }
      else
      {
         // ret[i] = l[i]
         for(Core::FastU i = 0; i != keepWords; ++i)
            genStmntPushTmp(i);
      }

      for(Core::FastU i = shiftWords; i--;)
         genCode(Code::Push_Lit, 0);
   }

   //
   // Info::genStmnt_ShR_U1
   //
   void Info::genStmnt_ShR_U1()
   {
      if(stmnt->args[2].a == IR::ArgBase::Lit)
      {
         genStmntShiftRU(getWord(stmnt->args[2].aLit));
      }
      else if(stmnt->args[2].a == IR::ArgBase::Stk)
      {
         // If shift is 0, jump to end.
         auto jump = module->chunkCODE.size();
         genCode(Code::Jcnd_Lit, 0, 0);

         genStmntDropTmp(0);
         genStmntPushTmp(0);
         genCode(Code::ShRI);
         genCode(Code::Push_Lit, 0x80000000);
         genStmntDecUTmp(0);
         genStmntPushTmp(0);
         genCode(Code::ShRI);
         genCode(Code::BNot);
         genCode(Code::BAnd);

         module->chunkCODE[jump].args[1] = getCodePos();
      }
      else
      {
         genStmntPushArg(stmnt->args[2], 0);

         // If shift is 0, jump to end.
         auto jump = module->chunkCODE.size();
         genCode(Code::Jcnd_Lit, 0, 0);

         genCode(Code::ShRI);
         genCode(Code::Push_Lit, 0x80000000);
         genStmntPushArg(stmnt->args[2], 0);
         genCode(Code::Push_Lit, 1);
         genCode(Code::SubU);
         genCode(Code::ShRI);
         genCode(Code::BNot);
         genCode(Code::BAnd);

         module->chunkCODE[jump].args[1] = getCodePos();
      }
   }

   //
   // Info::genStmntShiftRU
   //
   void Info::genStmntShiftRU(Core::FastU shift)
   {
      genCode(Code::Push_Lit, shift % 32);
      genCode(Code::ShRI);
      genCode(Code::Push_Lit, 0xFFFFFFFF >> (shift % 32));
      genCode(Code::BAnd);
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

