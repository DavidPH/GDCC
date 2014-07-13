//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "AST/Attribute.hpp"
#include "AST/Type.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GetType
      //
      // type-name:
      //    specifier-qualifier-list abstract-declarator(opt)
      //
      AST::Type::CRef GetType(ParserCtx const &ctx, Scope &scope)
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
      bool IsType(ParserCtx const &ctx, Scope &scope)
      {
         return IsSpecQual(ctx, scope);
      }
   }
}

// EOF

