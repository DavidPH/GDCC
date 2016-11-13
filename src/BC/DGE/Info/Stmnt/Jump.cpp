//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Jump statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::putStmnt_Jcnd_Nil
   //
   void Info::putStmnt_Jcnd_Nil(char const *code)
   {
      if(stmnt->op.size == 0)
         return putCode("Jump_Lit", stmnt->args[1].aLit);

      for(auto n = stmnt->op.size; --n;)
         putCode("BOrI");

      putCode(code, stmnt->args[1].aLit);
   }

   //
   // Info::putStmnt_Jcnd_Tab
   //
   void Info::putStmnt_Jcnd_Tab()
   {
      if(stmnt->args.size() == 1)
         return;

      if(stmnt->args.size() == 3)
         return putCode("Jcnd_Lit", stmnt->args[1].aLit, stmnt->args[2].aLit);

      putNTS("Jcnd_Tab");
      putNTS('(');

      // Write sorted jump cases.
      putCodeArg(stmnt->args[1].aLit); putNTS(',');
      putCodeArg(stmnt->args[2].aLit);
      for(Core::FastU i = 3, e = stmnt->args.size(); i != e;)
      {
         putNTS(','); putCodeArg(stmnt->args[i++].aLit);
         putNTS(','); putCodeArg(stmnt->args[i++].aLit);
      }

      putNTS(')');
   }

   //
   // Info::putStmnt_Jcnd_Tru
   //
   void Info::putStmnt_Jcnd_Tru()
   {
      if(stmnt->op.size == 0)
         return;

      putStmnt_Jcnd_Nil("Jcnd_Tru");
   }

   //
   // Info::putStmnt_Jfar
   //
   void Info::putStmnt_Jfar()
   {
      if(stmnt->args.size() > 1)
      {
         // Initiate far jump.

         // TODO
      }
      else
      {
         // Check for ongoing far jump.

         // TODO
      }
   }

   //
   // Info::putStmnt_Jump
   //
   void Info::putStmnt_Jump()
   {
      if(stmnt->args[0].a == IR::ArgBase::Lit)
         putCode("Jump_Lit", stmnt->args[0].aLit);
      else
         putCode("Jump");
   }

   //
   // Info::trStmnt_Jcnd_Nil
   //
   void Info::trStmnt_Jcnd_Nil()
   {
      CheckArgC(stmnt, 2);
      moveArgStk_src(stmnt->args[0], stmnt->op.size);
      CheckArgB(stmnt, 1, IR::ArgBase::Lit);
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
      trStmnt_Jcnd_Nil();
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
   // Info::trStmnt_Jump
   //
   void Info::trStmnt_Jump()
   {
      CheckArgC(stmnt, 1);
      if(stmnt->args[0].a != IR::ArgBase::Lit)
         moveArgStk_src(stmnt->args[0], stmnt->op.size);
   }
}

// EOF

