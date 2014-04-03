//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C specifier-qualifier-list parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "AST/Type.hpp"

#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // IsSpecQual
      //
      bool IsSpecQual(ParserCtx const &in, Scope &ctx)
      {
         return IsAttrSpec(in, ctx) || IsTypeQual(in, ctx) || IsTypeSpec(in, ctx);
      }

      //
      // ParseSpecQual
      //
      void ParseSpecQual(ParserCtx const &in, Scope &ctx, AST::Attribute &attr)
      {
         auto pos = in.in.peek().pos;

         AST::TypeQual declQual = AST::QualNone;
         TypeSpec      declSpec;

         // Read specifier-qualifier tokens until there are no more.
         for(;;)
         {
            // attribute-specifier
            if(IsAttrSpec(in, ctx))
               ParseAttrSpec(in, ctx, attr);

            // type-specifier
            else if(IsTypeSpec(in, ctx))
               ParseTypeSpec(in, ctx, attr, declSpec);

            // type-qualifier
            else if(IsTypeQual(in, ctx))
                ParseTypeQual(in, ctx, declQual);

            else
               break;
         }

         // Finalize the type specifiers.
         declSpec.finish(attr, declQual, pos);
      }
   }
}

// EOF

