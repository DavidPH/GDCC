//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C enum-specifier parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp.hpp"
#include "CC/Scope.hpp"
#include "CC/Type/Enum.hpp"

#include "SR/Attribute.hpp"
#include "SR/Exp.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Parser::parseTypeSpec_enum
   //
   // enum-specifier:
   //    <enum> identifier(opt) { enumerator-list }
   //    <enum> identifier(opt) { enumerator-list , }
   //    <enum> identifier
   //
   void Parser::parseTypeSpec_enum(Scope &scope, SR::Attribute &attr, TypeSpec &spec)
   {
      if(spec.specBase)
         Core::Error(in.reget().pos, "multiple type-specifier base");

      spec.specBase = TypeSpec::BaseName;

      Type_Enum::Ptr type;

      // Named enumeration.
      if(in.peek().tok == Core::TOK_Identi)
      {
         auto name  = in.get();
         bool defin = in.peek().tok == Core::TOK_BraceO;

         // Start with a local lookup. If the full lookup type is
         // incompatible, it is only an error if it is incompatible with a
         // type from this scope.
         auto lookup = scope.findTypeTag(name.str);
         bool local  = lookup;

         // If definition, only check for existing in current scope.
         if(!lookup && !defin) lookup = scope.lookupTypeTag(name.str);

         // Existing type, check for compatibility.
         if(lookup)
         {
            char const *err = nullptr;

            if(!lookup->isCTypeEnum())
               err = "tag redefined as enum";

            else if(defin && lookup->isTypeComplete())
               err = "tag redefined";

            if(err)
            {
               if(local) Core::Error(name.pos, err);
               lookup = nullptr;
            }
         }

         // No existing compatible type.
         if(!lookup)
         {
            type = Type_Enum::Create(name.str);
            scope.addTypeTag(name.str, type);
         }
         else
            type = static_cast<Type_Enum *>(&*lookup);
      }

      // Anonymous enumeration.
      else
      {
         // Must be a definition.
         if(!in.peek(Core::TOK_BraceO))
            Core::ErrorExpect("identifier", in.peek());

         type = Type_Enum::Create(nullptr);
      }

      attr.type = type;

      // Remainder of this function deals with parsing the definition itself.
      if(!in.drop(Core::TOK_BraceO))
         return;

      if(in.peek().tok == Core::TOK_BraceC)
         Core::Error(in.peek().pos, "empty enumerator-list");

      Core::Integ value = 0;

      // enumerator-list:
      //    enumerator
      //    enumerator-list , enumerator
      do
      {
         if(in.peek().tok == Core::TOK_BraceC) break;

         // enumerator:
         //    enumeration-constant
         //    enumeration-constant = constant-expression
         auto name = expectIdenti().str;

         // = constant-expression
         if(in.drop(Core::TOK_Equal))
            value = ExpToInteg(getExp_Cond(scope));

         // Add constant to scope.
         scope.addEnum(name, value);

         ++value;
      }
      while(in.drop(Core::TOK_Comma));

      expect(Core::TOK_BraceC);

      type->setComplete(TypeIntegPrS);
   }
}

// EOF

