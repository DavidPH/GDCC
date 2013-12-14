//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Program entry point.
//
//-----------------------------------------------------------------------------

#include "IRDump/IRDump.hpp"

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
   for(auto arg = *GDCC::Option::ArgV, end = arg + *GDCC::Option::ArgC; arg != end; ++arg)
      ProcessFile(*arg, prog);

   if(GDCC::Option::Output.data)
   {
      std::fstream out{GDCC::Option::Output.data};

      if(!out)
      {
         std::cerr << "couldn't open '" << GDCC::Option::Output.data << "' for writing";
         throw EXIT_FAILURE;
      }

      GDCC::IRDump::IRDump_Program(out, prog);
   }
   else
      GDCC::IRDump::IRDump_Program(std::cout, prog);
}

//
// ProcessFile
//
static void ProcessFile(char const *inName, GDCC::IR::Program &prog)
{
   std::ifstream in{inName, std::ios_base::binary | std::ios_base::in};

   if(!in)
   {
      std::cerr << "couldn't open '" << inName << "' for reading";
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
   GDCC::Option::Option::Help_Usage = "[option]... [source]...";

   GDCC::Option::Option::Help_DescS = "Output defaults to stdout.";

   try
   {
      GDCC::Core::InitOptions(argc, argv, "gdcc-irdump", false);
      MakeIRDump();
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

