//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2019 David Hill
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

#include "Option/Bool.hpp"

#include <cstring>
#include <iostream>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

//
// --extract
//
static GDCC::Option::Bool Extract
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("extract")
      .setGroup("output")
      .setDescS("Extracts to a directory instead of writing to archive."),

   false
};

//
// --iwad
//
static GDCC::Option::Bool IWad
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("iwad")
      .setGroup("output")
      .setDescS("Writes an IWAD instead of a PWAD."),

   false
};

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
   wad.iwad = IWad;

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
      if(Extract)
      {
         std::string path{outFile};
         wad.writeDirs(path);
      }
      else
      {
         auto buf = GDCC::Core::FileOpenStream(outFile,
            std::ios_base::out | std::ios_base::binary);
         std::ostream out{buf.get()};
         wad.writeData(out);
      }
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

   opts.list.descS =
     "Packs and unpacks Wad archives. Sources are TYPE:LUMP=PATH. Types are "
     "data, file, or wad.";

   opts.list.descL =
      "Packs and unpacks Wad archives.\n"
      "Sources are in the form TYPE:LUMP=PATH (as in file:ACSLIB=acslib.bin). "
      "The available types are: data, file, and wad. If the type is omitted, "
      "it is assumed to be file.\n"
      "\n"
      "data - Adds a lump with PATH as the contents. If LUMP is omitted, PATH "
      "is used as the name of an empty lump.\n"
      "\n"
      "file (file) - Adds a lump using the contents of the named file. If "
      "LUMP is omitted, the lump name defaults to PATH with any extension "
      "removed.\n"
      "file (directory) - Adds a set of lumps from a directory. If LUMP is "
      "omitted, the lumps are added directly to the root wad. Otherwise, they "
      "are added to a sub-wad.\n"
      "\n"
      "wad (file) - Adds a wad's lumps. If LUMP is omitted, the lumps are "
      "added directly to the root wad. Otherwise, they are added to a sub-wad.\n"
      "wad (directory) - Adds a set of lumps from a directory. "
      "Sub-directories are added as *_START or map sub-wads if applicable. "
      "LUMP usage is the same as for a file.";

   try
   {
      GDCC::Core::ProcessOptions(opts, argc, argv, false);
      MakeWad();
   }
   catch(std::exception const &e)
   {
      std::cerr << "ERROR: " << e.what() << std::endl;
      return EXIT_FAILURE;
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

