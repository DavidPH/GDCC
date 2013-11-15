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
#include "Parse.hpp"
#include "TStream.hpp"

#include "GDCC/Option.hpp"
#include "GDCC/Token.hpp"

#include "GDCC/IR/Function.hpp"
#include "GDCC/IR/Import.hpp"
#include "GDCC/IR/OArchive.hpp"
#include "GDCC/IR/Object.hpp"
#include "GDCC/IR/StrEnt.hpp"

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
      std::cerr << "couldn't open '" << Option::Output.data << "' for writing\n";
      throw EXIT_FAILURE;
   }

   GDCC::IR::OArchive(out).putHeader().putTables();
}

//
// ParseTLK
//
static void ParseTLK(GDCC::TokenStream &in)
{
   GDCC::IR::AddrSpace as;

   while(in.peek().tok == GDCC::TOK_LnEnd) in.get();

   if(in.peek().tok == GDCC::TOK_EOF) return;

   Asm::ExpectToken(in, GDCC::TOK_Identi, "identifier");
   switch(static_cast<GDCC::StringIndex>(in.get().str))
   {
   case GDCC::STR_Function:
      Asm::ExpectToken(in, GDCC::TOK_String, "string");
      Asm::ParseFunction(in, GDCC::IR::Function::Get(in.get().str));
      break;

   case GDCC::STR_Import:
      Asm::ExpectToken(in, GDCC::TOK_String, "string");
      Asm::ParseImport(in, GDCC::IR::Import::Get(in.get().str));
      break;

   case GDCC::STR_Macro:
      Asm::ExpectToken(in, GDCC::TOK_Identi, "identifier");
      {
         GDCC::String    name = in.get().str;
         GDCC::IR::Block list;

         while(in.drop(GDCC::TOK_LnEnd)) {}
         Asm::SkipToken(in, GDCC::TOK_BraceO, "{");
         Asm::ParseBlock(in, list, GDCC::TOK_BraceC);

         Asm::Macro::Add(name, std::move(list));
      }
      break;

   case GDCC::STR_Object:
      as.base = Asm::ParseAddrBase(in.get());
      as.name = Asm::ExpectToken(in, GDCC::TOK_String, "string").get().str;
      Asm::ExpectToken(in, GDCC::TOK_String, "string");

      if(auto space = GDCC::IR::Space::Find(as))
         Asm::ParseObject(in, space->get(in.get().str));
      else
      {
         std::cerr << "ERROR: " << in.peek().pos << ": expected address space\n";
         throw EXIT_FAILURE;
      }

      break;

   case GDCC::STR_Space:
      as.base = Asm::ParseAddrBase(in.get());
      as.name = Asm::ExpectToken(in, GDCC::TOK_String, "string").get().str;

      Asm::ParseSpace(in, GDCC::IR::Space::Get(as));
      break;

   case GDCC::STR_StrEnt:
      Asm::ExpectToken(in, GDCC::TOK_String, "string");
      Asm::ParseStrEnt(in, GDCC::IR::StrEnt::Get(in.get().str));
      break;

   default:
      in.unget();
      std::cerr << "ERROR: " << in.peek().pos << ": bad top-level keyword: '"
         << in.peek().str << "'\n";
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
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   Asm::TStream in{fbuf, GDCC::AddString(inName)};
   while(in.peek().tok != GDCC::TOK_EOF) ParseTLK(in);
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

