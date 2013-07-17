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
#include "Parse.hpp"

#include "GDCC/Option.hpp"
#include "GDCC/Token.hpp"

#include "GDCC/IR/Function.hpp"
#include "GDCC/IR/OArchive.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static void ProcessFile(char const *inName);

//
// MakeAsm
//
static void MakeAsm()
{
   // Process inputs.
   for(auto arg = *Option::ArgV, end = arg + *Option::ArgC; arg != end; ++arg)
      ProcessFile(*arg);

   // Write IR data.
   std::fstream out{Option::Output.data, std::ios_base::binary | std::ios_base::out};

   if(!out)
   {
      std::cerr << "couldn't open '" << Option::Output.data << "' for writing";
      throw EXIT_FAILURE;
   }

   GDCC::IR::OArchive(out).writeHeader().writeTables();
}

//
// ParseTLK
//
static bool ParseTLK(Asm::IStream &in)
{
   GDCC::Token tok;
   if(!(in >> tok)) return false;

   if(tok.tok == GDCC::TOK_EOL) return true;

   switch(static_cast<GDCC::StringIndex>(tok.str))
   {
   case GDCC::STR_Function:
      in >> tok;
      if(tok.tok != GDCC::TOK_Identifier && tok.tok != GDCC::TOK_String)
      {
         std::cerr << "ERROR: " << tok.pos << ": expected identifier or string\n";
         throw EXIT_FAILURE;
      }

      Asm::ParseFunction(in, GDCC::IR::Function::Get(tok.str));
      return true;

   default:
      std::cerr << "ERROR: " << tok.pos << ": bad top-level keyword: '"
         << tok.str << "'\n";
      throw EXIT_FAILURE;
   }
}

//
// ProcessFile
//
static void ProcessFile(char const *inName)
{
   std::filebuf fbuf;

   if(!fbuf.open(inName, std::ios_base::in))
   {
      std::cerr << "couldn't open '" << Option::Output.data << "' for reading";
      throw EXIT_FAILURE;
   }

   Asm::IStream in{fbuf, GDCC::AddString(inName)};
   while(ParseTLK(in)) {}
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
      GDCC::InitOptions(argc, argv, "gdcc-as");
      MakeAsm();
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

