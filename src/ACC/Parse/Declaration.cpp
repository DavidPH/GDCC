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
      // GetDecl
      //
      AST::Statement::CRef GetDecl(ParserCtx const &ctx, Scope_Global &scope)
      {
         if(ctx.in.peek(Core::TOK_KeyWrd, Core::STR_function))
            return GetDecl_Function(ctx, scope);

         if(ctx.in.peek(Core::TOK_Identi, Core::STR_print))
            return GetDecl_Print(ctx, scope);

         if(ctx.in.peek(Core::TOK_KeyWrd, Core::STR_script))
            return GetDecl_Script(ctx, scope);

         if(ctx.in.peek(Core::TOK_KeyWrd, Core::STR_special))
            return GetDecl_Special(ctx, scope);

         return GetDecl_Object(ctx, scope);
      }

      //
      // GetDecl
      //
      AST::Statement::CRef GetDecl(ParserCtx const &ctx, CC::Scope_Local &scope)
      {
         return GetDecl_Object(ctx, scope);
      }

      //
      // IsDecl
      //
      bool IsDecl(ParserCtx const &ctx, CC::Scope &scope)
      {
         return IsDeclSpec(ctx, scope);
      }
   }
}

// EOF

