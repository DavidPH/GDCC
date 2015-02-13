//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS specifier-qualifier-list parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "AST/Type.hpp"

#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // IsSpecQual
      //
      bool IsSpecQual(ParserCtx const &ctx, CC::Scope &scope)
      {
         return
            IsTypeQual(ctx, scope) ||
            IsTypeSpec(ctx, scope);
      }

      //
      // ParseSpecQual
      //
      void ParseSpecQual(ParserCtx const &ctx, CC::Scope &scope,
         AST::Attribute &attr)
      {
         auto pos = ctx.in.peek().pos;

         AST::TypeQual declQual = AST::QualNone;
         CC::TypeSpec  declSpec;

         // Read specifier-qualifier tokens until there are no more.
         for(;;)
         {
            // type-specifier
            if(IsTypeSpec(ctx, scope))
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

