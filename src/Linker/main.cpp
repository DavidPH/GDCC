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

#include "Bytecode/Platform.hpp"

#include "Bytecode/MgC/Info.hpp"
#include "Bytecode/ZDACS/Info.hpp"

#include "GDCC/Option.hpp"

#include "GDCC/IR/IArchive.hpp"
#include "GDCC/IR/OArchive.hpp"
#include "GDCC/IR/Program.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace Option
{
   //
   // -c, --ir-output
   //
   OptionData<bool> OutputIR{'c', "ir-output", "output",
      "Generate a new IR file instead of bytecode.", nullptr, false};
}


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static void ProcessFile(char const *inName, GDCC::IR::Program &prog);

//
// MakeLinked
//
static void MakeLinker()
{
   GDCC::IR::Program prog;

   // Process inputs.
   for(auto arg = *Option::ArgV, end = arg + *Option::ArgC; arg != end; ++arg)
      ProcessFile(*arg, prog);

   // Write IR data.
   if(Option::OutputIR.data)
   {
      std::fstream out{Option::Output.data, std::ios_base::binary | std::ios_base::out};

      if(!out)
      {
         std::cerr << "couldn't open '" << Option::Output.data << "' for writing";
         throw EXIT_FAILURE;
      }

      GDCC::IR::OArchive(out).putHeader() << prog;

      return;
   }

   std::unique_ptr<Bytecode::Info> info;

   switch(Bytecode::TargetCur)
   {
   case Bytecode::Target::None:
      std::cerr << "must specify target\n";
      throw EXIT_FAILURE;

   case Bytecode::Target::MageCraft:
      info.reset(new Bytecode::MgC::Info);
      break;

   case Bytecode::Target::ZDoom:
      info.reset(new Bytecode::ZDACS::Info);
      break;
   }

   info->pre(prog);
   info->tr(prog);
   info->gen(prog);

   std::fstream out{Option::Output.data, std::ios_base::binary | std::ios_base::out};

   if(!out)
   {
      std::cerr << "couldn't open '" << Option::Output.data << "' for writing";
      throw EXIT_FAILURE;
   }

   info->put(prog, out);
}

//
// ProcessFile
//
static void ProcessFile(char const *inName, GDCC::IR::Program &prog)
{
   std::ifstream in{inName, std::ios_base::binary | std::ios_base::in};

   if(!in)
   {
      std::cerr << "couldn't open '" << inName << "' for reading";
      throw EXIT_FAILURE;
   }

   GDCC::IR::IArchive(in).getHeader() >> prog;
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
      GDCC::InitOptions(argc, argv, "gdcc-ld");
      MakeLinker();
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

