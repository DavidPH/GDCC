//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
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
#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      namespace DGE
      {
         //
         // Info::trStmnt_Call
         //
         void Info::trStmnt_Call()
         {
            CheckArgC(stmnt, 1);

            moveArgStk_src(stmnt->args[0], 1);

            for(auto n = stmnt->args.size(); --n != 0;)
               CheckArgB(stmnt, n, IR::ArgBase::Stk);
         }

         //
         // Info::trStmnt_Cnat
         //
         void Info::trStmnt_Cnat()
         {
            CheckArgC(stmnt, 1);
            CheckArgB(stmnt, 0, IR::ArgBase::Lit);

            for(auto n = stmnt->args.size(); --n != 0;)
               CheckArgB(stmnt, n, IR::ArgBase::Stk);
         }

         //
         // Info::trStmnt_Retn
         //
         void Info::trStmnt_Retn()
         {
            if(stmnt->op.size)
            {
               CheckArgC(stmnt, 1);
               moveArgStk_src(stmnt->args[0], stmnt->op.size);
            }
         }

         //
         // Info::putStmnt_Call
         //
         void Info::putStmnt_Call()
         {
            putCode("Call", stmnt->args.size() - 1);
         }

         //
         // Info::putStmnt_Cnat
         //
         void Info::putStmnt_Cnat()
         {
            putCode("Cnat", stmnt->args.size() - 1, stmnt->args[0].aLit);
         }

         //
         // Info::putStmnt_Retn
         //
         void Info::putStmnt_Retn()
         {
            putCode("Retn");
         }
      }
   }
}

// EOF

