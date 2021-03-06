//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS declaration parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Factory.hpp"

#include "CC/Warning.hpp"

#include "Core/TokenStream.hpp"

#include "SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Parser::getDecl
   //
   SR::Statement::CRef Parser::getDecl(Scope_Global &scope)
   {
      if(in.peek(Core::TOK_Semico))
      {
         Core::Origin pos = in.get().pos;
         CC::WarnFileSemico(pos, "extraneous file-scope semicolon");
         return fact.stCreate_Empty({}, in.reget().pos);
      }

      if(in.peek(Core::TOK_KeyWrd) || in.peek(Core::TOK_Identi)) switch(in.peek().str)
      {
      case Core::STR_createtranslation: return getDecl_CreateTrans(scope);
      case Core::STR_enum:              return getDecl_Enum(scope);
      case Core::STR_function:          return getDecl_Function(scope);
      case Core::STR_print:             return getDecl_Print(scope);
      case Core::STR_script:            return getDecl_Script(scope);
      case Core::STR_special:           return getDecl_Special(scope);
      case Core::STR_struct:            return getDecl_Struct(scope);
      default: break;
      }

      return getDecl_Object(scope);
   }

   //
   // Parser::getDecl
   //
   SR::Statement::CRef Parser::getDecl(CC::Scope_Local &scope,
      SR::Attribute &&, Labels &&labels)
   {
      return getDecl_Object(scope, std::move(labels));
   }

   //
   // Parser::isDecl
   //
   bool Parser::isDecl(CC::Scope &scope)
   {
      return isDeclSpec(scope);
   }
}

// EOF

