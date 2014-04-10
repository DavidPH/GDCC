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

#ifndef GDCC__CC__Scope__Case_H__
#define GDCC__CC__Scope__Case_H__

#include "../../CC/Scope/Break.hpp"

#include "../../Core/StringGen.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Scope_Case
      //
      // A scope that supports case labels. In C, switch scopes.
      //
      class Scope_Case : public Scope_Break
      {
      public:
         explicit Scope_Case(Scope_Local &parent);

         virtual Core::String getLabelCase(Core::Integ const &n, bool define);
         virtual Core::String getLabelDefault(bool define);

      protected:
         Core::String    caseDef;
         Core::StringGen caseGen;
      };
   }
}

#endif//GDCC__CC__Scope__Case_H__

