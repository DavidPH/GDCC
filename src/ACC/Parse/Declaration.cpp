//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS declaration parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "AST/Statement.hpp"

#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // Parser::getDecl
      //
      AST::Statement::CRef Parser::getDecl(Scope_Global &scope)
      {
         if(in.peek(Core::TOK_KeyWrd, Core::STR_function))
            return getDecl_Function(scope);

         if(in.peek(Core::TOK_Identi, Core::STR_print))
            return getDecl_Print(scope);

         if(in.peek(Core::TOK_KeyWrd, Core::STR_script))
            return getDecl_Script(scope);

         if(in.peek(Core::TOK_KeyWrd, Core::STR_special))
            return getDecl_Special(scope);

         if(in.peek(Core::TOK_KeyWrd, Core::STR_struct))
            return getDecl_Struct(scope);

         return getDecl_Object(scope);
      }

      //
      // Parser::getDecl
      //
      AST::Statement::CRef Parser::getDecl(CC::Scope_Local &scope)
      {
         return getDecl_Object(scope);
      }

      //
      // Parser::isDecl
      //
      bool Parser::isDecl(CC::Scope &scope)
      {
         return isDeclSpec(scope);
      }
   }
}

// EOF

