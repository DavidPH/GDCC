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

#include "AST/Attribute.hpp"
#include "AST/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // GetType
      //
      // type-name:
      //    specifier-qualifier-list abstract-declarator(opt)
      //
      AST::Type::CRef GetType(ParserCtx const &ctx, CC::Scope &scope)
      {
         AST::Attribute attr;

         ParseSpecQual(ctx, scope, attr);
         if(IsDeclarator(ctx, scope))
            ParseDeclarator(ctx, scope, attr);

         return static_cast<AST::Type::CRef>(attr.type);
      }

      //
      // IsType
      //
      bool IsType(ParserCtx const &ctx, CC::Scope &scope)
      {
         return IsSpecQual(ctx, scope);
      }
   }
}

// EOF

