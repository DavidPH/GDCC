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
   std::fstream out{GDCC::Core::GetOptionOutput(),
      std::ios_base::binary | std::ios_base::out};

   if(!out)
   {
      std::cerr << "couldn't open '" << GDCC::Core::GetOptionOutput()
         << "' for writing\n";
      throw EXIT_FAILURE;
   }

   // Write format.
   out << "MgC_NTS" << '\0';

   // Write type.
   if(NTSType.str)
      out << NTSType.str;
   out << '\0';

   // Write version.
   if(NTSVersion.str)
      out << NTSVersion.str;
   out << '\0';

   // Process inputs.
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      ProcessFile(out, arg);
}

//
// ProcessFile
//
static void ProcessFile(std::ostream &out, char const *inName)
{
   std::filebuf fbuf;

   if(!fbuf.open(inName, std::ios_base::in))
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   GDCC::MageDefs::IStream in{fbuf, inName};
   for(GDCC::Core::Token tok; in >> tok;)
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

   list.usage = "[option]... [source]...";

   list.descS =
      "Converts text to MageCraft NTS. Output defaults to last loose argument.";

   try
   {
      Core::InitOptions(argc, argv, "gdcc-magedefs", "GDCC MageDefs");
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

