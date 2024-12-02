//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2024 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Program entry point.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "CPP/IncludeDTBuf.hpp"

#include "Core/File.hpp"
#include "Core/Option.hpp"

#include "IR/Program.hpp"

#include "LD/Linker.hpp"

#include "Target/Info.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

//
// --error-file
//
GDCC::Option::CStr ErrorFile
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("error-file")
      .setGroup("output")
      .setDescS("Write errors to file.")
};


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// MakeACS
//
static void MakeACS()
{
   GDCC::IR::Program prog;

   // Process inputs.
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      GDCC::ACC::ParseFile(arg, prog);

   for(auto const &arg : GDCC::Core::GetOptions().optSysSource)
      GDCC::ACC::ParseFile(arg, prog);

   // Write output.
   GDCC::LD::Link(prog, GDCC::Core::GetOptionOutput());
}

//
// MakeACSAlt
//
// Handles single-argument invocation.
//
static void MakeACSAlt()
{
   GDCC::IR::Program prog;

   // Determine file extension.
   std::string file{GDCC::Core::GetOptionOutput()};
   auto        dot = file.rfind('.');

   // If filename has no extension, use .acs.
   if(dot == std::string::npos)
   {
      dot   = file.size();
      file += ".acs";
   }

   // Process input.
   GDCC::ACC::ParseFile(file.data(), prog);

   // Replace extension with .o for output.
   file.resize(dot + 2);
   file[dot + 1] = 'o';

   // Write output.
   GDCC::LD::Link(prog, file.data());
}

//
// WriteError
//
static void WriteError(char const *filename, char const *what)
{
   if(!filename) return;

   try
   {
      auto buf = GDCC::Core::FileOpenStream(filename, std::ios_base::out);
      std::ostream out{buf.get()};
      out << "ERROR: " << what << std::endl;
   }
   catch(std::exception const &e)
   {
      std::cerr << "ERROR: " << e.what() << std::endl;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// main
//
int main(int argc, char *argv[])
{
   auto &opts = GDCC::Core::GetOptions();

   opts.list.name     = "gdcc-acc";
   opts.list.nameFull = "GDCC ACS Compiler";

   opts.list.usage = "[option]... [source]...";

   opts.list.descS =
      "Compiles ACS source into IR data. Output defaults to last loose "
      "argument.";

   opts.optLibPath.insert(&opts.list);
   opts.optSysSource.insert(&opts.list);

   // Default target to ZDoom, like acc.
   GDCC::Target::EngineCur = GDCC::Target::Engine::ZDoom;
   GDCC::Target::FormatCur = GDCC::Target::Format::ACSE;

   try
   {
      GDCC::Core::ProcessOptions(opts, argc, argv);

      if(!opts.args.size() && !opts.optSysSource.size())
         MakeACSAlt();
      else
         MakeACS();
   }
   catch(std::exception const &e)
   {
      std::cerr << "ERROR: " << e.what() << std::endl;

      // Also write error to file, if requested.
      WriteError(ErrorFile.data(), e.what());

      return EXIT_FAILURE;
   }
   catch(int e)
   {
      // Indicate failure to error file.
      if(e == EXIT_FAILURE)
         WriteError(ErrorFile.data(), "unknown error occurred");

      return e;
   }
}

// EOF

