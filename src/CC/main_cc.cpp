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

#include "CC/Parse.hpp"
#include "CC/Scope/Global.hpp"

#include "AST/Statement.hpp"

#include "CPP/Macro.hpp"
#include "CPP/TStream.hpp"

#include "Core/Option.hpp"
#include "Core/Path.hpp"

#include "IR/OArchive.hpp"
#include "IR/Program.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static void ProcessFile(char const *inName, GDCC::IR::Program &prog);

//
// MakeC
//
static void MakeC()
{
   GDCC::IR::Program prog;

   // Process inputs.
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      ProcessFile(arg, prog);

   // Write IR data.
   std::fstream out{GDCC::Core::GetOptionOutput(),
      std::ios_base::binary | std::ios_base::out};

   if(!out)
   {
      std::cerr << "couldn't open '" << GDCC::Core::GetOptionOutput()
         << "' for writing\n";
      throw EXIT_FAILURE;
   }

   GDCC::IR::OArchive(out).putHeader() << prog;
}

//
// MakeGlobalLabel
//
static GDCC::Core::String MakeGlobalLabel(GDCC::Core::String file)
{
   using namespace GDCC;

   char buf[2 + (sizeof(std::size_t) * CHAR_BIT + 3) / 4 + 1];

   buf[0] = '_';
   buf[1] = '$';

   std::sprintf(buf + 2, "%*zX", static_cast<int>(sizeof(buf) - 3), file.getHash());

   return {buf, sizeof(buf) - 1};
}

//
// ProcessFile
//
static void ProcessFile(char const *inName, GDCC::IR::Program &prog)
{
   using namespace GDCC;

   std::filebuf fbuf;

   if(!fbuf.open(inName, std::ios_base::in))
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   Core::String     file{inName};
   CPP::MacroMap    macr{CPP::Macro::Stringize(file)};
   CPP::PragmaLangC prag{};
   Core::String     path{Core::PathDirname(file)};
   CPP::TStream     tstr{fbuf, macr, prag, file, path};
   CC::ParserCtx    in  {tstr, prag, prog};
   CC::Scope_Global ctx {MakeGlobalLabel(file)};

   // Read declarations.
   while(in.in.peek().tok != Core::TOK_EOF)
      CC::GetDecl(in, ctx);

   ctx.allocAuto();

   // Generate IR data.
   ctx.genIR(prog);
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
      "Compiles C source into IR data. Output defaults to last loose argument.";

   try
   {
      Core::InitOptions(argc, argv, "gdcc-cc", "GDCC C Compiler");
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

