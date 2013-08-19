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

#include "IStream.hpp"

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
   void ParseSpace(IStream &in, GDCC::IR::Space &space)
   {
      for(GDCC::Token tok; in >> tok;) switch(static_cast<GDCC::StringIndex>(tok.str))
      {
      case GDCC::STR_alloc:
         space.alloc = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_defin:
         space.defin = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_linka:
         space.linka = ParseLinkage((SkipEqual(in) >> tok, tok));
         break;

      case GDCC::STR_value:
         space.value = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_words:
         space.words = ParseFastU(SkipEqual(in));
         break;

      default:
         if(tok.tok == GDCC::TOK_LnEnd) return;

         std::cerr << "ERROR: " << tok.pos << ": bad Space argument: '"
            << tok.str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

