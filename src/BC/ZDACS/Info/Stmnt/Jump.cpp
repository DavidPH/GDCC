//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
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

#include "IR/Exception.hpp"
#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::chkStmnt_Jcnd_Nil
   //
   void Info::chkStmnt_Jcnd_Nil()
   {
      chkStmntArgB(1, IR::ArgBase::Lit);
   }

   //
   // Info::chkStmnt_Jcnd_Tab
   //
   void Info::chkStmnt_Jcnd_Tab()
   {
      if(stmnt->args[0].getSize() != 1)
         IR::ErrorCode(stmnt, "unsupported size");

      for(Core::FastU i = 1; i != stmnt->args.size(); ++i)
         chkStmntArgB(i, IR::ArgBase::Lit);

      // Argument gets left on stack, so do not just move to stack.
      chkStmntArgB(0, IR::ArgBase::Stk);
   }

   //
   // Info::chkStmnt_Jfar_Pro
   //
   void Info::chkStmnt_Jfar_Pro()
   {
      chkStmntArgB(0, IR::ArgBase::Lit);
      chkStmntArgB(1, IR::ArgBase::Stk);
   }

   //
   // Info::chkStmnt_Jfar_Set
   //
   void Info::chkStmnt_Jfar_Set()
   {
      chkStmntArgB(0, IR::ArgBase::Lit);
      chkStmntArgB(1, IR::ArgBase::Sta);
   }

   //
   // Info::chkStmnt_Jfar_Sta
   //
   void Info::chkStmnt_Jfar_Sta()
   {
      chkStmntArgB(0, IR::ArgBase::Lit);
      chkStmntArgB(1, IR::ArgBase::Sta);
   }

   //
   // Info::chkStmnt_Jump
   //
   void Info::chkStmnt_Jump()
   {
      chkStmntArgB(0, IR::ArgBase::Lit);
   }

   //
   // Info::genStmnt_Jcnd_Nil
   //
   void Info::genStmnt_Jcnd_Nil()
   {
      auto n = getStmntSize();

      if(n == 0)
      {
         numChunkCODE += 8;
         return;
      }

      numChunkCODE += n * 4 + 4;
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
      auto n = getStmntSize();

      if(n == 0) return;

      numChunkCODE += n * 4 + 4;
   }

   //
   // Info::genStmnt_Jdyn
   //
   void Info::genStmnt_Jdyn()
   {
      numChunkCODE += 4;
   }

   //
   // Info::genStmnt_Jfar
   //
   void Info::genStmnt_Jfar()
   {
      numChunkCODE += 28 + 8 + (36 + 36 + 24 + 4);
   }

   //
   // Info::genStmnt_Jfar_Pro
   //
   void Info::genStmnt_Jfar_Pro()
   {
      auto n = stmnt->args[1].getSize();

      numChunkCODE += 28;

      if(n)
         numChunkCODE += n * 4 + lenDropTmp(0) + lenPushTmp(0);

      genStmnt_Jfar();
   }

   //
   // Info::genStmnt_Jfar_Set
   //
   void Info::genStmnt_Jfar_Set()
   {
      numChunkCODE += lenPushArg(stmnt->args[0], 0) + lenDropArg(stmnt->args[1], 1);
      numChunkCODE += 8 + lenDropArg(stmnt->args[1], 0);
      numChunkCODE += 8;
   }

   //
   // Info::genStmnt_Jfar_Sta
   //
   void Info::genStmnt_Jfar_Sta()
   {
      numChunkCODE += lenPushArg(stmnt->args[2], 0) + lenDropArg(stmnt->args[1], 2);
      numChunkCODE += 8 + lenPushIdx(stmnt->args[1], 0) + 8;
      numChunkCODE += lenPushIdx(stmnt->args[1], 0);

      genStmnt_Jfar();
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
      auto n = getStmntSize();

      if(n == 0)
      {
         putCode(Code::Jump_Lit, getWord(stmnt->args[1].aLit));
         return;
      }

      for(auto i = n; --i;)
         putCode(Code::BOrI);

      putCode(Code::Jcnd_Nil, getWord(stmnt->args[1].aLit));
   }

   //
   // Info::putStmnt_Jcnd_Tab
   //
   void Info::putStmnt_Jcnd_Tab()
   {
      if(stmnt->args.size() == 3)
      {
         putCode(Code::Jcnd_Lit);
         putWord(getWord(stmnt->args[1].aLit));
         putWord(getWord(stmnt->args[2].aLit));
      }
      else
      {
         putCode(Code::Jcnd_Tab, stmnt->args.size() / 2);

         struct JumpData {Core::FastU value, label;};

         // Collect jump cases.
         Core::Array<JumpData> Jumps{stmnt->args.size() / 2};
         for(Core::FastU i = 0; i != Jumps.size(); ++i)
         {
            Jumps[i].value = getWord(stmnt->args[i * 2 + 1].aLit);
            Jumps[i].label = getWord(stmnt->args[i * 2 + 2].aLit);
         }

         // Sort by value as signed.
         std::sort(Jumps.begin(), Jumps.end(),
            [](JumpData const &l, JumpData const &r) -> bool
            {
               if(l.value & 0x80000000)
                  return r.value & 0x80000000 ? l.value < r.value : true;
               else
                  return r.value & 0x80000000 ? false : l.value < r.value;
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
      auto n = getStmntSize();

      if(n == 0) return;

      for(auto i = n; --i;)
         putCode(Code::BOrI);

      putCode(Code::Jcnd_Tru, getWord(stmnt->args[1].aLit));
   }

   //
   // Info::putStmnt_Jdyn
   //
   void Info::putStmnt_Jdyn()
   {
      putCode(Code::Jdyn);
   }

   //
   // Info::putStmnt_Jfar
   //
   void Info::putStmnt_Jfar()
   {
      // Check auto pointer match.
      putCode(Code::Push_GblArr, StaArray);
      putCode(Code::Push_LocReg, getStkPtrIdx());
      putCode(Code::CmpU_EQ);
      putCode(Code::Jcnd_Tru,    putPos + 8 + 8);

      // Non-match! Branch to addr.
      putCode(Code::Jump_Lit, getWord(stmnt->args[0].aLit.value));

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
   // Info::putStmnt_Jfar_Pro
   //
   void Info::putStmnt_Jfar_Pro()
   {
      auto n = stmnt->args[1].getSize();

      // Check for ongoing far jump.

      putCode(Code::Push_Lit,    FarJumpIndex);
      putCode(Code::Push_GblArr, StaArray);
      putCode(Code::Jcnd_Lit,    0, putPos + 12
         + !!n * 16 + n * 4
         + 28 + 8 + (36 + 36 + 24 + 4));

      if(n)
      {
         putStmntDropTmp(0);

         for(auto i = n; i--;)
            putCode(Code::Drop_Nul);

         putStmntPushTmp(0);
      }

      // Propagate far jump.
      putStmnt_Jfar();
   }

   //
   // Info::putStmnt_Jfar_Set
   //
   void Info::putStmnt_Jfar_Set()
   {
      putStmntPushArg(stmnt->args[0], 0);
      putStmntDropArg(stmnt->args[1], 1);

      putCode(Code::Push_LocReg, getStkPtrIdx());
      putStmntDropArg(stmnt->args[1], 0);

      putCode(Code::Push_Lit, 0);
   }

   //
   // Info::putStmnt_Jfar_Sta
   //
   void Info::putStmnt_Jfar_Sta()
   {
      // Initiate far jump.

      putStmntPushArg(stmnt->args[2], 0);
      putStmntDropArg(stmnt->args[1], 2);

      putCode(Code::Push_Lit,    FarJumpIndex);
      putStmntPushIdx(stmnt->args[1], 0);
      putCode(Code::Drop_GblArr, StaArray);

      putStmntPushIdx(stmnt->args[1], 0);

      // Propagate far jump.
      putStmnt_Jfar();
   }

   //
   // Info::putStmnt_Jump
   //
   void Info::putStmnt_Jump()
   {
      putCode(Code::Jump_Lit, getWord(stmnt->args[0].aLit));
   }

   //
   // Info::trStmnt_Jcnd_Nil
   //
   void Info::trStmnt_Jcnd_Nil()
   {
      moveArgStk_src(stmnt->args[0]);
   }

   //
   // Info::trStmnt_Jdyn
   //
   void Info::trStmnt_Jdyn()
   {
      moveArgStk_src(stmnt->args[0]);
   }

   //
   // Info::trStmnt_Jfar_Pro
   //
   void Info::trStmnt_Jfar_Pro()
   {
      if(stmnt->args[1].getSize())
         trStmntTmp(1);
   }
}

// EOF

