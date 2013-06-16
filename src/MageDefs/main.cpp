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

#include "IStream.hpp"

#include "GDCC/Option.hpp"
#include "GDCC/Token.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace Option
{
   //
   // -t, --nts-type
   //
   OptionCStr NTSType{'t', "nts-type", "output",
      "Sets the type field. Default: GAMEDEFS", nullptr, "GAMEDEFS", false};

   //
   // -v, --nts-version
   //
   OptionCStr NTSVersion{'v', "nts-version", "output",
      "Sets the version field. Default: none", nullptr, "", false};
}


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static void ProcessFile(std::ostream &out, char const *inName);

//
// MakeDefs
//
static void MakeDefs()
{
   std::fstream out{Option::Output.data, std::ios_base::binary | std::ios_base::out};

   if(!out)
   {
      std::cerr << "couldn't open '" << Option::Output.data << "' for writing";
      throw EXIT_FAILURE;
   }

   // Write format.
   out << "MgC_NTS" << '\0';

   // Write type.
   if(Option::NTSType.data)
      out << Option::NTSType.data;
   out << '\0';

   // Write version.
   if(Option::NTSVersion.data)
      out << Option::NTSVersion.data;
   out << '\0';

   // Process inputs.
   for(auto arg = *Option::ArgV, end = arg + *Option::ArgC; arg != end; ++arg)
      ProcessFile(out, *arg);
}

//
// ProcessFile
//
static void ProcessFile(std::ostream &out, char const *inName)
{
   std::filebuf fbuf;

   if(!fbuf.open(inName, std::ios_base::in))
   {
      std::cerr << "couldn't open '" << Option::Output.data << "' for reading";
      throw EXIT_FAILURE;
   }

   MageDefs::IStream in{fbuf, GDCC::AddString(inName)};
   for(GDCC::Token tok; in >> tok;)
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
   Option::Option::Help_Usage = "[option]... [source]...";

   Option::Option::Help_DescS = "Output defaults to last loose argument.";

   try
   {
      GDCC::InitOptions(argc, argv, "gdcc-magedefs", "v0.0.0");

      MakeDefs();
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

