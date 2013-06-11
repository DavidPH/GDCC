//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Common GDCC option handling.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Option_H__
#define GDCC__Option_H__

#include "Option/Option.hpp"


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace Option
{
   extern OptionCall Help;
   extern OptionCStr Output;
   extern OptionCall Version;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   void InitOptions(int argc, char const *const *argv, char const *program,
      char const *version);
}

#endif//GDCC__Option_H__

