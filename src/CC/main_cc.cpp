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
   GDCC::CPP::IncludeDTBuf::AddIncludeLang("C");

   GDCC::IR::Program prog;

   // Process inputs.
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      ProcessFile(arg, prog);

   for(auto const &arg : GDCC::Core::GetOptions().optSysSource)
      ProcessFile(arg, prog);

   auto outName = GDCC::Core::GetOptionOutput();

   // Write IR data.
   auto buf = GDCC::Core::FileOpenStream(outName,
      std::ios_base::out | std::ios_base::binary);
   if(!buf)
   {
      std::cerr << "couldn't open '" << outName << "' for writing\n";
      throw EXIT_FAILURE;
   }

   std::ostream out{buf.get()};
   GDCC::IR::OArchive(out).putHeader() << prog;
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
   auto buf = GDCC::Core::FileOpenBlock(inName);
   if(!buf)
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   GDCC::Core::String      file {inName};
   GDCC::CPP::MacroMap     macr {GDCC::CPP::Macro::Stringize(file)};
   GDCC::Core::String      path {GDCC::Core::PathDirname(file)};
   GDCC::CPP::PragmaData   pragd{};
   GDCC::CPP::PragmaParser pragp{pragd};
   GDCC::Core::StringBuf   sbuf {buf->data(), buf->size()};
   GDCC::CPP::TStream      tstr {sbuf, macr, pragd, pragp, file, path};
   GDCC::CC::Parser        ctx  {tstr, pragd, prog};
   GDCC::CC::Scope_Global  scope{MakeGlobalLabel(buf->getHash())};

   // Read declarations.
   while(ctx.in.peek().tok != GDCC::Core::TOK_EOF)
      ctx.getDecl(scope);

   // Add ACS libraries.
   for(auto const &lib : pragd.stateLibrary)
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
   auto &opts = GDCC::Core::GetOptions();

   opts.list.name     = "gdcc-cc";
   opts.list.nameFull = "GDCC C Compiler";

   opts.list.usage = "[option]... [source]...";

   opts.list.descS =
      "Compiles C source into IR data. Output defaults to last loose "
      "argument.";

   opts.optSysSource.insert(&opts.list);

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

