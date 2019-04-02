//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Call statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "IR/Arg.hpp"
#include "IR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::chkStmnt_Call
   //
   void Info::chkStmnt_Call()
   {
      for(auto n = stmnt->args.size(); --n != 1;)
         chkStmntArgB(n, IR::ArgBase::Stk);
   }

   //
   // Info::chkStmnt_Cnat
   //
   void Info::chkStmnt_Cnat()
   {
      chkStmntArgB(1, IR::ArgBase::Lit);

      for(auto n = stmnt->args.size(); --n != 1;)
         chkStmntArgB(n, IR::ArgBase::Stk);
   }

   //
   // Info::putStmnt_Call
   //
   void Info::putStmnt_Call()
   {
      putCode("Call", getStmntSizeW());
   }

   //
   // Info::putStmnt_Cnat
   //
   void Info::putStmnt_Cnat()
   {
      putCode("Cnat", getStmntSizeW(), stmnt->args[1].aLit);
   }

   //
   // Info::putStmnt_Retn
   //
   void Info::putStmnt_Retn()
   {
      if(func->allocAut)
      {
         putCode("Push_Reg", getStkPtrIdx());
         putCode("Push_Lit", "___GDCC__Plsf");
         putCode("Call",     1);
      }

      putCode("Retn");
   }

   //
   // Info::trStmnt_Call
   //
   void Info::trStmnt_Call()
   {
      moveArgStk_dst(stmnt->args[0]);
      moveArgStk_src(stmnt->args[1]);
   }

   //
   // Info::trStmnt_Cnat
   //
   void Info::trStmnt_Cnat()
   {
      moveArgStk_dst(stmnt->args[0]);
   }

   //
   // Info::trStmnt_Retn
   //
   void Info::trStmnt_Retn()
   {
      if(!stmnt->args.empty())
         moveArgStk_src(stmnt->args[0]);
   }
}

// EOF

