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

#include "Macro.hpp"
#include "TStream.hpp"

#include "GDCC/Option.hpp"
#include "GDCC/Path.hpp"

#include "GDCC/IR/OArchive.hpp"
#include "GDCC/IR/Program.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static void ProcessFile(char const *inName);

//
// MakeC
//
static void MakeC()
{
   GDCC::IR::Program prog;

   // Process inputs.
   for(auto arg = *Option::ArgV, end = arg + *Option::ArgC; arg != end; ++arg)
      ProcessFile(*arg);

   // Write IR data.
   std::fstream out{Option::Output.data, std::ios_base::binary | std::ios_base::out};

   if(!out)
   {
      std::cerr << "couldn't open '" << Option::Output.data << "' for writing\n";
      throw EXIT_FAILURE;
   }

   GDCC::IR::OArchive(out).putHeader() << prog;
}

//
// ProcessFile
//
static void ProcessFile(char const *inName)
{
   std::filebuf fbuf;

   if(!fbuf.open(inName, std::ios_base::in))
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   auto inStr = GDCC::AddString(inName);

   C::Macro::Reset();
   C::Macro::LinePush(C::Macro::Stringize(inStr));

   C::PragmaLangC pragma;

   C::TStream in{fbuf, pragma, inStr, GDCC::PathDirname(inStr)};

   for(GDCC::Token tok; in >> tok;)
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
   Option::Option::Help_Usage = "[option]... [source]...";

   Option::Option::Help_DescS = "Output defaults to last loose argument.";

   try
   {
      GDCC::InitOptions(argc, argv, "gdcc-cc");
      MakeC();
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

