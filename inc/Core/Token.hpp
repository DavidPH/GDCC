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

#ifndef GDCC__Core__Token_H__
#define GDCC__Core__Token_H__

#include "Origin.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define GDCC_Core_Token_SetStrTok(tok, name) \
   ((tok).setStrTok(::GDCC::Core::STR_TOK_##name, ::GDCC::Core::TOK_##name))


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // TokenType
      //
      enum TokenType
      {
         // String tokens.
         TOK_Charac,
         TOK_ChrSeq,
         TOK_HdrStr,
         TOK_Header,
         TOK_Identi,
         TOK_KeyWrd,
         TOK_NumFix,
         TOK_NumFlt,
         TOK_NumInt,
         TOK_Number,
         TOK_String,
         TOK_WSpace,

         // Simple tokens.
         TOK_Add,    // +
         TOK_Add2,   // ++
         TOK_AddEq,  // +=
         TOK_And,    // &
         TOK_And2,   // &&
         TOK_And2Eq, // &&=
         TOK_AndEq,  // &=
         TOK_CmpEQ,  // ==
         TOK_CmpGE,  // >=
         TOK_CmpGT,  // >
         TOK_CmpLE,  // <=
         TOK_CmpLT,  // <
         TOK_CmpNE,  // !=
         TOK_Colon,  // :
         TOK_Colon2, // ::
         TOK_Comma,  // ,
         TOK_Div,    // /
         TOK_DivEq,  // /=
         TOK_Dot,    // .
         TOK_Dot2,   // ..
         TOK_Dot3,   // ...
         TOK_DotPtr, // .*
         TOK_Equal,  // =
         TOK_Hash,   // #
         TOK_Hash2,  // ##
         TOK_Inv,    // ~
         TOK_Mem,    // ->
         TOK_MemPtr, // ->*
         TOK_Mod,    // %
         TOK_ModEq,  // %=
         TOK_Mul,    // *
         TOK_MulEq,  // *=
         TOK_Not,    // !
         TOK_OrI,    // |
         TOK_OrI2,   // ||
         TOK_OrI2Eq, // ||=
         TOK_OrIEq,  // |=
         TOK_OrX,    // ^
         TOK_OrX2,   // ^^
         TOK_OrX2Eq, // ^^=
         TOK_OrXEq,  // ^=
         TOK_Query,  // ?
         TOK_Semico, // ;
         TOK_ShL,    // <<
         TOK_ShLEq,  // <<=
         TOK_ShR,    // >>
         TOK_ShREq,  // >>=
         TOK_Sub,    // -
         TOK_Sub2,   // --
         TOK_SubEq,  // -=

         // Paired tokens.
         TOK_BraceO, // {
         TOK_BraceC, // }
         TOK_BrackO, // [
         TOK_BrackC, // ]
         TOK_ParenO, // (
         TOK_ParenC, // )

         // Whitespace tokens.
         TOK_LnEnd, // \n
         TOK_Space, // ' '
         TOK_Tabul, // \t

         // Non-tokens
         TOK_Marker,
         TOK_EOF,

         // Alternate spelling aliases.
         TOK_DG_BraceO = TOK_BraceO, // <%
         TOK_DG_BraceC = TOK_BraceC, // %>
         TOK_DG_BrackO = TOK_BrackO, // <:
         TOK_DG_BrackC = TOK_BrackC, // :>
         TOK_DG_Hash   = TOK_Hash,   // %:
         TOK_DG_Hash2  = TOK_Hash2,  // %:%:
      };

      //
      // Token
      //
      struct Token
      {
         Token() = default;
         constexpr Token(Origin pos_, String str_, TokenType tok_) :
            pos{pos_}, str{str_}, tok{tok_} {}

         constexpr bool operator == (Token const &t) const
            {return pos == t.pos && str == t.str && tok == t.tok;}

         constexpr bool operator != (Token const &t) const
            {return pos != t.pos || str != t.str || tok != t.tok;}

         Token &setStrTok(String str_, TokenType tok_)
            {str = str_; tok = tok_; return *this;}

         Origin    pos;
         String    str;
         TokenType tok;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace Core
   {
      constexpr Token TokenEOF{Origin(STRNULL, 0), STRNULL, TOK_EOF};
   }
}

#endif//GDCC__Core__Token_H__

