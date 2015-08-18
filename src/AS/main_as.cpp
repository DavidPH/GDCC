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

#include "AS/Macro.hpp"
#include "AS/Parse.hpp"
#include "AS/TStream.hpp"

#include "Core/File.hpp"
#include "Core/Option.hpp"
#include "Core/Token.hpp"

#include "IR/Program.hpp"

#include "LD/Linker.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static void ProcessFile(char const *inName, GDCC::IR::Program &prog);

//
// MakeAsm
//
static void MakeAsm()
{
   GDCC::IR::Program prog;

   // Process inputs.
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      ProcessFile(arg, prog);

   for(auto const &arg : GDCC::Core::GetOptions().optSysSource)
      ProcessFile(arg, prog);

   // Write output.
   GDCC::LD::Link(prog, GDCC::Core::GetOptionOutput());
}

//
// ProcessFile
//
static void ProcessFile(char const *inName, GDCC::IR::Program &prog)
{
   auto buf = GDCC::Core::FileOpenStream(inName, std::ios_base::in);
   if(!buf)
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   GDCC::AS::TStream   in    {*buf, inName};
   GDCC::AS::MacroMap  macros{};
   GDCC::AS::ParserCtx ctx   {in, macros, prog};

   while(!ctx.in.peek(GDCC::Core::TOK_EOF))
      GDCC::AS::ParseDeclaration(ctx);
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

   opts.list.name     = "gdcc-as";
   opts.list.nameFull = "GDCC Assembler";

   opts.list.usage = "[option]... [source]...";

   opts.list.descS =
      "Compiles assembly into IR data. Output defaults to last loose "
      "argument.";

   opts.optSysSource.insert(&opts.list);

   GDCC::LD::OutputIR = true;

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

