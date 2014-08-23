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

#include "Bytecode/MgC/Info.hpp"
#include "Bytecode/ZDACS/Info.hpp"

#include "Core/File.hpp"
#include "Core/Option.hpp"

#include "IR/IArchive.hpp"
#include "IR/OArchive.hpp"
#include "IR/Program.hpp"

#include "Option/Bool.hpp"

#include "Platform/Platform.hpp"

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
   using namespace GDCC;

   IR::Program prog;

   // Process inputs.
   for(auto const &arg : Core::GetOptionArgs())
      ProcessFile(arg, prog);

   auto outName = Core::GetOptionOutput();

   // Write IR data.
   if(OutputIR)
   {
      auto buf = Core::FileOpenStream(outName, std::ios_base::binary | std::ios_base::out);
      if(!buf)
      {
         std::cerr << "couldn't open '" << outName << "' for writing\n";
         throw EXIT_FAILURE;
      }

      std::ostream out{buf.get()};
      IR::OArchive(out).putHeader() << prog;

      return;
   }

   // Select bytecode.
   std::unique_ptr<Bytecode::Info> info;

   switch(Platform::TargetCur)
   {
   case Platform::Target::None:
      std::cerr << "must specify target\n";
      throw EXIT_FAILURE;

   case Platform::Target::MageCraft:
      info.reset(new Bytecode::MgC::Info);
      break;

   case Platform::Target::ZDoom:
      info.reset(new Bytecode::ZDACS::Info);
      break;
   }

   // Process IR data.
   info->pre(prog);
   info->tr(prog);
   info->gen(prog);

   // Output bytecode.
   auto buf = Core::FileOpenStream(outName, std::ios_base::out | std::ios_base::binary);
   if(!buf)
   {
      std::cerr << "couldn't open '" << outName << "' for writing\n";
      throw EXIT_FAILURE;
   }

   std::ostream out{buf.get()};
   info->put(prog, out);
}

//
// ProcessFile
//
static void ProcessFile(char const *inName, GDCC::IR::Program &prog)
{
   using namespace GDCC;

   auto buf = Core::FileOpenStream(inName, std::ios_base::in | std::ios_base::binary);
   if(!buf)
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   std::istream in{buf.get()};
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

