//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2017 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Program entry point.
//
//-----------------------------------------------------------------------------

#include "IRDump/Put.hpp"

#include "Core/File.hpp"
#include "Core/Option.hpp"

#include "IR/IArchive.hpp"
#include "IR/Program.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static void ProcessFile(char const *inName, GDCC::IR::Program &prog);

//
// MakeIRDump
//
static void MakeIRDump()
{
   GDCC::IR::Program prog;

   // Process inputs.
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      ProcessFile(arg, prog);

   auto outName = GDCC::Core::GetOptionOutput();
   if(!outName) outName = "-";

   auto buf = GDCC::Core::FileOpenStream(outName, std::ios_base::out);
   std::ostream out{buf.get()};
   GDCC::IRDump::PutProgram(out, prog);
}

//
// ProcessFile
//
static void ProcessFile(char const *inName, GDCC::IR::Program &prog)
{
   auto buf = GDCC::Core::FileOpenStream(inName, std::ios_base::in | std::ios_base::binary);
   std::istream in{buf.get()};
   GDCC::IR::IArchive(in).getHeader() >> prog;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// main
//
int main(int argc, char *argv[])
{
   auto &opts = GDCC::Core::GetOptions();

   opts.list.name     = "gdcc-irdump";
   opts.list.nameFull = "GDCC IR Dump";

   opts.list.usage = "[option]... [source]...";

   opts.list.descS =
      "Dumps information on IR data. Output defaults to stdout.";

   try
   {
      GDCC::Core::ProcessOptions(opts, argc, argv, false);
      MakeIRDump();
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

