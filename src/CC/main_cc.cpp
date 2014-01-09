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
// ProcessFile
//
static void ProcessFile(char const *inName, GDCC::IR::Program &)
{
   std::filebuf fbuf;

   if(!fbuf.open(inName, std::ios_base::in))
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   auto inStr = GDCC::Core::AddString(inName);

   GDCC::CPP::Macro::Reset();
   GDCC::CPP::Macro::LinePush(GDCC::CPP::Macro::Stringize(inStr));

   GDCC::CPP::PragmaLangC pragma;

   GDCC::CPP::TStream in{fbuf, pragma, inStr, GDCC::Core::PathDirname(inStr)};

   for(GDCC::Core::Token tok; in >> tok;)
      std::cout << tok.tok << '(' << tok.str << ") ";

   std::cout << std::endl;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// main
//
int main(int argc, char *argv[])
{
   GDCC::Core::GetOptionList().usage = "[option]... [source]...";

   GDCC::Core::GetOptionList().descS = "Output defaults to last loose argument.";

   try
   {
      GDCC::Core::InitOptions(argc, argv, "gdcc-cc");
      MakeC();
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

