//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
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

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "SR/Exp.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Parser::isStaticAssert
   //
   bool Parser::isStaticAssert(Scope &)
   {
      auto const &tok = in.peek();
      return tok.tok == Core::TOK_KeyWrd && tok.str == Core::STR__Static_assert;
   }

   //
   // Parser::parseStaticAssert
   //
   void Parser::parseStaticAssert(Scope &scope)
   {
      // static_assert-declaration:
      //    <_Static_assert> ( constant-expression , string-literal )

      // <_Static_assert>
      if(!in.drop(Core::TOK_KeyWrd, Core::STR__Static_assert))
         throw Core::ExceptStr(in.peek().pos,
            "expected static_assert-declaration");

      // (
      if(!in.drop(Core::TOK_ParenO))
         throw Core::ExceptStr(in.peek().pos, "expected '('");

      // constant-expression
      auto exp = getExp_Cond(scope);

      // ,
      if(!in.drop(Core::TOK_Comma))
         throw Core::ExceptStr(in.peek().pos, "expected ','");

      // string-literal
      auto msg = in.get();
      if(!msg.isTokString())
         throw Core::ExceptStr(in.peek().pos, "expected string-literal");

      // )
      if(!in.drop(Core::TOK_ParenC))
         throw Core::ExceptStr(in.peek().pos, "expected ')'");

      if(!ExpToInteg(exp))
         throw Core::ExceptStr(msg.pos, msg.str);
   }
}

// EOF

