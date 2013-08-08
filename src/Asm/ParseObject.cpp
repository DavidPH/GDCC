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

#include "IStream.hpp"

#include "GDCC/IR/Exp.hpp"
#include "GDCC/IR/Object.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// SkipEqual
//
static Asm::IStream &SkipEqual(Asm::IStream &in)
{
   GDCC::Token tok;
   if((in >> tok, tok).tok != GDCC::TOK_Equal)
   {
      std::cerr << "ERROR: " << tok.pos << ": expected =\n";
      throw EXIT_FAILURE;
   }

   return in;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   //
   // ParseObject
   //
   void ParseObject(IStream &in, GDCC::IR::Object &obj)
   {
      for(GDCC::Token tok; in >> tok;) switch(static_cast<GDCC::StringIndex>(tok.str))
      {
      case GDCC::STR_alloc:
         obj.alloc = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_defin:
         obj.defin = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_initi:
         obj.initi = ParseExp(SkipEqual(in));
         break;

      case GDCC::STR_linka:
         obj.linka = ParseLinkage((SkipEqual(in) >> tok, tok));
         break;

      case GDCC::STR_value:
         obj.value = ParseFastU(SkipEqual(in));
         break;

      case GDCC::STR_words:
         obj.words = ParseFastU(SkipEqual(in));
         break;

      default:
         if(tok.tok == GDCC::TOK_EOL) return;

         std::cerr << "ERROR: " << tok.pos << ": bad Object argument: '"
            << tok.str << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

