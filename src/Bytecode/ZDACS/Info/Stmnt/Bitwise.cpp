//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Bitwise statement handling.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Bytecode/ZDACS/Code.hpp"

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
         // Info::genStmnt_Bclz_W
         //
         void Info::genStmnt_Bclz_W()
         {
            if(stmnt->op.size == 0)
            {
               numChunkCODE += 8;
               return;
            }

            if(stmnt->op.size == 1)
            {
               genStmntCall(1);
               return;
            }

            if(stmnt->args[1].a != IR::ArgBase::Stk)
            {
               numChunkCODE += 28 + (stmnt->op.size - 1) * 40 + 8
                  + lenPushArg(stmnt->args[1], 0, stmnt->op.size);
            }
            else if(stmnt->args[0].a != IR::ArgBase::Stk)
            {
               numChunkCODE += 28 + (stmnt->op.size - 1) * 40 + 8
                  + lenDropArg(stmnt->args[0], 0) * (stmnt->op.size + 1);

               for(Core::FastU i = stmnt->op.size; --i;)
                  numChunkCODE += i * 4;
            }
            else
            {
               numChunkCODE += 36 + (stmnt->op.size - 1) * 48 + 24;

               for(Core::FastU i = stmnt->op.size; --i;)
                  numChunkCODE += i * 4;
            }
         }

         //
         // Info::genStmntBitwise
         //
         void Info::genStmntBitwise()
         {
            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               if(stmnt->op.size > 1)
                  numChunkCODE += (stmnt->op.size * 4 - 2) * 8;
               numChunkCODE += stmnt->op.size * 4;
            }
            else
            {
               numChunkCODE += lenPushArg(stmnt->args[1], 0, stmnt->op.size);
               numChunkCODE += lenPushArg(stmnt->args[2], 0, stmnt->op.size);
               numChunkCODE += stmnt->op.size * 4;
            }
         }

         //
         // Info::putStmnt_Bclz_W
         //
         void Info::putStmnt_Bclz_W(bool ones)
         {
            if(stmnt->op.size == 0)
            {
               putCode(Code::Push_Lit, 0);
               return;
            }

            Core::String name = ones ? "___GDCC__Bclo_W1" : "___GDCC__Bclz_W1";

            if(stmnt->op.size == 1)
            {
               putStmntCall(name, 1);
               return;
            }

            Core::FastU skip = ones ? 0xFFFFFFFF : 0;
            Core::FastU fIdx = GetWord(resolveGlyph(name));

            if(stmnt->args[1].a != IR::ArgBase::Stk)
            {
               Core::FastU i = stmnt->op.size - 1;

               putStmntPushArg(stmnt->args[1], i);
               putCode(Code::Jcnd_Lit, skip, putPos + 28);
               putCode(Code::Call_Stk, fIdx);
               putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8
                  + lenPushArg(stmnt->args[1], 0, i));

               while(i--)
               {
                  putStmntPushArg(stmnt->args[1], i);
                  putCode(Code::Jcnd_Lit, skip, putPos + 40);
                  putCode(Code::Call_Stk, fIdx);
                  putCode(Code::Push_Lit, stmnt->op.size - 1 - i);
                  putCode(Code::AddU);
                  putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8
                     + lenPushArg(stmnt->args[1], 0, i));
               }

               putCode(Code::Push_Lit, stmnt->op.size * 32);
            }
            else if(stmnt->args[0].a != IR::ArgBase::Stk)
            {
               Core::FastU i = stmnt->op.size - 1;

               Core::FastU dropLen = 0;
               for(auto j = i; --j;) dropLen += j * 4;

               putCode(Code::Jcnd_Lit, skip, putPos + 28 + i * 4
                     + lenDropArg(stmnt->args[1], 0));
               putCode(Code::Call_Stk, fIdx);
               putStmntDropArg(stmnt->args[1], 0);
               for(auto j = i; j--;) putCode(Code::Drop_Nul);
               putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8 + dropLen
                  + lenDropArg(stmnt->args[1], 0) * (i + 1));

               while(i--)
               {
                  dropLen -= i * 4;

                  putCode(Code::Jcnd_Lit, skip, putPos + 40 + i * 4
                     + lenDropArg(stmnt->args[1], 0));
                  putCode(Code::Call_Stk, fIdx);
                  putCode(Code::Push_Lit, stmnt->op.size - 1 - i);
                  putCode(Code::AddU);
                  putStmntDropArg(stmnt->args[1], 0);
                  for(auto j = i; j--;) putCode(Code::Drop_Nul);
                  putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8 + dropLen
                     + lenDropArg(stmnt->args[1], 0) * (i + 1));
               }

               putCode(Code::Push_Lit, stmnt->op.size * 32);
               putStmntDropArg(stmnt->args[1], 0);
            }
            else
            {
               Core::FastU i = stmnt->op.size - 1;

               Core::FastU dropLen = 0;
               for(auto j = i; --j;) dropLen += j * 4;

               putCode(Code::Jcnd_Lit, skip, putPos + 28 + i * 4 + 8);
               putCode(Code::Call_Stk, fIdx);
               putCode(Code::Drop_LocReg, func->localReg + 0);
               for(auto j = i; j--;) putCode(Code::Drop_Nul);
               putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8 + dropLen
                  + 8 * (i + 1));

               while(i--)
               {
                  dropLen -= i * 4;

                  putCode(Code::Jcnd_Lit, skip, putPos + 40 + i * 4 + 8);
                  putCode(Code::Call_Stk, fIdx);
                  putCode(Code::Push_Lit, stmnt->op.size - 1 - i);
                  putCode(Code::AddU);
                  putCode(Code::Drop_LocReg, func->localReg + 0);
                  for(auto j = i; j--;) putCode(Code::Drop_Nul);
                  putCode(Code::Jump_Lit, putPos + 8 + i * 40 + 8 + dropLen
                     + 8 * (i + 1));
               }

               putCode(Code::Push_Lit, stmnt->op.size * 32);
               putCode(Code::Drop_LocReg, func->localReg + 0);

               putCode(Code::Push_LocReg, func->localReg + 0);
            }
         }

         //
         // Info::putStmntBitwise
         //
         void Info::putStmntBitwise(Code code)
         {
            if(stmnt->args[1].a == IR::ArgBase::Stk &&
               stmnt->args[2].a == IR::ArgBase::Stk)
            {
               if(stmnt->op.size > 1)
               {
                  for(Core::FastU i = 0, e = stmnt->op.size * 2 - 1; i != e; ++i)
                     putCode(Code::Drop_LocReg, func->localReg + i);

                  putCode(Code::Push_LocReg, func->localReg + stmnt->op.size - 1);
               }

               putCode(code);

               if(stmnt->op.size > 1) for(Core::FastU i = stmnt->op.size - 1; i--;)
               {
                  putCode(Code::Push_LocReg, func->localReg + stmnt->op.size + i);
                  putCode(Code::Push_LocReg, func->localReg + i);
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
         // Info::trStmnt_Bclz_W
         //
         void Info::trStmnt_Bclz_W()
         {
            CheckArgC(stmnt, 2);

            if(stmnt->op.size <= 1)
            {
               trStmntStk2(1, stmnt->op.size);
            }
            else if(isPushArg(stmnt->args[1]))
            {
               moveArgStk_dst(stmnt->args[0], 1);
            }
            else if(isDropArg(stmnt->args[0]))
            {
               moveArgStk_src(stmnt->args[1], stmnt->op.size);
            }
            else
            {
               trStmntStk2(1, stmnt->op.size);

               func->setLocalTmp(1);
            }
         }

         //
         // Info::trStmntBitwise
         //
         void Info::trStmntBitwise()
         {
            CheckArgC(stmnt, 3);

            if(isPushArg(stmnt->args[1]) && isPushArg(stmnt->args[2]))
            {
               moveArgStk_dst(stmnt->args[0], stmnt->op.size);
            }
            else
            {
               trStmntStk3(stmnt->op.size, stmnt->op.size, false);

               if(stmnt->op.size > 1)
                  func->setLocalTmp(stmnt->op.size * 2 - 1);
            }
         }
      }
   }
}

// EOF

