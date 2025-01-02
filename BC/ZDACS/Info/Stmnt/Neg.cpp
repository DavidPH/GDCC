//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2024 David Hill
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
#include "BC/ZDACS/Module.hpp"

#include "Core/Exception.hpp"

#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   GDCC_BC_CodeTypeSwitchFn(gen, Neg, FIU)

   GDCC_BC_CodeTypeSwitchFn(tr, Neg, FIU)

   //
   // Info::genStmnt_Neg_F
   //
   void Info::genStmnt_Neg_F()
   {
      genCode(Code::Push_Lit, 0x80000000);
      genCode(Code::BOrX);
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
         genCode(Code::NegI);

         if(stmnt->args[0].a != IR::ArgBase::Stk)
            genStmntDropArg(stmnt->args[0], 0);

         return;
      }

      if(stmnt->args[0].a == IR::ArgBase::Stk)
      {
         for(auto i = n; i--;)
         {
            genCode(Code::BNot);
            genStmntDropTmp(i);
         }

         genStmntIncUTmp(0);

         Core::Array<std::size_t> jumps{Core::Size, n - 1};
         for(Core::FastU i = 0, e = n - 1; i != e; ++i)
         {
            genStmntPushTmp(i);
            jumps[i] = module->chunkCODE.size();
            genCode(Code::Jcnd_Tru, 0);
            genStmntIncUTmp(i + 1);
         }

         for(auto const &jump : jumps)
            module->chunkCODE[jump].args[0] = getCodePos();

         for(Core::FastU i = 0; i != n; ++i)
            genStmntPushTmp(i);
      }
      else
      {
         for(auto i = n; i--;)
         {
            genCode(Code::BNot);
            genStmntDropArg(stmnt->args[0], i);
         }

         genStmntIncUArg(stmnt->args[0], 0);

         Core::Array<std::size_t> jumps{Core::Size, n - 1};
         for(Core::FastU i = 0, e = n - 1; i != e; ++i)
         {
            genStmntPushArg(stmnt->args[0], i);
            jumps[i] = module->chunkCODE.size();
            genCode(Code::Jcnd_Tru, 0);
            genStmntIncUArg(stmnt->args[0], i + 1);
         }

         for(auto &jump : jumps)
            module->chunkCODE[jump].args[0] = getCodePos();
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

