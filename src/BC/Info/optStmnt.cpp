//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Generic statement optimizations.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "Core/Option.hpp"

#include "IR/Exp/Glyph.hpp"
#include "IR/Statement.hpp"

#include "Option/Bool.hpp"

#include <algorithm>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::BC
{
   //
   // --bc-opt
   //
   static bool OptPass = true;
   static Option::Bool OptionOpt
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-opt")
         .setGroup("codegen")
         .setDescS("Enables or disables opt pass."),

      &OptPass
   };
}


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
      if(!OptPass)
         return;

      optStmnt_Cspe_Drop();
      optStmnt_JumpNext();
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
      if(next->code != IR::Code::Move || next->args.size() != 2 ||
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

   //
   // Info::optStmnt_JumpNext
   //
   // The sequence:
   //    Jump(Lit("label"))
   //    "label"
   //    ....
   // Can be transformed into:
   //    "label"
   //    ....
   //
   bool Info::optStmnt_JumpNext()
   {
      // Must be Jump(Lit(...))
      if(stmnt->code != IR::Code::Jump || stmnt->args[0].a != IR::ArgBase::Lit)
         return false;

      // Argument must be a glyph expression.
      auto jumpExp = dynamic_cast<IR::Exp_Glyph const *>(&*stmnt->args[0].aLit.value);
      if(!jumpExp)
         return false;

      Core::String jumpStr = jumpExp->glyph;

      auto next = stmnt->next;

      // Check if next has the target label.
      if(std::find(next->labs.begin(), next->labs.end(), jumpStr) == next->labs.end())
         return false;

      // Remove branch.
      if(!stmnt->labs.empty())
         next->labs += stmnt->labs;
      stmnt = stmnt->prev;
      delete stmnt->next;

      return true;
   }
}

// EOF

