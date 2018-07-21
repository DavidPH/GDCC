//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS type-specifier parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Pragma.hpp"

#include "CC/Exp.hpp"
#include "CC/Scope.hpp"
#include "CC/Type/Enum.hpp"
#include "CC/Type/Struct.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "Platform/Platform.hpp"

#include "SR/Attribute.hpp"
#include "SR/Exp.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Parser::isTypeSpec
   //
   bool Parser::isTypeSpec(CC::Scope &scope)
   {
      auto const &tok = in.peek();
      if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
         return false;

      switch(tok.str)
      {
         // type-specifier
      case Core::STR_bool:  return true;
      case Core::STR_float: return true;
      case Core::STR_fixed: return true;
      case Core::STR_int:   return true;
      case Core::STR_str:   return true;
      case Core::STR_void:  return true;

         // typedef-name
      default: return scope.lookup(tok.str).res == CC::Lookup::Type;
      }
   }

   //
   // Parser::parseTypeSpec
   //
   void Parser::parseTypeSpec(CC::Scope &scope, SR::Attribute &attr, CC::TypeSpec &spec)
   {
      auto const &tok = in.get();
      if(tok.tok != Core::TOK_Identi && tok.tok != Core::TOK_KeyWrd)
         throw Core::ParseExceptExpect(tok, "type-specifier", false);

      //
      // setSpecBase
      //
      auto setSpecBase = [&](CC::TypeSpec::SpecBase base)
      {
         if(spec.specBase)
            throw Core::ExceptStr(tok.pos, "multiple type-specifier base");

         spec.specBase = base;
      };

      switch(tok.str)
      {
         // type-specifier
      case Core::STR_bool:  setSpecBase(CC::TypeSpec::BaseBool); break;
      case Core::STR_float: setSpecBase(CC::TypeSpec::BaseFloa); break;
      case Core::STR_int:   setSpecBase(CC::TypeSpec::BaseInte); break;
      case Core::STR_str:   setSpecBase(CC::TypeSpec::BaseStri); break;
      case Core::STR_void:  setSpecBase(CC::TypeSpec::BaseVoid); break;

      case Core::STR_fixed:
         if(prag.stateFixedType)
            setSpecBase(CC::TypeSpec::BaseAccu);
         else
            setSpecBase(CC::TypeSpec::BaseInte);
         break;

      default:
         // typedef-name
         auto lookup = scope.lookup(tok.str);
         if(lookup.res != CC::Lookup::Type)
            throw Core::ParseExceptExpect(tok, "type-specifier", false);

         setSpecBase(CC::TypeSpec::BaseName);
         attr.type = lookup.resType;

         break;
      }
   }
}

// EOF

