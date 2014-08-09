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

#include "Bytecode/Platform.hpp"

#include "Bytecode/MgC/Info.hpp"
#include "Bytecode/ZDACS/Info.hpp"

#include "Core/Option.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"
#include "IR/Program.hpp"

#include "Option/Bool.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static bool OutputIR = false;


//----------------------------------------------------------------------------|
// Options                                                                    |
//

//
// -c, --ir-output
//
static GDCC::Option::Bool OutputIROpt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("ir-output").setName('c')
      .setGroup("output")
      .setDescS("Generate a new IR file instead of bytecode."),

   &OutputIR
};


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
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      ProcessFile(arg, prog);

   // Write IR data.
   if(OutputIR)
   {
      std::fstream out{GDCC::Core::GetOptionOutput(),
         std::ios_base::binary | std::ios_base::out};

      if(!out)
      {
         std::cerr << "couldn't open '" << GDCC::Core::GetOptionOutput()
            << "' for writing";
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

   std::fstream out{GDCC::Core::GetOptionOutput(),
      std::ios_base::binary | std::ios_base::out};

   if(!out)
   {
      std::cerr << "couldn't open '" << GDCC::Core::GetOptionOutput()
         << "' for writing";
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
   using namespace GDCC;

   auto &list = Core::GetOptionList();

   list.usage = "[option]... [source]...";

   list.descS =
      "Converts IR data into bytecode. Output defaults to last loose argument.";

   try
   {
      Core::InitOptions(argc, argv, "gdcc-ld", "GDCC Linker");
      MakeLinker();
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

