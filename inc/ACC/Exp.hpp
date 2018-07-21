//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__Exp_H__
#define GDCC__ACC__Exp_H__

#include "../ACC/Types.hpp"

#include "../Core/Counter.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   // Conversion as if by assignment.
   Core::CounterRef<SR::Exp const> ExpPromo_Assign(SR::Type const *t,
      SR::Exp const *e, Core::Origin pos);
}

#endif//GDCC__ACC__Exp_H__

