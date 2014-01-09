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

#include "AS/Macro.hpp"
#include "AS/Parse.hpp"
#include "AS/TStream.hpp"

#include "Core/Option.hpp"
#include "Core/Token.hpp"

#include "IR/OArchive.hpp"
#include "IR/Program.hpp"

#include <fstream>
#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static void ProcessFile(char const *inName, GDCC::IR::Program &prog);

//
// MakeAsm
//
static void MakeAsm()
{
   GDCC::IR::Program prog;

   // Process inputs.
   for(auto const &arg : GDCC::Core::GetOptionArgs())
      ProcessFile(arg, prog);

   // Write IR data.
   std::fstream out{GDCC::Core::GetOptionOutput(),
      std::ios_base::binary | std::ios_base::out};

   if(!out)
   {
      std::cerr << "couldn't open '" << GDCC::Core::GetOptionOutput()
         << "' for writing\n";
      throw EXIT_FAILURE;
   }

   GDCC::IR::OArchive(out).putHeader() << prog;
}

//
// ParseTLK
//
static void ParseTLK(GDCC::Core::TokenStream &in, GDCC::IR::Program &prog)
{
   GDCC::IR::AddrSpace as;

   while(in.peek().tok == GDCC::Core::TOK_LnEnd) in.get();

   if(in.peek().tok == GDCC::Core::TOK_EOF) return;

   GDCC::AS::ExpectToken(in, GDCC::Core::TOK_Identi, "identifier");
   switch(static_cast<GDCC::Core::StringIndex>(in.get().str))
   {
   case GDCC::Core::STR_Function:
      GDCC::AS::ExpectToken(in, GDCC::Core::TOK_String, "string");
      GDCC::AS::ParseFunction(in, prog, prog.getFunction(in.get().str));
      break;

   case GDCC::Core::STR_Import:
      GDCC::AS::ExpectToken(in, GDCC::Core::TOK_String, "string");
      GDCC::AS::ParseImport(in, prog, prog.getImport(in.get().str));
      break;

   case GDCC::Core::STR_Macro:
      GDCC::AS::ExpectToken(in, GDCC::Core::TOK_Identi, "identifier");
      {
         GDCC::Core::String name = in.get().str;
         GDCC::IR::Block    list;

         while(in.drop(GDCC::Core::TOK_LnEnd)) {}
         GDCC::AS::SkipToken(in, GDCC::Core::TOK_BraceO, "{");
         GDCC::AS::ParseBlock(in, prog, list, GDCC::Core::TOK_BraceC);

         GDCC::AS::Macro::Add(name, std::move(list));
      }
      break;

   case GDCC::Core::STR_Object:
      as.base = GDCC::AS::ParseAddrBase(in.get());
      as.name = GDCC::AS::ExpectToken(in, GDCC::Core::TOK_String, "string").get().str;
      GDCC::AS::ExpectToken(in, GDCC::Core::TOK_String, "string");
      {
         // HACK: space needs to be an Object property now.
         auto &obj = prog.getObject(in.get().str);
         obj.space = as;
         GDCC::AS::ParseObject(in, prog, obj);
      }
      break;

   case GDCC::Core::STR_Space:
      as.base = GDCC::AS::ParseAddrBase(in.get());
      as.name = GDCC::AS::ExpectToken(in, GDCC::Core::TOK_String, "string").get().str;

      GDCC::AS::ParseSpace(in, prog, prog.getSpace(as));
      break;

   case GDCC::Core::STR_StrEnt:
      GDCC::AS::ExpectToken(in, GDCC::Core::TOK_String, "string");
      GDCC::AS::ParseStrEnt(in, prog, prog.getStrEnt(in.get().str));
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
static void ProcessFile(char const *inName, GDCC::IR::Program &prog)
{
   std::filebuf fbuf;

   if(!fbuf.open(inName, std::ios_base::in))
   {
      std::cerr << "couldn't open '" << inName << "' for reading\n";
      throw EXIT_FAILURE;
   }

   GDCC::AS::TStream in{fbuf, GDCC::Core::AddString(inName)};
   while(in.peek().tok != GDCC::Core::TOK_EOF) ParseTLK(in, prog);
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
      GDCC::Core::InitOptions(argc, argv, "gdcc-as");
      MakeAsm();
   }
   catch(int e)
   {
      return e;
   }
}

// EOF

