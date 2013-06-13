//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source tokens.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Token_H__
#define GDCC__Token_H__

#include "Origin.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   //
   // TokenType
   //
   enum TokenType
   {
      TOK_Character,
      TOK_Identifier,
      TOK_String,

      TOK_EOF
   };

   //
   // Token
   //
   struct Token
   {
      Token() = default;
      Token(String str_, TokenType tok_) : pos{STRNULL, 0}, str{str_}, tok{tok_} {}
      Token(Origin pos_, String str_, TokenType tok_) : pos{pos_}, str{str_}, tok{tok_} {}

      Origin    pos;
      String    str;
      TokenType tok;
   };
}

#endif//GDCC__Token_H__

