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

#include "CPP/Macro.hpp"
#include "CPP/TStream.hpp"

#include "Core/Option.hpp"
#include "Core/Path.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static void ProcessFile(std::ostream &out, char const *inName);
static void PutStringEscape(std::ostream &out, GDCC::Core::String str);

//
// MakeCPP
//
static void MakeCPP()
{
   // Open output file.
   std::fstream out{GDCC::Option::Output.data, std::ios_base::binary | std::ios_base::out};

   if(!out)
   {
      std::cerr << "couldn't open '" << GDCC::Option::Output.data << "' for writing\n";
      throw EXIT_FAILURE;
   }

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

   auto inStr = GDCC::Core::AddString(inName);

   GDCC::CPP::Macro::Reset();
   GDCC::CPP::Macro::LinePush(GDCC::CPP::Macro::Stringize(inStr));

   GDCC::CPP::PragmaLangC pragma;

   GDCC::CPP::PPStream in{fbuf, pragma, inStr, GDCC::Core::PathDirname(inStr)};

   for(GDCC::Core::Token tok; in >> tok;) switch(tok.tok)
   {
   case GDCC::Core::TOK_Charac: out << '\''; PutStringEscape(out, tok.str); out << '\''; break;
   case GDCC::Core::TOK_String: out << '"'; PutStringEscape(out, tok.str); out << '"'; break;
   default: out << tok.str; break;
   }

   out << std::endl;
}

//
// PutStringEscape
//
static void PutStringEscape(std::ostream &out, GDCC::Core::String str)
{
   for(char c : str) switch(c)
   {
   case '\a': out << "\\a";  break;
   case '\b': out << "\\b";  break;
   case '\f': out << "\\f";  break;
   case '\n': out << "\\n";  break;
   case '\r': out << "\\r";  break;
   case '\t': out << "\\t";  break;
   case '\v': out << "\\v";  break;
   case '\'': out << "\\\'"; break;
   case '\"': out << "\\\""; break;
   case '\\': out << "\\\\"; break;
   case '\?': out << "\\\?"; break;
   default:   out << c;      break;
   }
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
      GDCC::Core::InitOptions(argc, argv, "gdcc-cpp");
      MakeCPP();
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

