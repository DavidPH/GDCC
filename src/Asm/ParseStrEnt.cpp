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

#include "IStream.hpp"

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
   void ParseStrEnt(IStream &in, GDCC::IR::StrEnt &str)
   {
      for(GDCC::Token tok; in >> tok;) switch(static_cast<GDCC::StringIndex>(tok.str))
      {
      case GDCC::STR_alias:
         str.alias = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_alloc:
         str.alloc = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_defin:
         str.defin = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_valueInt:
         str.valueInt = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_valueStr:
         str.valueStr = (SkipEqual(in) >> tok, tok).str;
         break;

      default:
         if(tok.tok == GDCC::TOK_EOL) return;

         std::cerr << "ERROR: " << tok.pos << ": bad StrEnt argument: '"
            << tok.str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

