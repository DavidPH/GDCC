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

#include "GDCC/Option.hpp"

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
   std::ifstream in{inName};

   if(!in)
   {
      std::cerr << "couldn't open '" << Option::Output.data << "' for reading";
      throw EXIT_FAILURE;
   }

   in.unsetf(std::ios_base::skipws);

   char c, p = '\0';

   while(in >> c)
   {
      if(c == '#')
         while(in >> c && c != '\n') {}
      else if(c == '\\')
         out << (in >> p, p);
      else if(!std::isspace(c))
         out << (p = c);
      else if(p)
         out << (p = '\0');
   }

   if(p) out << (p = '\0');
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

