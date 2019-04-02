//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
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

namespace GDCC::Core
{
   //
   // StringOption constructor
   //
   StringOption::StringOption(Option::Program *program, Info const &optInfo) :
      Option::Base{program, optInfo}, str{nullptr}
   {
   }

   //
   // StringOption constructor
   //
   StringOption::StringOption(Option::Program *program, Info const &optInfo, String s) :
      Option::Base{program, optInfo}, str{s}
   {
   }

   //
   // StringOption::v_process
   //
   std::size_t StringOption::v_process(Option::Args const &args)
   {
      if(args.optFalse) return str = nullptr, 0;
      if(!args.argC) Option::Exception::Error(args, "argument required");

      str = args.argV[0];

      return 1;
   }
}

// EOF

