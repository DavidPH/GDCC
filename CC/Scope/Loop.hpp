//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C break/continue block scope handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Scope__Loop_H__
#define GDCC__CC__Scope__Loop_H__

#include "../../CC/Scope/Break.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Scope_Loop
   //
   // A scope that supports break and continue. In C, do, for, and while scopes.
   //
   class Scope_Loop : public Scope_Break
   {
   public:
      explicit Scope_Loop(Scope_Local &parent);

      virtual Core::String getLabelContinue();

   protected:
      Core::String labelContinue;
   };
}

#endif//GDCC__CC__Scope__Loop_H__

