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

#include "Core/Option.hpp"

#include <cstdlib>
#include <iostream>


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace Option
   {
      //
      // -h, --help
      //
      OptionCall Help{'h', "help", nullptr, "Prints usage and exits.", nullptr,
         [](strp, uint, uint, strv) -> uint
            {Option::PrintHelp(std::cout); std::exit(EXIT_SUCCESS);}};

      //
      // -o, --output
      //
      OptionCStr Output{'o', "output", "output", "Sets output file or directory.", nullptr};

      //
      // --version
      //
      OptionCall Version{'\0', "version", nullptr, "Prints version and exits.",
         nullptr, [](strp, uint, uint, strv) -> uint
            {Option::PrintVersion(std::cout); std::exit(EXIT_SUCCESS);}};
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // InitOptions
      //
      void InitOptions(int argc, char const *const *argv, char const *program,
         bool needOutput)
      {
         if(program) Option::Option::Help_Program = program;

         Option::Option::Help_Version = "v0.3.0";

         if(argc <= 1)
         {
            Option::Option::PrintHelp(std::cout);
            throw EXIT_SUCCESS;
         }

         try
         {
            Option::Option::ProcessOptions(argc - 1, argv + 1, Option::OPTF_KEEPA);

            // Default output to last loose arg.
            if(needOutput && !Option::Output.handled && *Option::ArgC)
               Option::DefaultArgHandler.argPop(&Option::Output);
         }
         catch(Option::Exception const &e)
         {
            Option::Option::PrintHelp(std::cerr << e.what() << "\n\n");
            throw EXIT_FAILURE;
         }

         if(needOutput && (!Option::Output.handled || !Option::Output.data))
         {
            std::cerr << "No output specified. Use -h for usage.\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF
