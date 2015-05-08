//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
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


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
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
      void Parser::parseAttrSpec(CC::Scope &, AST::Attribute &)
      {
      }
   }
}

// EOF

