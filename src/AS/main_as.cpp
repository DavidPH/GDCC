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

#include "AS/Macro.hpp"
#include "AS/Parse.hpp"
#include "AS/TStream.hpp"

#include "Core/File.hpp"
#include "Core/Option.hpp"
#include "Core/Token.hpp"

#include "IR/OArchive.hpp"
#include "IR/Program.hpp"

#include <fstream>
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
   using namespace GDCC;

   IR::Program prog;

   // Process inputs.
   for(auto const &arg : Core::GetOptionArgs())
      ProcessFile(arg, prog);

   auto outName = Core::GetOptionOutput();

   // Write IR data.
   auto buf = Core::FileOpenStream(outName, std::ios_base::out | std::ios_base::binary);
   if(!buf)
   {
      std::cerr << "couldn't open '" << outName << "' for writing\n";
      throw EXIT_FAILURE;
   }

   std::ostream out{buf.get()};
   IR::OArchive(out).putHeader() << prog;
}

//
// ProcessFile
//
static void ProcessFile(char const *inName, GDCC::IR::Program &prog)
{
   using namespace GDCC;

   std::filebuf fbuf;

   auto buf = Core::FileOpenStream(inName, std::ios_base::in);
   if(!buf)
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   AS::TStream   in    {*buf, inName};
   AS::MacroMap  macros{};
   AS::ParserCtx ctx   {in, macros, prog};

   while(!ctx.in.peek(Core::TOK_EOF))
      AS::ParseDeclaration(ctx);
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
      "Compiles assembly into IR data. Output defaults to last loose argument.";

   try
   {
      Core::InitOptions(argc, argv, "gdcc-as", "GDCC Assembler");
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

