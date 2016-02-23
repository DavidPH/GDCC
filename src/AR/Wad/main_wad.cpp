//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Program entry point.
//
//-----------------------------------------------------------------------------

#include "AR/Wad/LumpInfo.hpp"
#include "AR/Wad/Wad.hpp"

#include "Core/File.hpp"
#include "Core/Option.hpp"

#include <cstring>
#include <iostream>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

//
// --list
//
static GDCC::Option::CStr OptionList
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("list")
      .setGroup("output")
      .setDescS("Outputs list of lumps to a file."),
};


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static void ProcessFile(char const *data, GDCC::AR::Wad::Wad &wad);

//
// MakeWad
//
static void MakeWad()
{
   GDCC::AR::Wad::Wad wad;

   // Process inputs.
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      ProcessFile(arg, wad);

   // Write lump list.
   if(auto listFile = OptionList.data())
   {
      auto buf = GDCC::Core::FileOpenStream(listFile, std::ios_base::out);
      std::ostream out{buf.get()};
      wad.writeList(out);
   }

   // Write output.
   if(auto outFile = GDCC::Core::GetOptionOutput())
   {
      auto buf = GDCC::Core::FileOpenStream(outFile,
         std::ios_base::out | std::ios_base::binary);
      std::ostream out{buf.get()};
      wad.writeData(out);
   }
}

//
// ProcessFile
//
static void ProcessFile(char const *data, GDCC::AR::Wad::Wad &wad)
{
   wad.addLump(GDCC::AR::Wad::GetLumpInfo(data));
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// main
//
int main(int argc, char *argv[])
{
   auto &opts = GDCC::Core::GetOptions();

   opts.list.name     = "gdcc-ar-wad";
   opts.list.nameFull = "GDCC Wad Archiver";

   opts.list.usage = "[option]... [source]...";

   opts.list.descS = "Packs and unpacks Wad archives.";

   try
   {
      GDCC::Core::ProcessOptions(opts, argc, argv, false);
      MakeWad();
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

