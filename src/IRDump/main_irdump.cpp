//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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
   using namespace GDCC;

   IR::Program prog;

   // Process inputs.
   for(auto const &arg : Core::GetOptionArgs())
      ProcessFile(arg, prog);

   auto outName = Core::GetOptionOutput();
   if(!outName) outName = "-";

   auto buf = Core::FileOpenStream(outName, std::ios_base::out);
   if(!buf)
   {
      std::cerr << "couldn't open '" << outName << "' for writing\n";
      throw EXIT_FAILURE;
   }

   std::ostream out{buf.get()};
   IRDump::PutProgram(out, prog);
}

//
// ProcessFile
//
static void ProcessFile(char const *inName, GDCC::IR::Program &prog)
{
   using namespace GDCC;

   auto buf = Core::FileOpenStream(inName, std::ios_base::in | std::ios_base::binary);
   if(!buf)
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   std::istream in{buf.get()};
   IR::IArchive(in).getHeader() >> prog;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// main
//
int main(int argc, char *argv[])
{
   using namespace GDCC;

   auto &list = Core::GetOptionList();

   list.usage = "[option]... [source]...";

   list.descS =
      "Dumps information on IR data. Output defaults to stdout.";

   try
   {
      Core::InitOptions(argc, argv, "gdcc-irdump", "GDCC IR Dump", false);
      MakeIRDump();
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

