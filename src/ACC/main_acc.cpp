//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Program entry point.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"
#include "ACC/Pragma.hpp"
#include "ACC/Scope.hpp"
#include "ACC/TStream.hpp"

#include "AST/Statement.hpp"

#include "CPP/Macro.hpp"

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
// MakeACS
//
static void MakeACS()
{
   GDCC::CPP::IncludeDTBuf::AddIncludeLang("ACS");

   GDCC::IR::Program prog;

   // Process inputs.
   for(auto const &arg : GDCC::Core::GetOptionArgs())
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
   GDCC::ACC::PragmaData   pragd{};
   GDCC::ACC::PragmaParser pragp{pragd};
   GDCC::ACC::Scope_Global scope{MakeGlobalLabel(buf->getHash())};

   // Do an import pass to pick up any function declarations.
   {
      GDCC::Core::StringBuf   sbuf{buf->data(), buf->size()};
      GDCC::ACC::ImportStream tstr{sbuf, macr, pragd, pragp, file};
      GDCC::ACC::ParserCtx    ctx {tstr, pragd, prog, true};

      pragd.push();

      // Read declarations.
      while(ctx.in.peek().tok != GDCC::Core::TOK_EOF)
         GDCC::ACC::GetDecl(ctx, scope);

      pragd.drop();

      // Ignore any library pragmas from this pass.
      pragd.stateLibrary.clear();
   }

   GDCC::Core::StringBuf   sbuf {buf->data(), buf->size()};
   GDCC::ACC::TStream      tstr {sbuf, macr, pragd, pragp, file, path, scope, prog};
   GDCC::ACC::ParserCtx    ctx  {tstr, pragd, prog};

   // Read declarations.
   while(ctx.in.peek().tok != GDCC::Core::TOK_EOF)
      GDCC::ACC::GetDecl(ctx, scope);

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
   auto &list = GDCC::Core::GetOptionList();

   list.name     = "gdcc-acc";
   list.nameFull = "GDCC ACS Compiler";

   list.usage = "[option]... [source]...";

   list.descS =
      "Compiles ACS source into IR data. Output defaults to last loose "
      "argument.";

   try
   {
      GDCC::Core::ProcessOptions(GDCC::Core::GetOptions(), argc, argv);
      MakeACS();
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

