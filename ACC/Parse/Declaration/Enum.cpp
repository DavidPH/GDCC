//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS enumeration-declaration parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Factory.hpp"
#include "ACC/Scope.hpp"

#include "CC/Exp.hpp"
#include "CC/Type/Enum.hpp"

#include "Core/TokenStream.hpp"

#include "SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Parser::getDecl_Enum
   //
   SR::Statement::CRef Parser::getDecl_Enum(Scope_Global &scope)
   {
      // enum-declaration:
      //    <enum> identifier(opt) { enumeration-member-declaration-list }
      //    <enum> identifier(opt) { enumeration-member-declaration-list , }

      // <enum>
      auto pos = expect(Core::TOK_KeyWrd, Core::STR_enum).pos;

      CC::Type_Enum::Ptr type;

      if(in.peek(Core::TOK_Identi))
      {
         auto name = in.get().str;

         type = CC::Type_Enum::Create(name);
         scope.add(name, type);
      }

      // {
      expect(Core::TOK_BraceO);

      Core::Integ value = 0;

      // enumeration-member-declaration-list:
      //    enumeration-member-declaration
      //    enumeration-member-declaration-list , enumeration-member-declaration
      do
      {
         // enumeration-member-declaration:
         //    identifier
         //    identifier = constant-expression

         auto name = expectIdenti().str;

         // = constant-expression
         if(in.drop(Core::TOK_Equal))
            value = CC::ExpToInteg(getExp_Cond(scope));

         scope.addEnum(name, value);

         ++value;
      }
      while(in.drop(Core::TOK_Comma) && !in.peek(Core::TOK_BraceC));

      // }
      expect(Core::TOK_BraceC);

      if(type)
         type->setComplete(CC::TypeIntegPrS);

      return fact.stCreate_Empty({}, pos);
   }
}

// EOF

