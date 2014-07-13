//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C static_assert-declaration parsing.
//
//-----------------------------------------------------------------------------

#include "CC/Parse.hpp"

#include "CC/Exp.hpp"

#include "AST/Exp.hpp"

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
      // IsStaticAssert
      //
      bool IsStaticAssert(ParserCtx const &ctx, Scope &)
      {
         auto const &tok = ctx.in.peek();
         return tok.tok == Core::TOK_KeyWrd && tok.str == Core::STR__Static_assert;
      }

      //
      // ParseStaticAssert
      //
      void ParseStaticAssert(ParserCtx const &ctx, Scope &scope)
      {
         // static_assert-declaration:
         //    <_Static_assert> ( constant-expression , string-literal )

         // <_Static_assert>
         if(!ctx.in.drop(Core::TOK_KeyWrd, Core::STR__Static_assert))
            throw Core::ExceptStr(ctx.in.peek().pos,
               "expected static_assert-declaration");

         // (
         if(!ctx.in.drop(Core::TOK_ParenO))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected '('");

         // constant-expression
         auto exp = GetExp_Cond(ctx, scope);

         // ,
         if(!ctx.in.drop(Core::TOK_Comma))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ','");

         // string-literal
         auto msg = ctx.in.get();
         if(!msg.isTokString())
            throw Core::ExceptStr(ctx.in.peek().pos, "expected string-literal");

         // )
         if(!ctx.in.drop(Core::TOK_ParenC))
            throw Core::ExceptStr(ctx.in.peek().pos, "expected ')'");

         if(!ExpToInteg(exp))
            throw Core::ExceptStr(msg.pos, msg.str);
      }
   }
}

// EOF

