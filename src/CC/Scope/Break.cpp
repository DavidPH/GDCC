//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C break block scope handling.
//
//-----------------------------------------------------------------------------

#include "CC/Scope/Break.hpp"

#include "CC/Scope/Function.hpp"

#include "SR/Function.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Scope_Break constructor
      //
      Scope_Break::Scope_Break(Scope_Local &parent_) :
         Scope_Block{parent_},

         labelBreak{Core::STRNULL}
      {
      }

      //
      // Scope_Break::getLabelBreak
      //
      Core::String Scope_Break::getLabelBreak()
      {
         if(!labelBreak)
            labelBreak = fn.fn->genLabel();

         return labelBreak;
      }
   }
}

//EOF

