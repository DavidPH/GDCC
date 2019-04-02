//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS type qualifier parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Parser::isTypeQual
   //
   bool Parser::isTypeQual(CC::Scope &)
   {
      auto const &tok = in.peek();
      if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
         return false;

      switch(tok.str)
      {
         // Standard C qualifiers.
      case Core::STR_const: return true;

      default:
         return false;
      }
   }

   //
   // Parser::parseTypeQual
   //
   void Parser::parseTypeQual(CC::Scope &, SR::TypeQual &qual)
   {
      auto const &tok = expectIdenti(true);

      switch(tok.str)
      {
         // Standard C qualifiers.
      case Core::STR_const: qual.aCons = true; break;

      default:
         Core::ErrorExpect("type-qualifier", tok);
      }
   }
}

// EOF

