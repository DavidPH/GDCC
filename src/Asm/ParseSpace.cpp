//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Space parsing utilities.
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
   // ParseSpace
   //
   void ParseSpace(GDCC::TokenStream &in, GDCC::IR::Space &space)
   {
      while(!in.drop(GDCC::TOK_LnEnd)) switch(static_cast<GDCC::StringIndex>(
         ExpectToken(in, GDCC::TOK_Identi, "identifier").get().str))
      {
      case GDCC::STR_alloc:
         space.alloc = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="));
         break;

      case GDCC::STR_defin:
         space.defin = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="));
         break;

      case GDCC::STR_linka:
         space.linka = ParseLinkage(SkipToken(in, GDCC::TOK_Equal, "=").get());
         break;

      case GDCC::STR_value:
         space.value = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="));
         break;

      case GDCC::STR_words:
         space.words = ParseFastU(SkipToken(in, GDCC::TOK_Equal, "="));
         break;

      default:
         in.unget();
         std::cerr << "ERROR: " << in.peek().pos << ": bad Space argument: '"
            << in.peek().str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

