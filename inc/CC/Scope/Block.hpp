//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C block scope handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Scope__Block_H__
#define GDCC__CC__Scope__Block_H__

#include "../../CC/Scope/Local.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Scope_Block
   //
   class Scope_Block : public Scope_Local
   {
   public:
      explicit Scope_Block(Scope_Local &parent);

      AllocAutoInfo allocAuto(AllocAutoInfo const &base);
   };
}

#endif//GDCC__CC__Scope__Block_H__

