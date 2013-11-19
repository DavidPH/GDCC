//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Object parsing utilities.
//
//-----------------------------------------------------------------------------

#include "Parse.hpp"

#include "GDCC/TokenStream.hpp"

#include "GDCC/IR/Exp.hpp"
#include "GDCC/IR/Object.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   //
   // ParseObject
   //
   void ParseObject(GDCC::TokenStream &in, GDCC::IR::Program &prog,
      GDCC::IR::Object &obj)
   {
      while(!in.drop(GDCC::TOK_LnEnd)) switch(static_cast<GDCC::StringIndex>(
         ExpectToken(in, GDCC::TOK_Identi, "identifier").get().str))
      {
      case GDCC::STR_alias:
         obj.alias = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      case GDCC::STR_alloc:
         obj.alloc = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      case GDCC::STR_defin:
         obj.defin = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      case GDCC::STR_initi:
         obj.initi = ParseExp(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      case GDCC::STR_linka:
         obj.linka = ParseLinkage(SkipToken(in, GDCC::TOK_Equal, "=").get());
         break;

      case GDCC::STR_value:
         obj.value = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      case GDCC::STR_words:
         obj.words = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="), prog);
         break;

      default:
         in.unget();
         std::cerr << "ERROR: " << in.peek().pos << ": bad Object argument: '"
            << in.peek().str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

