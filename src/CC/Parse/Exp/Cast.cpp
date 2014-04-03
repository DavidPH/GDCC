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
      AST::Exp::CRef GetExp_CLit(ParserCtx const &in, Scope &ctx)
      {
         if(!in.in.drop(Core::TOK_ParenO))
            throw Core::ExceptStr(in.in.peek().pos, "expected '('");

         auto type = GetType(in, ctx);

         if(!in.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(in.in.peek().pos, "expected ')'");

         return GetExp_CLit(in, ctx, type);
      }

      //
      // GetExp_CLit
      //
      AST::Exp::CRef GetExp_CLit(ParserCtx const &in, Scope &, AST::Type const *)
      {
         if(in.in.peek().tok != Core::TOK_BraceO)
            throw Core::ExceptStr(in.in.peek().pos, "expected '{'");

         throw Core::ExceptStr(in.in.peek().pos, "compound literal stub");
      }

      //
      // GetExp_Cast
      //
      AST::Exp::CRef GetExp_Cast(ParserCtx const &in, Scope &ctx)
      {
         if(IsExp_Cast(in, ctx))
         {
            // (
            auto pos = in.in.get().pos;

            // type-name
            auto type = GetType(in, ctx);

            // )
            if(!in.in.drop(Core::TOK_ParenC))
               throw Core::ExceptStr(in.in.peek().pos, "expected ')'");

            // Compound literal.
            if(in.in.peek().tok == Core::TOK_BraceO)
            {
               // Parse as though this is actually a postfix-expression.
               return GetExp_Post(in, ctx, GetExp_CLit(in, ctx, type));
            }

            return ExpCreate_Cst(type, GetExp_Cast(in, ctx), pos);
         }

         return GetExp_Unar(in, ctx);
      }

      //
      // IsExp_Cast
      //
      bool IsExp_Cast(ParserCtx const &in, Scope &ctx)
      {
         if(in.in.drop(Core::TOK_ParenO))
         {
            bool res = IsType(in, ctx);
            in.in.unget();
            return res;
         }

         return false;
      }
   }
}

// EOF

