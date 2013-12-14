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

#include "MageDefs/IStream.hpp"

#include "Core/Option.hpp"
#include "Core/Token.hpp"

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
   std::fstream out{GDCC::Option::Output.data, std::ios_base::binary | std::ios_base::out};

   if(!out)
   {
      std::cerr << "couldn't open '" << GDCC::Option::Output.data << "' for writing\n";
      throw EXIT_FAILURE;
   }

   // Write format.
   out << "MgC_NTS" << '\0';

   // Write type.
   if(GDCC::Option::NTSType.data)
      out << GDCC::Option::NTSType.data;
   out << '\0';

   // Write version.
   if(GDCC::Option::NTSVersion.data)
      out << GDCC::Option::NTSVersion.data;
   out << '\0';

   // Process inputs.
   for(auto arg = *GDCC::Option::ArgV, end = arg + *GDCC::Option::ArgC; arg != end; ++arg)
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
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   GDCC::MageDefs::IStream in{fbuf, GDCC::Core::AddString(inName)};
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
   GDCC::Option::Option::Help_Usage = "[option]... [source]...";

   GDCC::Option::Option::Help_DescS = "Output defaults to last loose argument.";

   try
   {
      GDCC::Core::InitOptions(argc, argv, "gdcc-magedefs");
      MakeDefs();
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

