//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Program entry point.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CPP/IncludeDTBuf.hpp"

#include "Core/Option.hpp"

#include "IR/Program.hpp"

#include "LD/Linker.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// MakeC
//
static void MakeC()
{
   GDCC::IR::Program prog;

   // Process inputs.
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      GDCC::CC::ParseFile(arg, prog);

   for(auto const &arg : GDCC::Core::GetOptions().optSysSource)
      GDCC::CC::ParseFile(arg, prog);

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

   opts.list.name     = "gdcc-cc";
   opts.list.nameFull = "GDCC C Compiler";

   opts.list.usage = "[option]... [source]...";

   opts.list.descS =
      "Compiles C source into IR data. Output defaults to last loose "
      "argument.";

   opts.optSysSource.insert(&opts.list);

   GDCC::LD::OutputIR = true;

   try
   {
      GDCC::Core::ProcessOptions(opts, argc, argv);
      MakeC();
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

