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
      Scope_Global::Scope_Global(Core::String label_) :
         CC::Scope_Global{label_}
      {
      }

      //
      // Scope_Global::addCreateTrans
      //
      void Scope_Global::addCreateTrans(Core::String name, CreateTransDecl &&ctrans)
      {
         globalCreateTrans.emplace(std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(ctrans));
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
      // Scope_Global::findCreateTrans
      //
      CreateTransDecl *Scope_Global::findCreateTrans(Core::String name)
      {
         auto ctrans = globalCreateTrans.find(name);
         return ctrans == globalCreateTrans.end() ? nullptr : &ctrans->second;
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

