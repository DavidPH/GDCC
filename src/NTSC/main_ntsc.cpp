//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Program entry point.
//
//-----------------------------------------------------------------------------

#include "NTSC/IStream.hpp"

#include "Core/Exception.hpp"
#include "Core/File.hpp"
#include "Core/Option.hpp"
#include "Core/Token.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

//
// -f, --nts-format
//
static GDCC::Option::CStr NTSFormat
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("nts-format").setName('f')
      .setGroup("output")
      .setDescS("Sets the format field. Default: DGE_NTS"),

   "DGE_NTS", false
};

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
   auto outName = GDCC::Core::GetOptionOutput();

   auto buf = GDCC::Core::FileOpenStream(outName, std::ios_base::out | std::ios_base::binary);
   std::ostream out{buf.get()};

   // Write format.
   if(NTSFormat.data())
      out << NTSFormat.data();
   out << '\0';

   // Write type.
   if(NTSType.data())
      out << NTSType.data();
   out << '\0';

   // Write version.
   if(NTSVersion.data())
      out << NTSVersion.data();
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
   auto buf = GDCC::Core::FileOpenStream(inName, std::ios_base::in);
   GDCC::NTSC::IStream in{*buf, inName};
   for(GDCC::Core::Token tok; in >> tok;)
      out << tok.str << '\0';
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

   opts.list.name     = "gdcc-ntsc";
   opts.list.nameFull = "GDCC NTS Compiler";

   opts.list.usage = "[option]... [source]...";

   opts.list.descS =
      "Converts text to Null-Terminated String format. Output defaults to last "
      "loose argument.";

   try
   {
      GDCC::Core::ProcessOptions(opts, argc, argv);
      MakeDefs();
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

