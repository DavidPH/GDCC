//-----------------------------------------------------------------------------
//
// Copyright (C) 2018-2022 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Common typedefs and class forward declarations.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option__Types_H__
#define GDCC__Option__Types_H__

#include "GDCC/Config.hpp"

#include <cstddef>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Option
{
   class Args;
   class Base;
   class Bool;
   class CStr;
   class CStrV;
   class Exception;
   class Function;
   template<typename T>
   class Int;
   template<typename Str, typename Int>
   class IntMap;
   class Program;
}

#endif//GDCC__Option__Types_H__

