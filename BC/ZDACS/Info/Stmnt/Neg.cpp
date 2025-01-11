//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
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
      auto n = getStmntSize();

      if(n == 0)
         return;

      if(stmnt->args[0].a != IR::ArgBase::Stk)
      {
         if(stmnt->args[1].a != IR::ArgBase::Stk)
         {
            genStmntDropArgPre(stmnt->args[0], n - 1);
            genStmntPushArg(stmnt->args[1], n - 1);
            genCode(Code::Push_Lit, 0x80000000);
            genCode(Code::BOrX);
            genStmntDropArgSuf(stmnt->args[0], n - 1);

            if(stmnt->args[0] != stmnt->args[1] && n > 1)
               for(Core::FastU i = n - 1; i--;)
            {
               genStmntDropArgPre(stmnt->args[0], i);
               genStmntPushArg(stmnt->args[1], i);
               genStmntDropArgSuf(stmnt->args[0], i);
            }
         }
         else
         {
            genCode(Code::Push_Lit, 0x80000000);
            genCode(Code::BOrX);
            genStmntDropArg(stmnt->args[0]);
         }
      }
      else
      {
         if(stmnt->args[1].a != IR::ArgBase::Stk)
         {
            genStmntPushArg(stmnt->args[1]);
            genCode(Code::Push_Lit, 0x80000000);
            genCode(Code::BOrX);
         }
         else
         {
            genCode(Code::Push_Lit, 0x80000000);
            genCode(Code::BOrX);
         }
      }
   }

   //
   // Info::genStmnt_Neg_I
   //
   void Info::genStmnt_Neg_I()
   {
      auto n = getStmntSize();

      if(n <= 1)
         return genStmntArg('I', Code::NegI);

      if(stmnt->args[1].a != IR::ArgBase::Stk)
         genStmntPushArg(stmnt->args[1]);

      if(!isCopyArg(stmnt->args[0]) || !isFastArg(stmnt->args[0]))
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

         if(stmnt->args[0].a == IR::ArgBase::Stk)
         {
            for(Core::FastU i = 0; i != n; ++i)
               genStmntPushTmp(i);
         }
         else
         {
            for(Core::FastU i = 0; i != n; ++i)
            {
               genStmntDropArgPre(stmnt->args[0], i);
               genStmntPushTmp(i);
               genStmntDropArgSuf(stmnt->args[0], i);
            }
         }
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

      if((!isCopyArg(stmnt->args[0]) || !isFastArg(stmnt->args[0])) && n > 1)
         trStmntTmp(n);
   }
}

// EOF

