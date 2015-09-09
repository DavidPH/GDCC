//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
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

#include "Core/Option.hpp"

#include "IR/Program.hpp"

#include "LD/Linker.hpp"

#include "Platform/Platform.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// MakeACS
//
static void MakeACS()
{
   GDCC::CPP::IncludeDTBuf::AddIncludeLang("ACS");

   GDCC::IR::Program prog;

   // Process inputs.
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      GDCC::ACC::ParseFile(arg, prog);

   for(auto const &arg : GDCC::Core::GetOptions().optSysSource)
      GDCC::ACC::ParseFile(arg, prog);

   // Write output.
   GDCC::LD::Link(prog, GDCC::Core::GetOptionOutput());
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

   opts.optSysSource.insert(&opts.list);

   // Default target to ZDoom, like acc.
   GDCC::Platform::TargetCur = GDCC::Platform::Target::ZDoom;

   try
   {
      GDCC::Core::ProcessOptions(opts, argc, argv);
      MakeACS();
   }
   catch(std::exception const &e)
   {
      std::cerr << e.what() << std::endl;
      return EXIT_FAILURE;
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

