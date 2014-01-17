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
      AST::Type::CRef GetType(ParserData &in, Scope *ctx)
      {
         AST::Attribute attr;

         ParseSpecQual(in, ctx, attr);
         if(IsDeclarator(in, ctx))
            ParseDeclarator(in, ctx, attr);

         return static_cast<AST::Type::CRef>(attr.type);
      }
   }
}

// EOF

