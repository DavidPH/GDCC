//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Generic translation functions.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      //
      // Info::trStmntStk2
      //
      void Info::trStmntStk2(Core::FastU sizeDst, Core::FastU sizeSrc)
      {
         CheckArgC(stmnt, 2);

         moveArgStk_dst(stmnt->args[0], sizeDst);
         moveArgStk_src(stmnt->args[1], sizeSrc);
      }

      //
      // Info::trStmntStk3
      //
      void Info::trStmntStk3(Core::FastU sizeDst, Core::FastU sizeSrc, bool ordered)
      {
         CheckArgC(stmnt, 3);

         moveArgStk_dst(stmnt->args[0], sizeDst);

         try
         {
            moveArgStk_src(stmnt->args[1], sizeSrc);
         }
         catch(ResetStmnt const &)
         {
            if(ordered && stmnt->next->args[2].a == IR::ArgBase::Stk)
               block->addStatementArgs(stmnt->next, {IR::Code::Swap_W, sizeSrc},
                  IR::Arg_Stk(), IR::Arg_Stk());

            throw;
         }

         moveArgStk_src(stmnt->args[2], sizeSrc);
      }

      //
      // Info::trStmntShift
      //
      bool Info::trStmntShift(Core::FastU size, bool moveLit)
      {
         CheckArgC(stmnt, 3);

         if(stmnt->args[1].a != IR::ArgBase::Stk &&
            stmnt->args[2].a == IR::ArgBase::Stk)
            throw Core::ExceptStr(stmnt->pos, "trStmntShift disorder");

         moveArgStk_dst(stmnt->args[0], size);
         moveArgStk_src(stmnt->args[1], size);

         if(!moveLit && stmnt->args[2].a == IR::ArgBase::Lit)
            return true;

         moveArgStk_src(stmnt->args[2], 1);
         return false;
      }
   }
}

// EOF

