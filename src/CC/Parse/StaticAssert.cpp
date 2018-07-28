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

      expect(Core::TOK_KeyWrd, Core::STR__Static_assert);
      expect(Core::TOK_ParenO);
      auto exp = getExp_Cond(scope);
      expect(Core::TOK_Comma);
      auto msg = expectString();
      expect(Core::TOK_ParenC);

      if(!ExpToInteg(exp))
         Core::Error(msg.pos, msg.str);
   }
}

// EOF

