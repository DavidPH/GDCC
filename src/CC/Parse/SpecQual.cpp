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
      bool IsSpecQual(ParserCtx const &ctx, Scope &scope)
      {
         return
            IsAttrSpec(ctx, scope) ||
            IsTypeQual(ctx, scope) ||
            IsTypeSpec(ctx, scope);
      }

      //
      // ParseSpecQual
      //
      void ParseSpecQual(ParserCtx const &ctx, Scope &scope, AST::Attribute &attr)
      {
         auto pos = ctx.in.peek().pos;

         AST::TypeQual declQual = AST::QualNone;
         TypeSpec      declSpec;

         // Read specifier-qualifier tokens until there are no more.
         for(;;)
         {
            // attribute-specifier
            if(IsAttrSpec(ctx, scope))
               ParseAttrSpec(ctx, scope, attr);

            // type-specifier
            else if(IsTypeSpec(ctx, scope))
               ParseTypeSpec(ctx, scope, attr, declSpec);

            // type-qualifier
            else if(IsTypeQual(ctx, scope))
                ParseTypeQual(ctx, scope, declQual);

            else
               break;
         }

         // Finalize the type specifiers.
         declSpec.finish(attr, declQual, pos);
      }
   }
}

// EOF

