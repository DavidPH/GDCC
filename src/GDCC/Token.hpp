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
// Macros                                                                     |
//

#define GDCC_Token_SetStrTok(tok, name) \
   ((tok).setStrTok(::GDCC::STR_TOK_##name, ::GDCC::TOK_##name))


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
      // String tokens.
      TOK_Character,
      TOK_Identifier,
      TOK_Number,
      TOK_String,

      // Simple tokens.
      TOK_Comma, // ,
      TOK_Equal, // =

      // Paired tokens.
      TOK_BraceO, // {
      TOK_BraceC, // }
      TOK_BrackO, // [
      TOK_BrackC, // ]
      TOK_ParenO, // (
      TOK_ParenC, // )

      // Whitespace tokens.
      TOK_EOL, // \n

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

      Token &setStrTok(String str_, TokenType tok_) {str = str_; tok = tok_; return *this;}

      Origin    pos;
      String    str;
      TokenType tok;
   };
}

#endif//GDCC__Token_H__

