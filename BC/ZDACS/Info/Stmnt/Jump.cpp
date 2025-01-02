//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
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
#include "BC/ZDACS/Module.hpp"

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
         return genCode(Code::Jump_Lit, stmnt->args[1].aLit.value);

      for(auto i = n; --i;)
         genCode(Code::BOrI);

      genCode(Code::Jcnd_Nil, stmnt->args[1].aLit.value);
   }

   //
   // Info::genStmnt_Jcnd_Tab
   //
   void Info::genStmnt_Jcnd_Tab()
   {
      if(stmnt->args.size() == 3)
      {
         genCode(Code::Jcnd_Lit, stmnt->args[1].aLit.value, stmnt->args[2].aLit.value);
      }
      else
      {
         ElemArgs args{stmnt->args.size()};

         args[0] = args.size() / 2;

         // Table sorting is handled later when symbols can be resolved.
         for(std::size_t i = 1; i != args.size(); ++i)
            args[i] = stmnt->args[i].aLit.value;

         genCode(Code::Jcnd_Tab, std::move(args));
      }
   }

   //
   // Info::genStmnt_Jcnd_Tru
   //
   void Info::genStmnt_Jcnd_Tru()
   {
      auto n = getStmntSize();

      if(n == 0) return;

      for(auto i = n; --i;)
         genCode(Code::BOrI);

      genCode(Code::Jcnd_Tru, stmnt->args[1].aLit.value);
   }

   //
   // Info::genStmnt_Jdyn
   //
   void Info::genStmnt_Jdyn()
   {
      genCode(Code::Jdyn);
   }

   //
   // Info::genStmnt_Jfar
   //
   void Info::genStmnt_Jfar()
   {
      // Check auto pointer match.
      genCode(Code::Push_GblArr, StaArray);
      genCode(Code::Push_LocReg, getStkPtrIdx());
      genCode(Code::CmpU_EQ);
      auto jump = module->chunkCODE.size();
      genCode(Code::Jcnd_Tru,    0);

      // Non-match! Branch to addr.
      genCode(Code::Jump_Lit, stmnt->args[0].aLit.value);

      // Match! Push result and execute dynamic branch.
      module->chunkCODE[jump].args[0] = getCodePos();
      genCode(Code::Push_Lit,    FarJumpIndex);
      genCode(Code::Push_GblArr, StaArray);
      genCode(Code::Push_Lit,    2);
      genCode(Code::AddU);
      genCode(Code::Push_GblArr, StaArray);

      genCode(Code::Push_Lit,    FarJumpIndex);
      genCode(Code::Push_GblArr, StaArray);
      genCode(Code::Push_Lit,    1);
      genCode(Code::AddU);
      genCode(Code::Push_GblArr, StaArray);

      genCode(Code::Push_Lit,    FarJumpIndex);
      genCode(Code::Push_Lit,    0);
      genCode(Code::Drop_GblArr, StaArray);

      genCode(Code::Jdyn);
   }

   //
   // Info::genStmnt_Jfar_Pro
   //
   void Info::genStmnt_Jfar_Pro()
   {
      auto n = stmnt->args[1].getSize();

      if(isFuncJfar_Set(func))
      {
         // Check for ongoing far jump.
         genCode(Code::Push_Lit,    FarJumpIndex);
         genCode(Code::Push_GblArr, StaArray);
         auto jump = module->chunkCODE.size();
         genCode(Code::Jcnd_Lit,    0, 0);

         if(n)
         {
            genStmntDropTmp(0);

            for(auto i = n; i--;)
               genCode(Code::Drop_Nul);

            genStmntPushTmp(0);
         }

         // Propagate far jump.
         genStmnt_Jfar();

         module->chunkCODE[jump].args[1] = getCodePos();
      }
      else
      {
         // Check for ongoing far jump.
         genCode(Code::Push_Lit,    FarJumpIndex);
         genCode(Code::Push_GblArr, StaArray);

         if(n)
         {
            auto jump = module->chunkCODE.size();
            genCode(Code::Jcnd_Nil, 0);

            for(auto i = n; i--;)
               genCode(Code::Drop_Nul);

            genCode(Code::Jump_Lit, stmnt->args[0].aLit.value);
            module->chunkCODE[jump].args[0] = getCodePos();
         }
         else
            genCode(Code::Jcnd_Tru, stmnt->args[0].aLit.value);
      }
   }

   //
   // Info::genStmnt_Jfar_Set
   //
   void Info::genStmnt_Jfar_Set()
   {
      genStmntPushArg(stmnt->args[0], 0);
      genStmntDropArg(stmnt->args[1], 1);

      genCode(Code::Push_LocReg, getStkPtrIdx());
      genStmntDropArg(stmnt->args[1], 0);

      genCode(Code::Push_Lit, 0);
   }

   //
   // Info::genStmnt_Jfar_Sta
   //
   void Info::genStmnt_Jfar_Sta()
   {
      // Initiate far jump.

      genStmntPushArg(stmnt->args[2], 0);
      genStmntDropArg(stmnt->args[1], 2);

      genCode(Code::Push_Lit,    FarJumpIndex);
      genStmntPushIdx(stmnt->args[1], 0);
      genCode(Code::Drop_GblArr, StaArray);

      if(isFuncJfar_Set(func))
      {
         genStmntPushIdx(stmnt->args[1], 0);

         // Propagate far jump.
         genStmnt_Jfar();
      }
      else
         genCode(Code::Jump_Lit, stmnt->args[0].aLit.value);
   }

   //
   // Info::genStmnt_Jump
   //
   void Info::genStmnt_Jump()
   {
      genCode(Code::Jump_Lit, stmnt->args[0].aLit.value);
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

