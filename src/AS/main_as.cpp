//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Program entry point.
//
//-----------------------------------------------------------------------------

#include "AS/Parse.hpp"

#include "Core/Option.hpp"

#include "IR/Program.hpp"

#include "LD/Linker.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// MakeAsm
//
static void MakeAsm()
{
   GDCC::IR::Program prog;

   // Process inputs.
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      GDCC::AS::ParseFile(arg, prog);

   for(auto const &arg : GDCC::Core::GetOptions().optSysSource)
      GDCC::AS::ParseFile(arg, prog);

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

   opts.list.name     = "gdcc-as";
   opts.list.nameFull = "GDCC Assembler";

   opts.list.usage = "[option]... [source]...";

   opts.list.descS =
      "Compiles assembly into IR data. Output defaults to last loose "
      "argument.";

   opts.optLibPath.insert(&opts.list);
   opts.optSysSource.insert(&opts.list);

   try
   {
      GDCC::Core::ProcessOptions(opts, argc, argv);
      MakeAsm();
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

