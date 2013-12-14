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

#include "Core/Option.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"
#include "IR/Program.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace Option
   {
      //
      // -c, --ir-output
      //
      OptionData<bool> OutputIR{'c', "ir-output", "output",
         "Generate a new IR file instead of bytecode.", nullptr, false};
   }
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
   for(auto arg = *GDCC::Option::ArgV, end = arg + *GDCC::Option::ArgC; arg != end; ++arg)
      ProcessFile(*arg, prog);

   // Write IR data.
   if(GDCC::Option::OutputIR.data)
   {
      std::fstream out{GDCC::Option::Output.data, std::ios_base::binary | std::ios_base::out};

      if(!out)
      {
         std::cerr << "couldn't open '" << GDCC::Option::Output.data << "' for writing";
         throw EXIT_FAILURE;
      }

      GDCC::IR::OArchive(out).putHeader() << prog;

      return;
   }

   std::unique_ptr<GDCC::Bytecode::Info> info;

   switch(GDCC::Bytecode::TargetCur)
   {
   case GDCC::Bytecode::Target::None:
      std::cerr << "must specify target\n";
      throw EXIT_FAILURE;

   case GDCC::Bytecode::Target::MageCraft:
      info.reset(new GDCC::Bytecode::MgC::Info);
      break;

   case GDCC::Bytecode::Target::ZDoom:
      info.reset(new GDCC::Bytecode::ZDACS::Info);
      break;
   }

   info->pre(prog);
   info->tr(prog);
   info->gen(prog);

   std::fstream out{GDCC::Option::Output.data, std::ios_base::binary | std::ios_base::out};

   if(!out)
   {
      std::cerr << "couldn't open '" << GDCC::Option::Output.data << "' for writing";
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
   GDCC::Option::Option::Help_Usage = "[option]... [source]...";

   GDCC::Option::Option::Help_DescS = "Output defaults to last loose argument.";

   try
   {
      GDCC::Core::InitOptions(argc, argv, "gdcc-ld");
      MakeLinker();
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

