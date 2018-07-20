//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C break block scope handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Scope__Break_H__
#define GDCC__CC__Scope__Break_H__

#include "../../CC/Scope/Block.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Scope_Break
   //
   // A scope that supports break. In C, do, for, while, and switch scopes.
   //
   class Scope_Break : public Scope_Block
   {
   public:
      explicit Scope_Break(Scope_Local &parent);

      virtual Core::String getLabelBreak();

   protected:
      Core::String labelBreak;
   };
}

#endif//GDCC__CC__Scope__Break_H__

