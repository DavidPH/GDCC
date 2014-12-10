//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Common language warnings
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Warning_H__
#define GDCC__AST__Warning_H__

#include "../Core/Warning.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      class Warning;
   }
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace AST
   {
      extern Core::Warning WarnDeprecated;
      extern Core::Warning WarnReturnType;
   }
}

#endif//GDCC__AST__Warning_H__

