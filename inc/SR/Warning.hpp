//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Common language warnings
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Warning_H__
#define GDCC__SR__Warning_H__

#include "../SR/Types.hpp"

#include "../Core/Warning.hpp"


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC::SR
{
   extern Core::Warning WarnDeprecated;
   extern Core::Warning WarnParentheses;
   extern Core::Warning WarnReturnType;
   extern Core::Warning WarnReturnTypeExt;
   extern Core::Warning WarnUnusedInit;
   extern Core::Warning WarnUnusedValue;
}

#endif//GDCC__SR__Warning_H__

