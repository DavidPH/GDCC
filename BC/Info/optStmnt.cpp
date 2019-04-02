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
   static Option::Bool OptPass
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("bc-opt")
         .setGroup("codegen")
         .setDescS("Enables or disables opt pass."),

      true
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
      optStmnt_LNot_Jcnd();
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
      if(stmnt->code != IR::CodeBase::Cspe || stmnt->args[0].a != IR::ArgBase::Stk)
         return false;

      auto next = stmnt->next;

      // Must have no label on subsequent statement.
      if(!next->labs.empty())
         return false;

      // Must be followed by a Move(Nul() Stk()).
      if(next->code != IR::CodeBase::Move ||
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
      // Must be Jump(Lit(...)).
      if(stmnt->code != IR::CodeBase::Jump || stmnt->args[0].a != IR::ArgBase::Lit)
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

   //
   // Info::optStmnt_LNot_Jcnd
   //
   // The squence:
   //    LNot(Stk N() ArgX)
   //    Jcnd_Nil(Stk N() ...)
   // Can be transformed into:
   //    Jcnd_Tru(ArgX ...)
   //
   bool Info::optStmnt_LNot_Jcnd()
   {
      // Must be LNot(Stk N() ...).
      if(stmnt->code != IR::CodeBase::LNot || stmnt->args[0].a != IR::ArgBase::Stk)
         return false;

      auto next = stmnt->next;

      // Must have no label on subsequent statement.
      if(!next->labs.empty())
         return false;

      // Must be followed by Jcnd_Nil(Stk N() ...).
      if((next->code != IR::CodeBase::Jcnd_Nil && next->code != IR::CodeBase::Jcnd_Tru) ||
         next->args[0].a != IR::ArgBase::Stk)
         return false;

      // Must have the same size for all three args.
      auto n = stmnt->args[0].aStk.size;
      if(stmnt->args[1].getSize() != n || next->args[0].aStk.size != n)
         return false;

      // Invert jump condition and move src.
      next->code = next->code == IR::CodeBase::Jcnd_Nil
         ? IR::CodeBase::Jcnd_Tru : IR::CodeBase::Jcnd_Nil;

      next->args[0] = std::move(stmnt->args[1]);

      // Remove LNot.
      if(!stmnt->labs.empty())
         next->labs += stmnt->labs;
      stmnt = stmnt->prev;
      delete stmnt->next;

      return true;
   }
}

// EOF

