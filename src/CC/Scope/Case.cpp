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
         caseGen{caseDef.label, "$C$"}
      {
      }

      //
      // Scope_Case::getLabelCase
      //
      Core::String Scope_Case::getLabelCase(Core::Integ const &n, bool defin)
      {
         // Find existing case.
         auto itr = caseSet.find(n);

         // If no such case yet, add it.
         if(itr == caseSet.end())
         {
            itr = caseSet.emplace(std::piecewise_construct,
               std::forward_as_tuple(n),
               std::forward_as_tuple(n, caseGen())
               ).first;
         }

         // Mark as definition, or fail if already defined.
         if(defin)
         {
            if(itr->second.defin)
               return Core::STRNULL;

            itr->second.defin = true;
         }

         // Return case label.
         return itr->second.label;
      }

      //
      // Scope_Case::getLabelDefault
      //
      Core::String Scope_Case::getLabelDefault(bool defin)
      {
         if(defin)
         {
            if(caseDef.defin)
               return Core::STRNULL;

            caseDef.defin = true;
         }

         return caseDef.label;
      }
   }
}

//EOF

