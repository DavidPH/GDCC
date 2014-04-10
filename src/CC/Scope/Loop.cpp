//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C break/continue block scope handling.
//
//-----------------------------------------------------------------------------

#include "CC/Scope/Loop.hpp"

#include "CC/Scope/Function.hpp"

#include "AST/Function.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Scope_Loop constructor
      //
      Scope_Loop::Scope_Loop(Scope_Local &parent_) :
         Scope_Break{parent_},

         labelContinue{Core::STRNULL}
      {
      }

      //
      // Scope_Loop::getLabelContinue
      //
      Core::String Scope_Loop::getLabelContinue()
      {
         if(!labelContinue)
            labelContinue = fn.fn->genLabel();

         return labelContinue;
      }
   }
}

//EOF

