//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C warnings
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Warning_H__
#define GDCC__CC__Warning_H__

#include "../CC/Types.hpp"

#include "../Core/Warning.hpp"


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC::CC
{
   extern Core::Warning WarnDeclCompat;
   extern Core::Warning WarnDelayCall;
   extern Core::Warning WarnFileSemico;
   extern Core::Warning WarnForwardRef;
   extern Core::Warning WarnUnknownAttr;
}

#endif//GDCC__CC__Warning_H__

