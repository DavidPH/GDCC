//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Jump statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "Core/Exception.hpp"

#include "IR/Statement.hpp"


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
         // Info::genStmnt_Jcnd_Nil
         //
         void Info::genStmnt_Jcnd_Nil()
         {
            if(stmnt->op.size == 0)
            {
               numChunkCODE += 8;
               return;
            }

            numChunkCODE += stmnt->op.size * 4 + 4;
         }

         //
         // Info::genStmnt_Jcnd_Tab
         //
         void Info::genStmnt_Jcnd_Tab()
         {
            if(stmnt->args.size() == 3)
               numChunkCODE += 12;
            else
               numChunkCODE += stmnt->args.size() * 4 + 4;
         }

         //
         // Info::genStmnt_Jcnd_Tru
         //
         void Info::genStmnt_Jcnd_Tru()
         {
            if(stmnt->op.size == 0) return;

            numChunkCODE += stmnt->op.size * 4 + 4;
         }

         //
         // Info::genStmnt_Jfar
         //
         void Info::genStmnt_Jfar()
         {
            if(stmnt->args.size() > 1)
            {
               numChunkCODE += lenPushArg(stmnt->args[2], 0) + lenDropArg(stmnt->args[1], 2);
               numChunkCODE += 8 + lenPushIdx(stmnt->args[1], 0) + 8;
               numChunkCODE += lenPushIdx(stmnt->args[1], 0);
            }
            else
            {
               numChunkCODE += 28;
            }

            numChunkCODE += stmnt->op.size * 4 + 28 + 8 + (36 + 36 + 24 + 4);
         }

         //
         // Info::genStmnt_Jset
         //
         void Info::genStmnt_Jset()
         {
            numChunkCODE += lenPushArg(stmnt->args[1], 0) + lenDropArg(stmnt->args[0], 1);
            numChunkCODE += 8 + lenDropArg(stmnt->args[0], 0);
            numChunkCODE += 8;
         }

         //
         // Info::genStmnt_Jump
         //
         void Info::genStmnt_Jump()
         {
            numChunkCODE += 8;
         }

         //
         // Info::putStmnt_Jcnd_Nil
         //
         void Info::putStmnt_Jcnd_Nil()
         {
            if(stmnt->op.size == 0)
            {
               putCode(Code::Jump_Lit, GetWord(stmnt->args[1].aLit));
               return;
            }

            for(auto i = stmnt->op.size; --i;)
               putCode(Code::OrIU);

            putCode(Code::Jcnd_Nil, GetWord(stmnt->args[1].aLit));
         }

         //
         // Info::putStmnt_Jcnd_Tab
         //
         void Info::putStmnt_Jcnd_Tab()
         {
            if(stmnt->args.size() == 3)
            {
               putCode(Code::Jcnd_Lit);
               putWord(GetWord(stmnt->args[1].aLit));
               putWord(GetWord(stmnt->args[2].aLit));
            }
            else
            {
               putCode(Code::Jcnd_Tab, stmnt->args.size() / 2);

               struct JumpData {Core::FastU value, label;};

               // Collect jump cases.
               Core::Array<JumpData> Jumps{stmnt->args.size() / 2};
               for(Core::FastU i = 0; i != Jumps.size(); ++i)
               {
                  Jumps[i].value = GetWord(stmnt->args[i * 2 + 1].aLit);
                  Jumps[i].label = GetWord(stmnt->args[i * 2 + 2].aLit);
               }

               // Sort by value as signed.
               std::sort(Jumps.begin(), Jumps.end(),
                  [](JumpData const &l, JumpData const &r) -> bool
                  {
                     if(l.value & 0x80000000)
                        return r.value & 0x80000000 ? l.value > r.value : false;
                     else
                        return r.value & 0x80000000 ? true : l.value < r.value;
                  }
               );

               // Write sorted jump cases.
               for(auto const &jump : Jumps)
               {
                  putWord(jump.value);
                  putWord(jump.label);
               }
            }
         }

         //
         // Info::putStmnt_Jcnd_Tru
         //
         void Info::putStmnt_Jcnd_Tru()
         {
            if(stmnt->op.size == 0) return;

            for(auto i = stmnt->op.size; --i;)
               putCode(Code::OrIU);

            putCode(Code::Jcnd_Tru, GetWord(stmnt->args[1].aLit));
         }

         //
         // Info::putStmnt_Jfar
         //
         void Info::putStmnt_Jfar()
         {
            if(stmnt->args.size() > 1)
            {
               // Initiate far jump.

               putStmntPushArg(stmnt->args[2], 0);
               putStmntDropArg(stmnt->args[1], 2);

               putCode(Code::Push_Lit,    FarJumpIndex);
               putStmntPushIdx(stmnt->args[1], 0);
               putCode(Code::Drop_GblArr, StaArray);

               putStmntPushIdx(stmnt->args[1], 0);
            }
            else
            {
               // Check for ongoing far jump.

               putCode(Code::Push_Lit,    FarJumpIndex);
               putCode(Code::Push_GblArr, StaArray);
               putCode(Code::Jcnd_Lit,    0, putPos + 12 + stmnt->op.size * 4
                  + 28 + 8 + (36 + 36 + 24 + 4));
            }

            for(auto i = stmnt->op.size; i--;)
               putCode(Code::Drop_Nul);

            // Check auto pointer match.
            putCode(Code::Push_GblArr, StaArray);
            putCode(Code::Push_LocReg, getStkPtrIdx());
            putCode(Code::CmpU_EQ);
            putCode(Code::Jcnd_Tru,    putPos + 8 + 8);

            // Non-match! Branch to addr.
            putCode(Code::Jump_Lit, GetWord(stmnt->args[0].aLit.value));

            // Match! Push result and execute dynamic branch.
            putCode(Code::Push_Lit,    FarJumpIndex);
            putCode(Code::Push_GblArr, StaArray);
            putCode(Code::Push_Lit,    2);
            putCode(Code::AddU);
            putCode(Code::Push_GblArr, StaArray);

            putCode(Code::Push_Lit,    FarJumpIndex);
            putCode(Code::Push_GblArr, StaArray);
            putCode(Code::Push_Lit,    1);
            putCode(Code::AddU);
            putCode(Code::Push_GblArr, StaArray);

            putCode(Code::Push_Lit,    FarJumpIndex);
            putCode(Code::Push_Lit,    0);
            putCode(Code::Drop_GblArr, StaArray);

            putCode(Code::Jdyn);
         }

         //
         // Info::putStmnt_Jset
         //
         void Info::putStmnt_Jset()
         {
            putStmntPushArg(stmnt->args[1], 0);
            putStmntDropArg(stmnt->args[0], 1);

            putCode(Code::Push_LocReg, getStkPtrIdx());
            putStmntDropArg(stmnt->args[0], 0);

            putCode(Code::Push_Lit, 0);
         }

         //
         // Info::putStmnt_Jump
         //
         void Info::putStmnt_Jump()
         {
            putCode(Code::Jump_Lit, GetWord(stmnt->args[0].aLit));
         }

         //
         // Info::trStmnt_Jcnd_Nil
         //
         void Info::trStmnt_Jcnd_Nil()
         {
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 1, IR::ArgBase::Lit);
            moveArgStk_src(stmnt->args[0], stmnt->op.size);
         }

         //
         // Info::trStmnt_Jcnd_Tab
         //
         void Info::trStmnt_Jcnd_Tab()
         {
            if(stmnt->op.size != 1)
               throw Core::ExceptStr(stmnt->pos, "unsupported op size for Jcnd_Tab");

            if(stmnt->args.size() % 2 != 1)
               throw Core::ExceptStr(stmnt->pos, "invalied arg count for Jcnd_Tab");

            for(Core::FastU i = 1; i != stmnt->args.size(); ++i)
               CheckArgB(stmnt, i, IR::ArgBase::Lit);

            // Argument gets left on stack, so do not just move to stack.
            CheckArgB(stmnt, 0, IR::ArgBase::Stk);
         }

         //
         // Info::trStmnt_Jcnd_Tru
         //
         void Info::trStmnt_Jcnd_Tru()
         {
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 1, IR::ArgBase::Lit);
            moveArgStk_src(stmnt->args[0], stmnt->op.size);
         }

         //
         // Info::trStmnt_Jfar
         //
         void Info::trStmnt_Jfar()
         {
            CheckArgC(stmnt, 1);

            CheckArgB(stmnt, 0, IR::ArgBase::Lit);

            if(stmnt->args.size() > 1)
            {
               CheckArgC(stmnt, 3);
               CheckArgB(stmnt, 1, IR::ArgBase::Sta);
            }
         }

         //
         // Info::trStmnt_Jset
         //
         void Info::trStmnt_Jset()
         {
            CheckArgC(stmnt, 2);
            CheckArgB(stmnt, 0, IR::ArgBase::Sta);
         }

         //
         // Info::trStmnt_Jump
         //
         void Info::trStmnt_Jump()
         {
            CheckArgC(stmnt, 1);
            CheckArgB(stmnt, 0, IR::ArgBase::Lit);
         }
      }
   }
}

// EOF

