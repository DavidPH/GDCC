//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Common GDCC option handling.
//
//-----------------------------------------------------------------------------

#include "Core/Option.hpp"

#include "Core/Path.hpp"

#include "Option/Exception.hpp"

#include <cstdlib>
#include <iostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // OptionList constructor
      //
      OptionList::OptionList() :
         list{},

         args{nullptr, Option::Base::Info(), 0},

         optHelp
         {
            &list, Option::Base::Info()
               .setName("help").setName('h')
               .setDescS("Prints usage and exits."),

            [](Option::Base *opt, Option::Args const &) -> std::size_t
               {opt->getProgram()->putHelp(std::cout); throw EXIT_SUCCESS;}
         },

         optHelpADoc
         {
            &list, Option::Base::Info()
               .setName("help-adoc")
               .setDescS("Prints AsciiDoc usage and exits."),

            [](Option::Base *opt, Option::Args const &) -> std::size_t
               {opt->getProgram()->putAsciiDoc(std::cout); throw EXIT_SUCCESS;}
         },

         optHelpLong
         {
            &list, Option::Base::Info()
               .setName("help-long")
               .setDescS("Prints full usage and exits."),

            [](Option::Base *opt, Option::Args const &) -> std::size_t
               {opt->getProgram()->putHelpLong(std::cout); throw EXIT_SUCCESS;}
         },

         optOutput
         {
            &list, Option::Base::Info()
               .setName("output").setName('o')
               .setGroup("output")
               .setDescS("Sets output file or directory.")
         },

         optVersion
         {
            &list, Option::Base::Info()
               .setName("version")
               .setDescS("Prints version and exits."),

            [](Option::Base *opt, Option::Args const &) -> std::size_t
               {opt->getProgram()->putVersion(std::cout); throw EXIT_SUCCESS;}
         },

         optLibPath
         {
            nullptr, Option::Base::Info()
               .setName("lib-path")
               .setGroup("input")
               .setDescS("Sets the base path for library sources and headers.")
               .setDescL("Sets the base path for library sources and headers. "
                  "Sources will be found by appending src/<library name> to "
                  "this path. Headers will be found by appending "
                  "inc/<language> to this path.\n"
                  "\n"
                  "Default is <system path>/lib.")
         },

         optSysSource
         {
            nullptr, Option::Base::Info()
               .setName("sys-source")
               .setGroup("input")
               .setDescS("Adds source file from system directory."),
         }
      {
         list.processLoose = &args;
      }

      //
      // SystemSourceOption constructor
      //
      SystemSourceOption::SystemSourceOption(Option::Program *program,
         Info const &optInfo) :
         Option::CStrV{program, optInfo, 1}
      {
      }

      //
      // SystemSourceOption::v_process
      //
      std::size_t SystemSourceOption::v_process(Option::Args const &args)
      {
         if(!args.argC)
            Option::Exception::Error(args, "argument required");

         std::string name = GetSystemPath();

         Core::PathAppend(name, "lib");
         Core::PathAppend(name, "src");
         Core::PathAppend(name, args.argV[0]);

         char const *argV[1] = {name.data()};
         push(argV, 1, false, args.optFinal);

         return 1;
      }

      //
      // GetOptionArgs
      //
      Option::CStrV &GetOptionArgs()
      {
         return GetOptions().args;
      }

      //
      // GetOptionLibPath
      //
      std::string GetOptionLibPath()
      {
         if(auto path = GetOptions().optLibPath.data())
            return path;

         std::string path = GDCC::Core::GetSystemPath();
         GDCC::Core::PathAppend(path, "lib");
         return path;
      }

      //
      // GetOptionList
      //
      Option::Program &GetOptionList()
      {
         return GetOptions().list;
      }

      //
      // GetOptionOutput
      //
      char const *GetOptionOutput()
      {
         return GetOptions().optOutput.data();
      }

      //
      // GetOptions
      //
      OptionList &GetOptions()
      {
         static OptionList opts;
         return opts;
      }

      //
      // ProcessOptions
      //
      void ProcessOptions(OptionList &opts, int argc, char const *const *argv,
         bool needOutput)
      {
         if(!opts.list.version)
            opts.list.version = "v0.10.0";

         if(argc <= 1)
         {
            opts.list.putHelp(std::cout);
            throw EXIT_SUCCESS;
         }

         try
         {
            opts.list.process(Option::Args().setArgs(argv + 1, argc - 1).setOptKeepA());

            // Default output to last loose arg.
            if(needOutput && !opts.optOutput.processed && opts.args.size())
               opts.args.pop(&opts.optOutput);
         }
         catch(Option::Exception const &e)
         {
            opts.list.putHelp(std::cerr << e.what() << "\n\n");
            throw EXIT_FAILURE;
         }

         if(needOutput && (!opts.optOutput.processed || !opts.optOutput.data()))
         {
            std::cerr << "No output specified. Use -h for usage.\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

