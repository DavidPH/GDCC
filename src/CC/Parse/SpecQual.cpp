//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
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
      // Parser::isSpecQual
      //
      bool Parser::isSpecQual(Scope &scope)
      {
         return
            isAttrSpec(scope) ||
            isTypeQual(scope) ||
            isTypeSpec(scope);
      }

      //
      // Parser::parseSpecQual
      //
      void Parser::parseSpecQual(Scope &scope, AST::Attribute &attr)
      {
         auto pos = in.peek().pos;

         AST::TypeQual declQual = AST::QualNone;
         TypeSpec      declSpec;

         // Read specifier-qualifier tokens until there are no more.
         for(;;)
         {
            // attribute-specifier
            if(isAttrSpec(scope))
               parseAttrSpec(scope, attr);

            // type-specifier
            else if(isTypeSpec(scope))
               parseTypeSpec(scope, attr, declSpec);

            // type-qualifier
            else if(isTypeQual(scope))
                parseTypeQual(scope, declQual);

            else
               break;
         }

         // Finalize the type specifiers.
         declSpec.finish(attr, declQual, pos);
      }
   }
}

// EOF

