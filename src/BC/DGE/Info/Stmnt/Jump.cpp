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
   }
}

// EOF

