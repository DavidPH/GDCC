//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// StrEnt parsing utilities.
//
//-----------------------------------------------------------------------------

#include "Parse.hpp"

#include "GDCC/TokenStream.hpp"

#include "GDCC/IR/StrEnt.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   //
   // ParseStrEnt
   //
   void ParseStrEnt(GDCC::TokenStream &in, GDCC::IR::StrEnt &str)
   {
      while(!in.drop(GDCC::TOK_LnEnd)) switch(static_cast<GDCC::StringIndex>(
         ExpectToken(in, GDCC::TOK_Identi, "identifier").get().str))
      {
      case GDCC::STR_alias:
         str.alias = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="));
         break;

      case GDCC::STR_alloc:
         str.alloc = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="));
         break;

      case GDCC::STR_defin:
         str.defin = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="));
         break;

      case GDCC::STR_valueInt:
         str.valueInt = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="));
         break;

      case GDCC::STR_valueStr:
         SkipToken(in, GDCC::TOK_Equal, "=");
         str.valueStr = ExpectToken(in, GDCC::TOK_String, "string").get().str;
         break;

      default:
         in.unget();
         std::cerr << "ERROR: " << in.peek().pos << ": bad StrEnt argument: '"
            << in.peek().str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

