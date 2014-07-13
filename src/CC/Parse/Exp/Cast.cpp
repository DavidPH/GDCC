//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C cast-expression parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp.hpp"

#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GetExp_CLit
      //
      AST::Exp::CRef GetExp_CLit(ParserCtx const &ctx, Scope &scope)
      {
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

         auto type = GetType(ctx, scope);

         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         return GetExp_CLit(ctx, scope, type);
      }

      //
      // GetExp_CLit
      //
      AST::Exp::CRef GetExp_CLit(ParserCtx const &ctx, Scope &, AST::Type const *)
      {
         if(ctx.in.peek().tok != Core::TOK_BraceO)
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '{'");

         throw Core::ExceptStr(ctx.in.peek().pos, "compound literal stub");
      }

      //
      // GetExp_Cast
      //
      AST::Exp::CRef GetExp_Cast(ParserCtx const &ctx, Scope &scope)
      {
         if(IsExp_Cast(ctx, scope))
         {
            // (
            auto pos = ctx.in.get().pos;

            // type-name
            auto type = GetType(ctx, scope);

            // )
            if(!ctx.in.drop(Core::TOK_ParenC))
               throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

            // Compound literal.
            if(ctx.in.peek().tok == Core::TOK_BraceO)
            {
               // Parse as though this is actually a postfix-expression.
               return GetExp_Post(ctx, scope, GetExp_CLit(ctx, scope, type));
            }

            return ExpCreate_Cst(type, GetExp_Cast(ctx, scope), pos);
         }

         return GetExp_Unar(ctx, scope);
      }

      //
      // IsExp_Cast
      //
      bool IsExp_Cast(ParserCtx const &ctx, Scope &scope)
      {
         if(ctx.in.drop(Core::TOK_ParenO))
         {
            bool res = IsType(ctx, scope);
            ctx.in.unget();
            return res;
         }

         return false;
      }
   }
}

// EOF

