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

#include "MageDefs/IStream.hpp"

#include "Core/File.hpp"
#include "Core/Option.hpp"
#include "Core/Token.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

//
// -t, --nts-type
//
static GDCC::Option::CStr NTSType
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("nts-type").setName('t')
      .setGroup("output")
      .setDescS("Sets the type field. Default: GAMEDEFS"),

   "GAMEDEFS", false
};

//
// -v, --nts-version
//
static GDCC::Option::CStr NTSVersion
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("nts-version").setName('v')
      .setGroup("output")
      .setDescS("Sets the version field. Default: none"),

   "", false
};


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static void ProcessFile(std::ostream &out, char const *inName);

//
// MakeDefs
//
static void MakeDefs()
{
   using namespace GDCC;

   auto outName = Core::GetOptionOutput();

   auto buf = Core::FileOpenStream(outName, std::ios_base::out | std::ios_base::binary);
   if(!buf)
   {
      std::cerr << "couldn't open '" << outName << "' for writing\n";
      throw EXIT_FAILURE;
   }

   std::ostream out{buf.get()};

   // Write format.
   out << "MgC_NTS" << '\0';

   // Write type.
   if(NTSType.data())
      out << NTSType.data();
   out << '\0';

   // Write version.
   if(NTSVersion.data())
      out << NTSVersion.data();
   out << '\0';

   // Process inputs.
   for(auto const &arg : Core::GetOptionArgs())
      ProcessFile(out, arg);
}

//
// ProcessFile
//
static void ProcessFile(std::ostream &out, char const *inName)
{
   using namespace GDCC;

   auto buf = Core::FileOpenStream(inName, std::ios_base::in);
   if(!buf)
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   MageDefs::IStream in{*buf, inName};
   for(Core::Token tok; in >> tok;)
      out << tok.str << '\0';
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

   list.name     = "gdcc-magedefs";
   list.nameFull = "GDCC MageDefs";

   list.usage = "[option]... [source]...";

   list.descS =
      "Converts text to MageCraft NTS. Output defaults to last loose argument.";

   try
   {
      Core::ProcessOptions(Core::GetOptions(), argc, argv);
      MakeDefs();
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

