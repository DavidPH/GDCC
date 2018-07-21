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

#include "CPP/Macro.hpp"
#include "CPP/TStream.hpp"

#include "Core/File.hpp"
#include "Core/Option.hpp"
#include "Core/Path.hpp"

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
   auto outName = GDCC::Core::GetOptionOutput();

   // Open output file.
   auto buf = GDCC::Core::FileOpenStream(outName, std::ios_base::out);

   // Process inputs.
   std::ostream out{buf.get()};
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      ProcessFile(out, arg);
}

//
// ProcessFile
//
static void ProcessFile(std::ostream &out, char const *inName)
{
   auto buf = GDCC::Core::FileOpenStream(inName, std::ios_base::in);

   GDCC::Core::String      file {inName};
   GDCC::CPP::IncludeLang  langs{"C"};
   GDCC::CPP::MacroMap     macr {GDCC::CPP::Macro::Stringize(file)};
   GDCC::Core::String      path {GDCC::Core::PathDirname(file)};
   GDCC::CPP::PragmaData   pragd{};
   GDCC::CPP::PragmaParser pragp{pragd};
   GDCC::CPP::PPStream     in   {*buf, langs, macr, pragd, pragp, file, path};

   for(GDCC::Core::Token tok; in >> tok;) switch(tok.tok)
   {
   case GDCC::Core::TOK_ChrU16: out << 'u'; goto case_Charac;
   case GDCC::Core::TOK_ChrU32: out << 'U'; goto case_Charac;
   case GDCC::Core::TOK_ChrWid: out << 'L'; goto case_Charac;
   case GDCC::Core::TOK_Charac: case_Charac:
      out << '\''; PutStringEscape(out, tok.str); out << '\''; break;

   case GDCC::Core::TOK_StrIdx: out << 's';  goto case_String;
   case GDCC::Core::TOK_StrU08: out << "u8"; goto case_String;
   case GDCC::Core::TOK_StrU16: out << 'u';  goto case_String;
   case GDCC::Core::TOK_StrU32: out << 'U';  goto case_String;
   case GDCC::Core::TOK_StrWid: out << 'L';  goto case_String;
   case GDCC::Core::TOK_String: case_String:
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
// Extern Functions                                                           |
//

//
// main
//
int main(int argc, char *argv[])
{
   auto &list = GDCC::Core::GetOptionList();

   list.name     = "gdcc-cpp";
   list.nameFull = "GDCC C Preprocessor";

   list.usage = "[option]... [source]...";

   list.descS =
      "Performs C preprocessing. Output defaults to last loose argument.";

   try
   {
      GDCC::Core::ProcessOptions(GDCC::Core::GetOptions(), argc, argv);
      MakeCPP();
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

