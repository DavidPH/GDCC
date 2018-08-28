//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Pragma.hpp"

#include "Core/Parse.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Parser constructor
   //
   Parser::Parser(Core::TokenStream &in_, PragmaData &prag_,
      IR::Program &prog_, bool importing_) :
      CC::Parser{in_, prag_, prog_},
      prag     (prag_),
      importing{importing_}
   {
   }

   //
   // Parser constructor
   //
   Parser::Parser(Parser const &ctx, Core::TokenStream &in_) :
      CC::Parser{ctx, in_},
      prag     (ctx.prag),
      importing{ctx.importing}
   {
   }

   //
   // Parser::isAttrSpec
   //
   bool Parser::isAttrSpec(CC::Scope &)
   {
      return false;
   }

   //
   // Parser::parseAttrSpec
   //
   void Parser::parseAttrSpec(CC::Scope &, SR::Attribute &)
   {
   }

   //
   // ParseEscape
   //
   bool ParseEscape(std::ostream &out, std::istream &in, char escape)
   {
      switch(escape)
      {
      case 'c': out.put('\x1C'); return true;

      case '\'':
      case '\"':
      case '\\':

      case 'a':
      case 'b':
      case 'f':
      case 'n':
      case 'r':
      case 't':
      case 'v':
      case 'x':

      case '0': case '1': case '2': case '3':
      case '4': case '5': case '6': case '7':
         return Core::ParseEscapeC(out, in, escape);

      default:
         return false;
      }
   }
}

// EOF

