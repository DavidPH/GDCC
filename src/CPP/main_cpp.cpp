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

#include "Core/File.hpp"
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
   using namespace GDCC;

   auto outName = Core::GetOptionOutput();

   // Open output file.
   auto buf = Core::FileOpenStream(outName, std::ios_base::out);
   if(!buf)
   {
      std::cerr << "couldn't open '" << GDCC::Core::GetOptionOutput()
         << "' for writing\n";
      throw EXIT_FAILURE;
   }

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
   using namespace GDCC;

   auto buf = Core::FileOpenStream(inName, std::ios_base::in);
   if(!buf)
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   Core::String     file{inName};
   CPP::MacroMap    macr{CPP::Macro::Stringize(file)};
   CPP::PragmaLangC prag{};
   CPP::PPStream    in  {*buf, macr, prag, file, Core::PathDirname(file)};

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
   using namespace GDCC;

   auto &list = Core::GetOptionList();

   list.name     = "gdcc-cpp";
   list.nameFull = "GDCC C Preprocessor";

   list.usage = "[option]... [source]...";

   list.descS =
      "Performs C preprocessing. Output defaults to last loose argument.";

   try
   {
      Core::ProcessOptions(Core::GetOptions(), argc, argv);
      MakeCPP();
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

