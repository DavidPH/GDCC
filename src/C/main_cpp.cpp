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

#include "Macro.hpp"
#include "TStream.hpp"

#include "GDCC/Option.hpp"
#include "GDCC/Path.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static void ProcessFile(std::ostream &out, char const *inName);
static void PutStringEscape(std::ostream &out, GDCC::String str);

//
// MakeCPP
//
static void MakeCPP()
{
   // Open output file.
   std::fstream out{Option::Output.data, std::ios_base::binary | std::ios_base::out};

   if(!out)
   {
      std::cerr << "couldn't open '" << Option::Output.data << "' for writing\n";
      throw EXIT_FAILURE;
   }

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
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   auto inStr = GDCC::AddString(inName);

   C::Macro::Reset();
   C::Macro::LinePush(C::Macro::Stringize(inStr));

   C::PPStream in{fbuf, inStr, GDCC::PathDirname(inStr)};

   for(GDCC::Token tok; in >> tok;) switch(tok.tok)
   {
   case GDCC::TOK_Charac: out << '\''; PutStringEscape(out, tok.str); out << '\''; break;
   case GDCC::TOK_String: out << '"'; PutStringEscape(out, tok.str); out << '"'; break;
   default: out << tok.str; break;
   }

   out << std::endl;
}

//
// PutStringEscape
//
static void PutStringEscape(std::ostream &out, GDCC::String str)
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
   Option::Option::Help_Usage = "[option]... [source]...";

   Option::Option::Help_DescS = "Output defaults to last loose argument.";

   try
   {
      GDCC::InitOptions(argc, argv, "gdcc-cpp");
      MakeCPP();
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

