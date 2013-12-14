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

#ifndef GDCC__Core__Option_H__
#define GDCC__Core__Option_H__

#include "../Option/Option.hpp"


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace Option
   {
      extern OptionCall Help;
      extern OptionCStr Output;
      extern OptionCall Version;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      void InitOptions(int argc, char const *const *argv, char const *program,
         bool needOutput = true);
   }
}

#endif//GDCC__Core__Option_H__

