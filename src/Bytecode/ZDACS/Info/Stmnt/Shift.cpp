//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Shift statement handling.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Bytecode/ZDACS/Code.hpp"

#include "Core/Exception.hpp"

#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
      {
         //
         // Info::genStmnt_ShRU_W
         //
         void Info::genStmnt_ShRU_W()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
               numChunkCODE += 24;
            else if(stmnt->args[2].a == IR::ArgBase::Stk)
               numChunkCODE += 68;
            else
               numChunkCODE += 48 + lenPushArg(stmnt->args[2], 0) * 2;
         }

         //
         // Info::genStmnt_ShLU_W2
         //
         void Info::genStmnt_ShLU_W2()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 64;

                    if(shift ==  0) numChunkCODE +=  0;
               else if(shift <  32) numChunkCODE += 92;
               else if(shift == 32) numChunkCODE += 16;
               else                 numChunkCODE += 28;
            }
            else
               genStmntCall(2);
         }

         //
         // Info::genStmnt_ShRI_W2
         //
         void Info::genStmnt_ShRI_W2()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 64;

                    if(shift ==  0) numChunkCODE +=  0;
               else if(shift <  32) numChunkCODE += 76;
               else if(shift == 32) numChunkCODE += 24;
               else                 numChunkCODE += 36;
            }
            else
               genStmntCall(2);
         }

         //
         // Info::genStmnt_ShRU_W2
         //
         void Info::genStmnt_ShRU_W2()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 64;

                    if(shift ==  0) numChunkCODE +=  0;
               else if(shift <  32) numChunkCODE += 88;
               else if(shift == 32) numChunkCODE += 16;
               else                 numChunkCODE += 40;
            }
            else
               genStmntCall(2);
         }

         //
         // Info::genStmnt_ShLU_W3
         //
         void Info::genStmnt_ShLU_W3()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 96;

                    if(shift ==  0) numChunkCODE +=   0;
               else if(shift <  32) numChunkCODE += 156;
               else if(shift == 32) numChunkCODE +=  32;
               else if(shift <  64) numChunkCODE += 104;
               else if(shift == 64) numChunkCODE +=  32;
               else                 numChunkCODE +=  44;
            }
            else
               genStmntCall(3);
         }

         //
         // Info::genStmnt_ShRI_W3
         //
         void Info::genStmnt_ShRI_W3()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 96;

                    if(shift ==  0) numChunkCODE +=   0;
               else if(shift <  32) numChunkCODE += 140;
               else if(shift == 32) numChunkCODE +=  40;
               else if(shift <  64) numChunkCODE += 100;
               else if(shift == 64) numChunkCODE +=  36;
               else                 numChunkCODE +=  48;
            }
            else
               genStmntCall(3);
         }

         //
         // Info::genStmnt_ShRU_W3
         //
         void Info::genStmnt_ShRU_W3()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 96;

                    if(shift ==  0) numChunkCODE +=   0;
               else if(shift <  32) numChunkCODE += 152;
               else if(shift == 32) numChunkCODE +=  32;
               else if(shift <  64) numChunkCODE += 104;
               else if(shift == 64) numChunkCODE +=  32;
               else                 numChunkCODE +=  56;
            }
            else
               genStmntCall(3);
         }

         //
         // Info::putStmnt_ShRU_W
         //
         void Info::putStmnt_ShRU_W()
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
               putCode(Code::InvU);
               putCode(Code::AndU);
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
               putCode(Code::InvU);
               putCode(Code::AndU);
            }
         }

         //
         // Info::putStmnt_ShLU_W2
         //
         void Info::putStmnt_ShLU_W2()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 64;

               if(shift == 0)
               {
               }
               else if(shift < 32)
               {
                  putCode(Code::Drop_LocReg, func->localReg + 1);
                  putCode(Code::Drop_LocReg, func->localReg + 0);

                  // ret0 = l0 << r
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putCode(Code::Push_Lit,    shift);
                  putCode(Code::ShLU);

                  // ret1 = (l1 << r) | (l0 >> (32 - r))
                  putCode(Code::Push_LocReg, func->localReg + 1);
                  putCode(Code::Push_Lit,    shift);
                  putCode(Code::ShLU);

                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putStmntShiftRU(32 - shift);

                  putCode(Code::OrIU);
               }
               else if(shift == 32)
               {
                  putCode(Code::Drop_Nul);
                  putCode(Code::Push_Lit, 0);
                  putCode(Code::Swap);
               }
               else
               {
                  putCode(Code::Drop_Nul);
                  putCode(Code::Push_Lit, shift - 32);
                  putCode(Code::ShLU);
                  putCode(Code::Push_Lit, 0);
                  putCode(Code::Swap);
               }
            }
            else
               putStmntCall("___GDCC__ShLU_W2", 2);
         }

         //
         // Info::putStmnt_ShRI_W2
         //
         void Info::putStmnt_ShRI_W2()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 64;

               if(shift == 0)
               {
               }
               else if(shift < 32)
               {
                  putCode(Code::Drop_LocReg, func->localReg + 0);

                  // ret0 = (l0 >> r) | (l1 << (32 - r))
                  putStmntShiftRU(shift);
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putCode(Code::Push_Lit,    32 - shift);
                  putCode(Code::ShLU);
                  putCode(Code::OrIU);

                  // ret1 = l1 >> r
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putCode(Code::Push_Lit,    shift);
                  putCode(Code::ShRI);
               }
               else if(shift == 32)
               {
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);
                  putCode(Code::Copy);
                  putCode(Code::Push_Lit, 31);
                  putCode(Code::ShRI);
               }
               else
               {
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);

                  // ret0 = l1 >> (r - 32)
                  putCode(Code::Push_Lit, shift - 32);
                  putCode(Code::ShRI);

                  // ret1 = l1 < 0 ? -1 : 0
                  putCode(Code::Copy);
                  putCode(Code::Push_Lit, 31);
                  putCode(Code::ShRI);
               }
            }
            else
               putStmntCall("___GDCC__ShRI_W2", 2);
         }

         //
         // Info::putStmnt_ShRU_W2
         //
         void Info::putStmnt_ShRU_W2()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 64;

               if(shift == 0)
               {
               }
               else if(shift < 32)
               {
                  putCode(Code::Drop_LocReg, func->localReg + 0);

                  // ret0 = (l0 >> r) | (l1 << (32 - r))
                  putStmntShiftRU(shift);
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putCode(Code::Push_Lit,    32 - shift);
                  putCode(Code::ShLU);
                  putCode(Code::OrIU);

                  // ret1 = l1 >> r
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putStmntShiftRU(shift);
               }
               else if(shift == 32)
               {
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);
                  putCode(Code::Push_Lit, 0);
               }
               else
               {
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);

                  // ret0 = l1 >> (r - 32)
                  putStmntShiftRU(shift - 32);

                  // ret1 = 0
                  putCode(Code::Push_Lit, 0);
               }
            }
            else
               putStmntCall("___GDCC__ShRU_W2", 2);
         }

         //
         // Info::putStmnt_ShLU_W3
         //
         void Info::putStmnt_ShLU_W3()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 96;

               if(shift == 0)
               {
               }
               else if(shift < 32)
               {
                  putCode(Code::Drop_LocReg, func->localReg + 2);
                  putCode(Code::Drop_LocReg, func->localReg + 1);
                  putCode(Code::Drop_LocReg, func->localReg + 0);

                  // ret0 = l0 << r
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putCode(Code::Push_Lit,    shift);
                  putCode(Code::ShLU);

                  // ret1 = (l1 << r) | (l0 >> (32 - r))
                  putCode(Code::Push_LocReg, func->localReg + 1);
                  putCode(Code::Push_Lit,    shift);
                  putCode(Code::ShLU);

                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putStmntShiftRU(32 - shift);

                  putCode(Code::OrIU);

                  // ret2 = (l2 << r) | (l1 >> (32 - r))
                  putCode(Code::Push_LocReg, func->localReg + 2);
                  putCode(Code::Push_Lit,    shift);
                  putCode(Code::ShLU);

                  putCode(Code::Push_LocReg, func->localReg + 1);
                  putStmntShiftRU(32 - shift);

                  putCode(Code::OrIU);
               }
               else if(shift == 32)
               {
                  putCode(Code::Drop_Nul);
                  putCode(Code::Drop_LocReg, func->localReg + 0);
                  putCode(Code::Push_Lit, 0);
                  putCode(Code::Swap);
                  putCode(Code::Push_LocReg, func->localReg + 0);
               }
               else if(shift < 64)
               {
                  putCode(Code::Drop_Nul);
                  putCode(Code::Drop_LocReg, func->localReg + 1);
                  putCode(Code::Drop_LocReg, func->localReg + 0);

                  // ret0 = 0
                  putCode(Code::Push_Lit, 0);

                  // ret0 = l1 << r
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putCode(Code::Push_Lit,    shift - 32);
                  putCode(Code::ShLU);

                  // ret1 = (l2 << r) | (l1 >> (32 - r))
                  putCode(Code::Push_LocReg, func->localReg + 1);
                  putCode(Code::Push_Lit,    shift - 32);
                  putCode(Code::ShLU);

                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putStmntShiftRU(32 - (shift - 32));

                  putCode(Code::OrIU);
               }
               else if(shift == 64)
               {
                  putCode(Code::Drop_Nul);
                  putCode(Code::Drop_Nul);
                  putCode(Code::Push_Lit, 0);
                  putCode(Code::Swap);
                  putCode(Code::Push_Lit, 0);
                  putCode(Code::Swap);
               }
               else
               {
                  putCode(Code::Drop_Nul);
                  putCode(Code::Drop_Nul);
                  putCode(Code::Push_Lit, shift - 32);
                  putCode(Code::ShLU);
                  putCode(Code::Push_Lit, 0);
                  putCode(Code::Swap);
                  putCode(Code::Push_Lit, 0);
                  putCode(Code::Swap);
               }
            }
            else
               putStmntCall("___GDCC__ShLU_W3", 3);
         }

         //
         // Info::putStmnt_ShRI_W3
         //
         void Info::putStmnt_ShRI_W3()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 96;

               if(shift == 0)
               {
               }
               else if(shift < 32)
               {
                  putCode(Code::Drop_LocReg, func->localReg + 1);
                  putCode(Code::Drop_LocReg, func->localReg + 0);

                  // ret0 = (l0 >> r) | (l1 << (32 - r))
                  putStmntShiftRU(shift);
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putCode(Code::Push_Lit,    32 - shift);
                  putCode(Code::ShLU);
                  putCode(Code::OrIU);

                  // ret1 = (l1 >> r) | (l2 << (32 - r))
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putStmntShiftRU(shift);
                  putCode(Code::Push_LocReg, func->localReg + 1);
                  putCode(Code::Push_Lit,    32 - shift);
                  putCode(Code::ShLU);
                  putCode(Code::OrIU);

                  // ret2 = l2 >> r
                  putCode(Code::Push_LocReg, func->localReg + 1);
                  putCode(Code::Push_Lit,    shift);
                  putCode(Code::ShRI);
               }
               else if(shift == 32)
               {
                  putCode(Code::Drop_LocReg, func->localReg + 0);
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putCode(Code::Copy);
                  putCode(Code::Push_Lit, 31);
                  putCode(Code::ShRI);
               }
               else if(shift < 64)
               {
                  putCode(Code::Drop_LocReg, func->localReg + 0);
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);

                  // ret0 = (l1 >> r) | (l2 << (32 - r))
                  putStmntShiftRU(shift - 32);
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putCode(Code::Push_Lit,    32 - (shift - 32));
                  putCode(Code::ShLU);
                  putCode(Code::OrIU);

                  // ret1 = l2 >> r
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putCode(Code::Push_Lit,    shift - 32);
                  putCode(Code::ShRI);

                  // ret1 = l2 < 0 ? -1 : 0
                  putCode(Code::Copy);
                  putCode(Code::Push_Lit, 31);
                  putCode(Code::ShRI);
               }
               else if(shift == 64)
               {
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);
                  putCode(Code::Copy);
                  putCode(Code::Push_Lit, 31);
                  putCode(Code::ShRI);
                  putCode(Code::Copy);
               }
               else
               {
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);

                  // ret0 = l2 >> (r - 64)
                  putCode(Code::Push_Lit, shift - 64);
                  putCode(Code::ShRI);

                  // ret1 = l2 < 0 ? -1 : 0
                  putCode(Code::Copy);
                  putCode(Code::Push_Lit, 31);
                  putCode(Code::ShRI);

                  // ret2 = l2 < 0 ? -1 : 0
                  putCode(Code::Copy);
               }
            }
            else
               putStmntCall("___GDCC__ShRI_W3", 3);
         }

         //
         // Info::putStmnt_ShRU_W3
         //
         void Info::putStmnt_ShRU_W3()
         {
            if(stmnt->args[2].a == IR::ArgBase::Lit)
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 96;

               if(shift == 0)
               {
               }
               else if(shift < 32)
               {
                  putCode(Code::Drop_LocReg, func->localReg + 1);
                  putCode(Code::Drop_LocReg, func->localReg + 0);

                  // ret0 = (l0 >> r) | (l1 << (32 - r))
                  putStmntShiftRU(shift);
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putCode(Code::Push_Lit,    32 - shift);
                  putCode(Code::ShLU);
                  putCode(Code::OrIU);

                  // ret1 = (l1 >> r) | (l2 << (32 - r))
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putStmntShiftRU(shift);
                  putCode(Code::Push_LocReg, func->localReg + 1);
                  putCode(Code::Push_Lit,    32 - shift);
                  putCode(Code::ShLU);
                  putCode(Code::OrIU);

                  // ret2 = l2 >> r
                  putCode(Code::Push_LocReg, func->localReg + 1);
                  putStmntShiftRU(shift);
               }
               else if(shift == 32)
               {
                  putCode(Code::Drop_LocReg, func->localReg + 0);
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putCode(Code::Push_Lit, 0);
               }
               else if(shift < 64)
               {
                  putCode(Code::Drop_LocReg, func->localReg + 0);
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);

                  // ret0 = (l1 >> r) | (l2 << (32 - r))
                  putStmntShiftRU(shift - 32);
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putCode(Code::Push_Lit,    32 - (shift - 32));
                  putCode(Code::ShLU);
                  putCode(Code::OrIU);

                  // ret1 = l2 >> r
                  putCode(Code::Push_LocReg, func->localReg + 0);
                  putStmntShiftRU(shift - 32);

                  // ret2 = 0
                  putCode(Code::Push_Lit, 0);
               }
               else if(shift == 64)
               {
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);
                  putCode(Code::Push_Lit, 0);
                  putCode(Code::Push_Lit, 0);
               }
               else
               {
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);
                  putCode(Code::Swap);
                  putCode(Code::Drop_Nul);

                  // ret0 = l2 >> (r - 64)
                  putStmntShiftRU(shift - 64);

                  // ret1 = 0
                  putCode(Code::Push_Lit, 0);

                  // ret2 = 0
                  putCode(Code::Push_Lit, 0);
               }
            }
            else
               putStmntCall("___GDCC__ShRU_W3", 3);
         }

         //
         // Info::putStmntShiftRU
         //
         void Info::putStmntShiftRU(Core::FastU shift)
         {
            putCode(Code::Push_Lit, shift % 32);
            putCode(Code::ShRI);
            putCode(Code::Push_Lit, 0xFFFFFFFF >> (shift % 32));
            putCode(Code::AndU);
         }

         //
         // Info::trStmnt_ShRU_W
         //
         void Info::trStmnt_ShRU_W()
         {
            CheckArgC(stmnt, 3);

            if(stmnt->args[1].a != IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
               throw Core::ExceptStr(stmnt->pos, "trStmnt_ShRU_W disorder");

            moveArgStk_dst(stmnt->args[0], IR::Code::Move_W);
            if(moveArgStk_src(stmnt->args[1], IR::Code::Move_W)) return;

            if(!isPushArg(stmnt->args[2]) || !isFastArg(stmnt->args[2]))
            {
               func->setLocalTmp(1);
               moveArgStk_src(stmnt->args[2], IR::Code::Move_W);
            }
         }

         //
         // Info::trStmnt_ShLU_W2
         //
         void Info::trStmnt_ShLU_W2()
         {
            if(trStmntShift(IR::Code::Move_W2))
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 64;

                    if(shift ==  0) func->setLocalTmp(0);
               else if(shift <  32) func->setLocalTmp(2);
               else if(shift == 32) func->setLocalTmp(0);
               else                 func->setLocalTmp(0);
            }
         }

         //
         // Info::trStmnt_ShRI_W2
         //
         void Info::trStmnt_ShRI_W2()
         {
            if(trStmntShift(IR::Code::Move_W2))
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 64;

                    if(shift ==  0) func->setLocalTmp(0);
               else if(shift <  32) func->setLocalTmp(1);
               else if(shift == 32) func->setLocalTmp(0);
               else                 func->setLocalTmp(0);
            }
         }

         //
         // Info::trStmnt_ShRU_W2
         //
         void Info::trStmnt_ShRU_W2()
         {
            if(trStmntShift(IR::Code::Move_W2))
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 64;

                    if(shift ==  0) func->setLocalTmp(0);
               else if(shift <  32) func->setLocalTmp(1);
               else if(shift == 32) func->setLocalTmp(0);
               else                 func->setLocalTmp(0);
            }
         }

         //
         // Info::trStmnt_ShLU_W3
         //
         void Info::trStmnt_ShLU_W3()
         {
            if(trStmntShift(IR::Code::Move_W3))
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 96;

                    if(shift ==  0) func->setLocalTmp(0);
               else if(shift <  32) func->setLocalTmp(3);
               else if(shift == 32) func->setLocalTmp(1);
               else if(shift <  64) func->setLocalTmp(2);
               else if(shift == 64) func->setLocalTmp(0);
               else                 func->setLocalTmp(0);
            }
         }

         //
         // Info::trStmnt_ShRI_W3
         //
         void Info::trStmnt_ShRI_W3()
         {
            if(trStmntShift(IR::Code::Move_W3))
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 96;

                    if(shift ==  0) func->setLocalTmp(0);
               else if(shift <  32) func->setLocalTmp(2);
               else if(shift == 32) func->setLocalTmp(1);
               else if(shift <  64) func->setLocalTmp(1);
               else if(shift == 64) func->setLocalTmp(0);
               else                 func->setLocalTmp(0);
            }
         }

         //
         // Info::trStmnt_ShRU_W3
         //
         void Info::trStmnt_ShRU_W3()
         {
            if(trStmntShift(IR::Code::Move_W3))
            {
               auto shift = GetWord(stmnt->args[2].aLit) % 96;

                    if(shift ==  0) func->setLocalTmp(0);
               else if(shift <  32) func->setLocalTmp(2);
               else if(shift == 32) func->setLocalTmp(1);
               else if(shift <  64) func->setLocalTmp(1);
               else if(shift == 64) func->setLocalTmp(0);
               else                 func->setLocalTmp(0);
            }
         }

         //
         // Info::trStmntShift
         //
         bool Info::trStmntShift(IR::Code codeMove, bool moveLit)
         {
            CheckArgC(stmnt, 3);

            if(stmnt->args[1].a != IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
               throw Core::ExceptStr(stmnt->pos, "trStmntShift disorder");

            moveArgStk_dst(stmnt->args[0], codeMove);
            if(moveArgStk_src(stmnt->args[1], codeMove)) return false;

            if(!moveLit && stmnt->args[2].a == IR::ArgBase::Lit)
               return true;

            moveArgStk_src(stmnt->args[2], IR::Code::Move_W);
            return false;
         }
      }
   }
}

// EOF

