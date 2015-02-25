//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS identifier scoping/lookup handling.
//
//-----------------------------------------------------------------------------

#include "ACC/Scope.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // Scope_Global constructor
      //
      Scope_Global::Scope_Global(Core::String label) :
         CC::Scope_Global{label}
      {
      }

      //
      // Scope_Global::addPrint
      //
      void Scope_Global::addPrint(Core::String name, PrintDecl &&print)
      {
         globalPrint.emplace(std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(print));
      }

      //
      // Scope_Global::findPrint
      //
      PrintDecl *Scope_Global::findPrint(Core::String name)
      {
         auto print = globalPrint.find(name);
         return print == globalPrint.end() ? nullptr : &print->second;
      }
   }
}

// EOF

