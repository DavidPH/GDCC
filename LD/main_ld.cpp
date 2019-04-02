//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Program entry point.
//
//-----------------------------------------------------------------------------

#include "LD/Linker.hpp"

#include "Core/File.hpp"
#include "Core/Option.hpp"

#include "IR/IArchive.hpp"
#include "IR/Program.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static void ProcessFile(char const *inName, GDCC::IR::Program &prog);

//
// MakeLinker
//
static void MakeLinker()
{
   GDCC::IR::Program prog;

   // Process inputs.
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      ProcessFile(arg, prog);

   // Write output.
   GDCC::LD::Link(prog, GDCC::Core::GetOptionOutput());
}

//
// ProcessFile
//
static void ProcessFile(char const *inName, GDCC::IR::Program &prog)
{
   auto buf = GDCC::Core::FileOpenStream(inName, std::ios_base::in | std::ios_base::binary);
   std::istream in{buf.get()};
   GDCC::IR::IArchive arc{in};
   arc >> prog;
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

   opts.list.name     = "gdcc-ld";
   opts.list.nameFull = "GDCC Linker";

   opts.list.usage = "[option]... [source]...";

   opts.list.descS =
      "Converts IR data into bytecode. Output defaults to last loose argument.";

   try
   {
      GDCC::Core::ProcessOptions(opts, argc, argv);
      MakeLinker();
   }
   catch(std::exception const &e)
   {
      std::cerr << "ERROR: " << e.what() << std::endl;
      return EXIT_FAILURE;
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

