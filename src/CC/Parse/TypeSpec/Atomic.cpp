//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C atomic-type-specifier parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "SR/Attribute.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Parser::isTypeSpec_atomic
   //
   bool Parser::isTypeSpec_atomic(Scope &)
   {
      // If not followed by a parenthesis, it is a type-qualifier.
      in.get();
      bool res = in.peek().tok == Core::TOK_ParenO;
      in.unget();
      return res;
   }

   //
   // Parser::parseTypeSpec_Atomic
   //
   // atomic-type-specifier:
   //    <_Atomic> ( type-name )
   //
   void Parser::parseTypeSpec_atomic(Scope &scope, SR::Attribute &attr, TypeSpec &spec)
   {
      if(spec.specBase)
         throw Core::ExceptStr(in.reget().pos, "multiple type-specifier base");

      spec.specBase = TypeSpec::BaseName;

      // (
      if(!in.drop(Core::TOK_ParenO))
         throw Core::ParseExceptExpect(in.peek(), "(", true);

      // type-name
      auto type = getType(scope);

      // )
      if(!in.drop(Core::TOK_ParenC))
         throw Core::ParseExceptExpect(in.peek(), ")", true);

      // Constraints.
      if(type->isTypeArray())
         throw Core::ExceptStr(in.peek().pos, "atomic array");

      if(type->isCTypeFunction())
         throw Core::ExceptStr(in.peek().pos, "atomic function");

      if(type->getQual())
         throw Core::ExceptStr(in.peek().pos, "atomic qualified");

      // Set attribute type.
      auto qual = type->getQual();
      qual.aAtom = true;
      attr.type = type->getTypeQual(qual);
   }
}

// EOF

