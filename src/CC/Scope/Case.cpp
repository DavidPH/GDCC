//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C case block scope handling.
//
//-----------------------------------------------------------------------------

#include "CC/Scope/Case.hpp"

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
      // Scope_Case constructor
      //
      Scope_Case::Scope_Case(Scope_Local &parent_) :
         Scope_Break{parent_},

         caseDef{fn.fn->genLabel()},
         caseGen{caseDef, "$C$"}
      {
      }

      //
      // Scope_Case::getLabelCase
      //
      Core::String Scope_Case::getLabelCase(Core::Integ const &n, bool)
      {
         // FIXME: TODO: Check for redefinition.
         // FIXME: FastU is not enough!
         return caseGen(number_cast<Core::FastU>(n));
      }

      //
      // Scope_Case::getLabelDefault
      //
      Core::String Scope_Case::getLabelDefault(bool)
      {
         return caseDef;
      }
   }
}

//EOF

