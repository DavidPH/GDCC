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

   // Write output.
   auto buf = GDCC::Core::FileOpenStream(GDCC::Core::GetOptionOutput(),
      std::ios_base::out | std::ios_base::binary);
   std::ostream out{buf.get()};
   wad.writeData(out);
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

   opts.list.descS =
      "Packs and unpacks Wad archives. Output defaults to last loose argument.";

   try
   {
      GDCC::Core::ProcessOptions(opts, argc, argv);
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

