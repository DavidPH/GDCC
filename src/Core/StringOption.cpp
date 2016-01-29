//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Core::String option handling.
//
//-----------------------------------------------------------------------------

#include "Core/StringOption.hpp"

#include "Option/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // StringOption constructor
      //
      StringOption::StringOption(Option::Program *program, Info const &optInfo, String *ptr) :
         Option::Base{program, optInfo}, dptr{ptr}
      {
      }

      //
      // StringOption::v_process
      //
      std::size_t StringOption::v_process(Option::Args const &args)
      {
         if(args.optFalse) return *dptr = nullptr, 0;
         if(!args.argC) Option::Exception::Error(args, "argument required");

         *dptr = args.argV[0];

         return 1;
      }
   }
}

// EOF

