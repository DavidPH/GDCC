//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS type qualifier parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "AST/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // IsTypeQual
      //
      bool IsTypeQual(ParserCtx const &ctx, CC::Scope &)
      {
         auto const &tok = ctx.in.peek();
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
      // ParseTypeQual
      //
      void ParseTypeQual(ParserCtx const &ctx, CC::Scope &, AST::TypeQual &qual)
      {
         auto const &tok = ctx.in.get();
         if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
            throw Core::ParseExceptExpect(tok, "identifier", false);

         switch(tok.str)
         {
            // Standard C qualifiers.
         case Core::STR_const: qual.aCons = true; break;

         default:
            throw Core::ParseExceptExpect(tok, "type-qualifier", false);
         }
      }
   }
}

// EOF

