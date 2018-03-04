//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Generic statement optimizations.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
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
   //    Cspe(Stk N() ...)
   //    Move(Nul N() Stk N())
   // Can be transformed into:
   //    Cspe(Nul N() ...)
   //
   bool Info::optStmnt_Cspe_Drop()
   {
      // Must be Cspe(Stk() ...).
      if(stmnt->code != IR::Code::Cspe || stmnt->args.size() < 1 ||
         stmnt->args[0].a != IR::ArgBase::Stk)
         return false;

      auto next = stmnt->next;

      // Must have no label on subsequent statements.
      if(!next->labs.empty())
         return false;

      // Must be followed by a Move(Nul() Stk()).
      if(next->code != IR::Code::Move_W || next->args.size() != 2 ||
         next->args[0].a != IR::ArgBase::Nul || next->args[1].a != IR::ArgBase::Stk)
         return false;

      // Must have the same size for all three args.
      auto size = stmnt->args[0].aStk.size;
      if(next->args[0].aNul.size != size || next->args[1].aStk.size != size)
         return false;

      // Transform sequence.
      stmnt->args[0] = std::move(next->args[0]);
      delete next;

      return true;
   }
}

// EOF

