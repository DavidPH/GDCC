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
      return in.peek(Core::TOK_KeyWrd, Core::STR__Atomic, Core::TOK_ParenO);
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
         Core::Error(in.reget().pos, "multiple type-specifier base");

      spec.specBase = TypeSpec::BaseName;

      // (
      expect(Core::TOK_ParenO);

      // type-name
      auto type = getType(scope);

      // )
      expect(Core::TOK_ParenC);

      // Constraints.
      if(type->isTypeArray())
         Core::Error(in.peek().pos, "atomic array");

      if(type->isCTypeFunction())
         Core::Error(in.peek().pos, "atomic function");

      if(type->getQual())
         Core::Error(in.peek().pos, "atomic qualified");

      // Set attribute type.
      auto qual = type->getQual();
      qual.aAtom = true;
      attr.type = type->getTypeQual(qual);
   }
}

// EOF

