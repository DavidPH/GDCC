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

#include "Core/File.hpp"
#include "Core/Option.hpp"
#include "Core/Path.hpp"
#include "Core/StringBuf.hpp"

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
   using namespace GDCC;

   CPP::IncludeDTBuf::AddIncludeLang("C");

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
// MakeGlobalLabel
//
static GDCC::Core::String MakeGlobalLabel(std::size_t hash)
{
   char buf[2 + (sizeof(std::size_t) * CHAR_BIT + 3) / 4 + 1];

   buf[0] = '_';
   buf[1] = '$';

   std::sprintf(buf + 2, "%*zX", static_cast<int>(sizeof(buf) - 3), hash);

   return {buf, sizeof(buf) - 1};
}

//
// ProcessFile
//
static void ProcessFile(char const *inName, GDCC::IR::Program &prog)
{
   using namespace GDCC;

   auto buf = Core::FileOpenBlock(inName);
   if(!buf)
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   Core::String     file {inName};
   CPP::MacroMap    macr {CPP::Macro::Stringize(file)};
   CPP::PragmaLangC prag {};
   Core::String     path {Core::PathDirname(file)};
   Core::StringBuf  sbuf {buf->data(), buf->size()};
   CPP::TStream     tstr {sbuf, macr, prag, file, path};
   CC::ParserCtx    ctx  {tstr, prag, prog};
   CC::Scope_Global scope{MakeGlobalLabel(buf->getHash())};

   // Read declarations.
   while(ctx.in.peek().tok != Core::TOK_EOF)
      CC::GetDecl(ctx, scope);

   // Add ACS libraries.
   for(auto const &lib : prag.pragmaACS_library)
      prog.getImport(lib);

   scope.allocAuto();

   // Generate IR data.
   scope.genIR(prog);
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

   list.name     = "gdcc-cc";
   list.nameFull = "GDCC C Compiler";

   list.usage = "[option]... [source]...";

   list.descS =
      "Compiles C source into IR data. Output defaults to last loose argument.";

   try
   {
      Core::ProcessOptions(Core::GetOptions(), argc, argv);
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

