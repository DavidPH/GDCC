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

   if(GDCC::Core::GetOptionOutput())
   {
      std::ofstream out{GDCC::Core::GetOptionOutput()};

      if(!out)
      {
         std::cerr << "couldn't open '" << GDCC::Core::GetOptionOutput()
            << "' for writing\n";
         throw EXIT_FAILURE;
      }

      GDCC::IRDump::PutProgram(out, prog);
   }
   else
      GDCC::IRDump::PutProgram(std::cout, prog);
}

//
// ProcessFile
//
static void ProcessFile(char const *inName, GDCC::IR::Program &prog)
{
   std::ifstream in{inName, std::ios_base::binary | std::ios_base::in};

   if(!in)
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

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

