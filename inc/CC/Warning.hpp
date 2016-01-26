//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2016 David Hill
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

#include "../Core/Warning.hpp"


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC
{
   namespace CC
   {
      extern Core::Warning WarnDeclCompat;
      extern Core::Warning WarnFileSemico;
      extern Core::Warning WarnForwardRef;
   }
}

#endif//GDCC__CC__Warning_H__

