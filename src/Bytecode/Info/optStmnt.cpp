//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Generic statement optimizations.
//
//-----------------------------------------------------------------------------

#include "Bytecode/Info.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      //
      // Info::optStmnt
      //
      void Info::optStmnt()
      {
         optStmnt_Cspe_Drop();
      }

      //
      // Info::optStmnt_Cspe_Drop
      //
      // The sequence:
      //    Cspe   N, ...
      //    Move_W N, Nul(), Stk()
      // Can be transformed into:
      //    Cspe   0, ...
      //
      bool Info::optStmnt_Cspe_Drop()
      {
         // Must be Cspe Move_W sequence.
         if(stmnt->op.code != IR::Code::Cspe || stmnt->next->op.code != IR::Code::Move_W)
            return false;

         // Must have no label on subsequent statements.
         if(!stmnt->next->labs.empty())
            return false;

         // Must have same op size.
         if(stmnt->op.size != stmnt->next->op.size)
            return false;

         // Transform sequence.
         stmnt->op.size = 0;
         delete stmnt->next;

         return true;
      }
   }
}

// EOF

