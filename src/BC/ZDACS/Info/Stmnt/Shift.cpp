//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
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

#include "Core/Exception.hpp"

#include "IR/Function.hpp"
#include "IR/Glyph.hpp"

#include <sstream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      namespace ZDACS
      {
         //
         // Info::genStmnt_ShLF_W
         //
         void Info::genStmnt_ShLF_W()
         {
            if(stmnt->op.size == 0)
            {
               numChunkCODE += 4;
               return;
            }

            if(stmnt->op.size == 1)
            {
               genStmntCall(1);
               return;
            }

            genStmntCall(2);

            if(stmnt->op.size == 2)
               return;

            numChunkCODE += 8 + 16 + (stmnt->op.size - 1) * 12;
         }

         //
         // Info::genStmnt_ShLU_W
         //
         void Info::genStmnt_ShLU_W()
         {
            if(stmnt->op.size <= 1)
            {
               numChunkCODE += 4;
               return;
            }

            if(stmnt->args[2].a != IR::ArgBase::Lit)
            {
               genStmntCall(stmnt->op.size);
               return;
            }

            Core::FastU shift = GetWord(stmnt->args[2].aLit) % (32 * stmnt->op.size);

            // Shift of 0 is a no-op.
            if(!shift) return;

            Core::FastU shiftWords = shift / 32;
            Core::FastU keepWords  = stmnt->op.size - shiftWords;

            numChunkCODE += shiftWords * 12 + keepWords * 8;

            if(shift % 32)
               numChunkCODE += 20 + (keepWords - 1) * 56;
            else
               numChunkCODE += keepWords * 8;
         }

         //
         // Info::genStmnt_ShRI_W
         //
         void Info::genStmnt_ShRI_W()
         {
            if(stmnt->op.size <= 1)
            {
               numChunkCODE += 4;
               return;
            }

            if(stmnt->args[2].a != IR::ArgBase::Lit)
            {
               genStmntCall(stmnt->op.size);
               return;
            }

            Core::FastU shift = GetWord(stmnt->args[2].aLit) % (32 * stmnt->op.size);

            // Shift of 0 is a no-op.
            if(!shift) return;

            Core::FastU shiftWords = shift / 32;
            Core::FastU keepWords  = stmnt->op.size - shiftWords;

            numChunkCODE += shiftWords * 4 + keepWords * 8;

            if(shift % 32)
               numChunkCODE += (keepWords - 1) * 56 + 20;
            else
               numChunkCODE += keepWords * 8;

            if(shiftWords)
               numChunkCODE += 20 + (shiftWords - 1) * 4;
         }

         //
         // Info::genStmnt_ShRU_W
         //
         void Info::genStmnt_ShRU_W()
         {
            if(stmnt->op.size == 0)
            {
               numChunkCODE += 4;
               return;
            }

            if(stmnt->op.size == 1)
            {
               genStmnt_ShRU_W1();
               return;
            }

            if(stmnt->args[2].a != IR::ArgBase::Lit)
            {
               genStmntCall(stmnt->op.size);
               return;
            }

            Core::FastU shift = GetWord(stmnt->args[2].aLit) % (32 * stmnt->op.size);

            // Shift of 0 is a no-op.
            if(!shift) return;

            Core::FastU shiftWords = shift / 32;
            Core::FastU keepWords  = stmnt->op.size - shiftWords;

            numChunkCODE += shiftWords * 4 + keepWords * 8;

            if(shift % 32)
               numChunkCODE += (keepWords - 1) * 56 + 32;
            else
               numChunkCODE += keepWords * 8;

            numChunkCODE += shiftWords * 8;
         }

         //
         // Info::genStmnt_ShRU_W1
         //
         void Info::genStmnt_ShRU_W1()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
               numChunkCODE += 24;
            else if(stmnt->args[2].a == IR::ArgBase::Stk)
               numChunkCODE += 68;
            else
               numChunkCODE += 48 + lenPushArg(stmnt->args[2], 0) * 2;
         }

         //
         // Info::preStmnt_ShLF_W
         //
         void Info::preStmnt_ShLF_W()
         {
            if(stmnt->op.size == 0)
               return;

            Core::FastU retWords = stmnt->op.size == 1 ? 1 : 2;
            Core::FastU lopWords = stmnt->op.size == 2 ? 2 : 1;

            Core::String name = getCallName();
            auto newFunc = preStmntCallDef(name, retWords, lopWords + 1,
               lopWords + 1, __FILE__, __LINE__);

            if(!newFunc)
               return;

            bool left = stmnt->op.code == IR::Code::ShLF_W;

            FloatInfo fi = GetFloatInfo(stmnt->op.size);

            IR::Glyph labelEMax{prog, name + "$emax"};
            IR::Glyph labelInf {prog, name + "$inf"};

            IR::Arg_LocReg exp{IR::Arg_Lit(newFunc->block.getExp(lopWords - 1))};
            IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))};
            IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(lopWords))};

            IR::Arg_Stk stk{};

            #define AS_Stmnt newFunc->block.addStatementArgs

            if(!left)
               AS_Stmnt({IR::Code::ShLU_W, 1}, rop, rop, 31 - fi.bitsExp);

            AS_Stmnt({IR::Code::BAnd_W,   1}, stk, exp, fi.maskExp);
            AS_Stmnt({IR::Code::Jcnd_Tab, 1}, stk, fi.maskExp, labelEMax, 0, labelEMax);

            if(left)
            {
               AS_Stmnt({IR::Code::ShRI_W, 1}, stk, stk, 31 - fi.bitsExp);
               AS_Stmnt({IR::Code::AddU_W, 1}, rop, rop, stk);

               AS_Stmnt({IR::Code::CmpI_GE_W, 1}, stk, rop, fi.maxExp);
               AS_Stmnt({IR::Code::Jcnd_Tru,  1}, stk, labelInf);

               if(lopWords > 1)
                  AS_Stmnt({IR::Code::Move_W, lopWords - 1}, stk, lop);

               AS_Stmnt({IR::Code::BAnd_W, 1}, stk, exp, ~fi.maskExp);
               AS_Stmnt({IR::Code::ShLU_W, 1}, stk, rop, 31 - fi.bitsExp);
               AS_Stmnt({IR::Code::BOrI_W, 1}, stk, stk, stk);
            }
            else
            {
               AS_Stmnt({IR::Code::CmpI_LE_W, 1}, stk, stk, rop);
               AS_Stmnt({IR::Code::Jcnd_Tru,  1}, stk, labelInf);

               if(lopWords > 1)
                  AS_Stmnt({IR::Code::Move_W, lopWords - 1}, stk, lop);

               AS_Stmnt({IR::Code::SubU_W, 1}, stk, exp, rop);
            }

            if(lopWords < retWords)
               AS_Stmnt({IR::Code::Move_W, retWords - lopWords}, stk, 0);

            AS_Stmnt({IR::Code::Retn, retWords}, stk);

            // Return INF or zero.
            newFunc->block.addLabel(labelInf);

            if(lopWords > 1)
               AS_Stmnt({IR::Code::Move_W, lopWords - 1}, stk, 0);

            AS_Stmnt({IR::Code::BAnd_W, 1}, stk, exp, 0x80000000);

            if(left)
               AS_Stmnt({IR::Code::BOrI_W, 1}, stk, stk, fi.maskExp);

            if(lopWords < retWords)
               AS_Stmnt({IR::Code::Move_W, 1}, stk, 1);

            AS_Stmnt({IR::Code::Retn, retWords}, stk);

            // If lop is INF, NaN, or zero, return as-is.
            newFunc->block.addLabel(labelEMax);

            AS_Stmnt({IR::Code::Move_W, lopWords}, stk, lop);

            if(lopWords < retWords)
               AS_Stmnt({IR::Code::Move_W, retWords - lopWords}, stk, 0);

            AS_Stmnt({IR::Code::Retn, retWords}, stk);

            #undef AS_Stmnt

            throw ResetFunc();
         }

         //
         // Info::preStmnt_ShLU_W
         //
         void Info::preStmnt_ShLU_W()
         {
            if(stmnt->op.size <= 1)
               return;

            if(stmnt->op.code == IR::Code::ShLU_W)
               addFunc_ShLU_W(stmnt->op.size);
            else if(stmnt->op.code == IR::Code::ShRI_W)
               addFunc_ShRI_W(stmnt->op.size);
            else
               addFunc_ShRU_W(stmnt->op.size);
         }

         //
         // Info::putStmnt_ShLF_W
         //
         void Info::putStmnt_ShLF_W()
         {
            if(stmnt->op.size == 0)
            {
               putCode(Code::Drop_Nul);
               return;
            }

            if(stmnt->op.size == 1)
            {
               putStmntCall(1);
               return;
            }

            putStmntCall(2);

            if(stmnt->op.size == 2)
               return;

            // Check if returned word indicates INF.
            putCode(Code::Jcnd_Nil, putPos + 8 + 16 + (stmnt->op.size - 1) * 12);

            // Infinity! Drop everything and push INF.
            putCode(Code::Drop_LocReg, func->localReg + 0);

            for(Core::FastU n = stmnt->op.size; --n;)
                putCode(Code::Drop_Nul);

            for(Core::FastU n = stmnt->op.size; --n;)
                putCode(Code::Push_Lit, 0);

            putCode(Code::Push_LocReg, func->localReg + 0);
         }

         //
         // Info::putStmnt_ShLU_W
         //
         void Info::putStmnt_ShLU_W()
         {
            if(stmnt->op.size == 0)
            {
               putCode(Code::Drop_Nul);
               return;
            }

            if(stmnt->op.size == 1)
            {
               putCode(Code::ShLU);
               return;
            }

            if(stmnt->args[2].a != IR::ArgBase::Lit)
            {
               putStmntCall(stmnt->op.size);
               return;
            }

            Core::FastU shift = GetWord(stmnt->args[2].aLit) % (32 * stmnt->op.size);

            // Shift of 0 is a no-op.
            if(!shift) return;

            Core::FastU shiftBits  = shift % 32;
            Core::FastU shiftWords = shift / 32;
            Core::FastU keepWords  = stmnt->op.size - shiftWords;

            for(Core::FastU n = shiftWords; n--;)
               putCode(Code::Drop_Nul);

            for(Core::FastU n = keepWords; n--;)
               putCode(Code::Drop_LocReg, func->localReg + n);

            for(Core::FastU n = shiftWords; n--;)
               putCode(Code::Push_Lit, 0);

            if(shiftBits)
            {
               // ret[0] = l[0] << r
               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::Push_Lit,    shiftBits);
               putCode(Code::ShLU);

               for(Core::FastU n = 1; n != keepWords; ++n)
               {
                  // ret[n] = (l[n] << r) | (l[n-1] >> (32 - r))

                  putCode(Code::Push_LocReg, func->localReg + n);
                  putCode(Code::Push_Lit,    shiftBits);
                  putCode(Code::ShLU);

                  putCode(Code::Push_LocReg, func->localReg + n - 1);
                  putStmntShiftRU(32 - shiftBits);

                  putCode(Code::BOrI);
               }
            }
            else
            {
               // ret[n] = l[n]
               for(Core::FastU n = 0; n != keepWords; ++n)
                  putCode(Code::Push_LocReg, func->localReg + n);
            }
         }

         //
         // Info::putStmnt_ShRI_W
         //
         void Info::putStmnt_ShRI_W()
         {
            if(stmnt->op.size == 0)
            {
               putCode(Code::Drop_Nul);
               return;
            }

            if(stmnt->op.size == 1)
            {
               putCode(Code::ShRI);
               return;
            }

            if(stmnt->args[2].a != IR::ArgBase::Lit)
            {
               putStmntCall(stmnt->op.size);
               return;
            }

            Core::FastU shift = GetWord(stmnt->args[2].aLit) % (32 * stmnt->op.size);

            // Shift of 0 is a no-op.
            if(!shift) return;

            Core::FastU shiftBits  = shift % 32;
            Core::FastU shiftWords = shift / 32;
            Core::FastU keepWords  = stmnt->op.size - shiftWords;

            for(Core::FastU n = keepWords; n--;)
               putCode(Code::Drop_LocReg, func->localReg + n);

            for(Core::FastU n = shiftWords; n--;)
               putCode(Code::Drop_Nul);

            if(shiftBits)
            {
               for(Core::FastU n = 0; n != keepWords - 1; ++n)
               {
                  // ret[n] = (l[n] >> r) | (l[n+1] << (32 - r))

                  putCode(Code::Push_LocReg, func->localReg + n);
                  putStmntShiftRU(shiftBits);

                  putCode(Code::Push_LocReg, func->localReg + n + 1);
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
               // ret[n] = l[n]
               for(Core::FastU n = 0; n != keepWords; ++n)
                  putCode(Code::Push_LocReg, func->localReg + n);
            }

            if(shiftWords)
            {
               putCode(Code::Push_LocReg, func->localReg + keepWords - 1);
               putCode(Code::Push_Lit,    31);
               putCode(Code::ShRI);

               for(Core::FastU n = shiftWords - 1; n--;)
                  putCode(Code::Copy);
            }
         }

         //
         // Info::putStmnt_ShRU_W
         //
         void Info::putStmnt_ShRU_W()
         {
            if(stmnt->op.size == 0)
            {
               putCode(Code::Drop_Nul);
               return;
            }

            if(stmnt->op.size == 1)
            {
               putStmnt_ShRU_W1();
               return;
            }

            if(stmnt->args[2].a != IR::ArgBase::Lit)
            {
               putStmntCall(stmnt->op.size);
               return;
            }

            Core::FastU shift = GetWord(stmnt->args[2].aLit) % (32 * stmnt->op.size);

            // Shift of 0 is a no-op.
            if(!shift) return;

            Core::FastU shiftBits  = shift % 32;
            Core::FastU shiftWords = shift / 32;
            Core::FastU keepWords  = stmnt->op.size - shiftWords;

            for(Core::FastU n = keepWords; n--;)
               putCode(Code::Drop_LocReg, func->localReg + n);

            for(Core::FastU n = shiftWords; n--;)
               putCode(Code::Drop_Nul);

            if(shiftBits)
            {
               for(Core::FastU n = 0; n != keepWords - 1; ++n)
               {
                  // ret[n] = (l[n] >> r) | (l[n+1] << (32 - r))

                  putCode(Code::Push_LocReg, func->localReg + n);
                  putStmntShiftRU(shiftBits);

                  putCode(Code::Push_LocReg, func->localReg + n + 1);
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
               // ret[n] = l[n]
               for(Core::FastU n = 0; n != keepWords; ++n)
                  putCode(Code::Push_LocReg, func->localReg + n);
            }

            for(Core::FastU n = shiftWords; n--;)
               putCode(Code::Push_Lit, 0);
         }

         //
         // Info::putStmnt_ShRU_W1
         //
         void Info::putStmnt_ShRU_W1()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
            {
               putStmntShiftRU(GetWord(stmnt->args[2].aLit));
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
         // Info::trStmnt_ShLF_W
         //
         void Info::trStmnt_ShLF_W()
         {
            if(!trStmntShift(stmnt->op.size, true))
               return;

            if(stmnt->op.size <= 2)
               return;

            func->setLocalTmp(1);
         }

         //
         // Info::trStmnt_ShLU_W
         //
         void Info::trStmnt_ShLU_W()
         {
            if(stmnt->op.size <= 1)
               return (void)trStmntShift(stmnt->op.size, true);

            if(!trStmntShift(stmnt->op.size, false))
               return;

            Core::FastU shift = GetWord(stmnt->args[2].aLit) % (32 * stmnt->op.size);

            if(shift)
               func->setLocalTmp(stmnt->op.size - shift / 32);
         }

         //
         // Info::trStmnt_ShRI_W
         //
         void Info::trStmnt_ShRI_W()
         {
            if(stmnt->op.size <= 1)
               return (void)trStmntShift(stmnt->op.size, true);

            if(!trStmntShift(stmnt->op.size, false))
               return;

            Core::FastU shift = GetWord(stmnt->args[2].aLit) % (32 * stmnt->op.size);

            if(shift)
               func->setLocalTmp(stmnt->op.size - shift / 32);
         }

         //
         // Info::trStmnt_ShRU_W
         //
         void Info::trStmnt_ShRU_W()
         {
            if(stmnt->op.size == 0)
               return (void)trStmntShift(stmnt->op.size, true);

            if(stmnt->op.size == 1)
               return trStmnt_ShRU_W1();

            if(!trStmntShift(stmnt->op.size, false))
               return;

            Core::FastU shift = GetWord(stmnt->args[2].aLit) % (32 * stmnt->op.size);

            if(shift)
               func->setLocalTmp(stmnt->op.size - shift / 32);
         }

         //
         // Info::trStmnt_ShRU_W1
         //
         void Info::trStmnt_ShRU_W1()
         {
            CheckArgC(stmnt, 3);

            if(stmnt->args[1].a != IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
               throw Core::ExceptStr(stmnt->pos, "trStmnt_ShRU_W1 disorder");

            moveArgStk_dst(stmnt->args[0], stmnt->op.size);
            moveArgStk_src(stmnt->args[1], stmnt->op.size);

            if(!isPushArg(stmnt->args[2]) || !isFastArg(stmnt->args[2]))
            {
               func->setLocalTmp(1);
               moveArgStk_src(stmnt->args[2], 1);
            }
         }
      }
   }
}

// EOF

