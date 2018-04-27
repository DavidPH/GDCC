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
   // Info::trStmnt_Call
   //
   void Info::trStmnt_Call()
   {
      CheckArgC(stmnt, 1);

      moveArgStk_dst(stmnt->args[0]);
      moveArgStk_src(stmnt->args[1]);

      for(auto n = stmnt->args.size(); --n != 1;)
         CheckArgB(stmnt, n, IR::ArgBase::Stk);
   }

   //
   // Info::trStmnt_Cnat
   //
   void Info::trStmnt_Cnat()
   {
      CheckArgC(stmnt, 1);
      CheckArgB(stmnt, 1, IR::ArgBase::Lit);

      moveArgStk_dst(stmnt->args[0]);

      for(auto n = stmnt->args.size(); --n != 1;)
         CheckArgB(stmnt, n, IR::ArgBase::Stk);
   }

   //
   // Info::trStmnt_Retn
   //
   void Info::trStmnt_Retn()
   {
      if(!stmnt->args.empty())
         moveArgStk_src(stmnt->args[0]);
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
}

// EOF

