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
   std::fstream out{GDCC::Core::GetOptionOutput(),
      std::ios_base::binary | std::ios_base::out};

   if(!out)
   {
      std::cerr << "couldn't open '" << GDCC::Core::GetOptionOutput()
         << "' for writing\n";
      throw EXIT_FAILURE;
   }

   // Process inputs.
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      ProcessFile(out, arg);
}

//
// ProcessFile
//
static void ProcessFile(std::ostream &out, char const *inName)
{
   using namespace GDCC;

   std::filebuf fbuf;

   if(!fbuf.open(inName, std::ios_base::in))
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   auto inStr = Core::AddString(inName);

   CPP::Macro::Reset();
   CPP::Macro::LinePush(CPP::Macro::Stringize(inStr));

   CPP::PragmaLangC pragma;

   CPP::PPStream in{fbuf, pragma, inStr, Core::PathDirname(inStr)};

   for(Core::Token tok; in >> tok;) switch(tok.tok)
   {
   case Core::TOK_ChrU16: out << 'u'; goto case_Charac;
   case Core::TOK_ChrU32: out << 'U'; goto case_Charac;
   case Core::TOK_ChrWid: out << 'L'; goto case_Charac;
   case Core::TOK_Charac: case_Charac:
      out << '\''; PutStringEscape(out, tok.str); out << '\''; break;

   case Core::TOK_StrIdx: out << 's';  goto case_String;
   case Core::TOK_StrU08: out << "u8"; goto case_String;
   case Core::TOK_StrU16: out << 'u';  goto case_String;
   case Core::TOK_StrU32: out << 'U';  goto case_String;
   case Core::TOK_StrWid: out << 'L';  goto case_String;
   case Core::TOK_String: case_String:
      out << '"'; PutStringEscape(out, tok.str); out << '"'; break;

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
   GDCC::Core::GetOptionList().usage = "[option]... [source]...";

   GDCC::Core::GetOptionList().descS = "Output defaults to last loose argument.";

   try
   {
      GDCC::Core::InitOptions(argc, argv, "gdcc-cpp");
      MakeCPP();
   }
   catch(std::exception const &e)
   {
      std::cerr << e.what() << std::endl;
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

