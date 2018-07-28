//-----------------------------------------------------------------------------
//
// Copyright (C) 2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source tokens.
//
//-----------------------------------------------------------------------------

#include "Core/Token.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   //
   // GetString
   //
   Core::String Token::GetString(TokenType tok)
   {
      switch(tok)
      {
      // String tokens.
         case TOK_Charac:
         case TOK_ChrSeq:
         case TOK_ChrU16:
         case TOK_ChrU32:
         case TOK_ChrWid:
         case TOK_HdrStr:
         case TOK_Header:
         case TOK_Identi:
         case TOK_KeyWrd:
         case TOK_NumFix:
         case TOK_NumFlt:
         case TOK_NumInt:
         case TOK_Number:
         case TOK_StrChr:
         case TOK_StrIdx:
         case TOK_StrU08:
         case TOK_StrU16:
         case TOK_StrU32:
         case TOK_StrWid:
         case TOK_String:
         case TOK_WSpace:
            return STR_;

      // Simple tokens.
         case TOK_Add:    return STR_TOK_Add;    // +
         case TOK_Add2:   return STR_TOK_Add2;   // ++
         case TOK_AddEq:  return STR_TOK_AddEq;  // +=
         case TOK_And:    return STR_TOK_And;    // &
         case TOK_And2:   return STR_TOK_And2;   // &&
         case TOK_And2Eq: return STR_TOK_And2Eq; // &&=
         case TOK_AndEq:  return STR_TOK_AndEq;  // &=
         case TOK_CmpEQ:  return STR_TOK_CmpEQ;  // ==
         case TOK_CmpGE:  return STR_TOK_CmpGE;  // >=
         case TOK_CmpGT:  return STR_TOK_CmpGT;  // >
         case TOK_CmpLE:  return STR_TOK_CmpLE;  // <=
         case TOK_CmpLT:  return STR_TOK_CmpLT;  // <
         case TOK_CmpNE:  return STR_TOK_CmpNE;  // !=
         case TOK_Colon:  return STR_TOK_Colon;  // :
         case TOK_Colon2: return STR_TOK_Colon2; // ::
         case TOK_Comma:  return STR_TOK_Comma;  // ,
         case TOK_Div:    return STR_TOK_Div;    // /
         case TOK_DivEq:  return STR_TOK_DivEq;  // /=
         case TOK_Dot:    return STR_TOK_Dot;    // .
         case TOK_Dot2:   return STR_TOK_Dot2;   // ..
         case TOK_Dot3:   return STR_TOK_Dot3;   // ...
         case TOK_DotPtr: return STR_TOK_DotPtr; // .*
         case TOK_Equal:  return STR_TOK_Equal;  // =
         case TOK_Hash:   return STR_TOK_Hash;   // #
         case TOK_Hash2:  return STR_TOK_Hash2;  // ##
         case TOK_Inv:    return STR_TOK_Inv;    // ~
         case TOK_Mem:    return STR_TOK_Mem;    // ->
         case TOK_MemPtr: return STR_TOK_MemPtr; // ->*
         case TOK_Mod:    return STR_TOK_Mod;    // %
         case TOK_ModEq:  return STR_TOK_ModEq;  // %=
         case TOK_Mul:    return STR_TOK_Mul;    // *
         case TOK_MulEq:  return STR_TOK_MulEq;  // *=
         case TOK_Not:    return STR_TOK_Not;    // !
         case TOK_OrI:    return STR_TOK_OrI;    // |
         case TOK_OrI2:   return STR_TOK_OrI2;   // ||
         case TOK_OrI2Eq: return STR_TOK_OrI2Eq; // ||=
         case TOK_OrIEq:  return STR_TOK_OrIEq;  // |=
         case TOK_OrX:    return STR_TOK_OrX;    // ^
         case TOK_OrX2:   return STR_TOK_OrX2;   // ^^
         case TOK_OrX2Eq: return STR_TOK_OrX2Eq; // ^^=
         case TOK_OrXEq:  return STR_TOK_OrXEq;  // ^=
         case TOK_Query:  return STR_TOK_Query;  // ?
         case TOK_Semico: return STR_TOK_Semico; // ;
         case TOK_ShL:    return STR_TOK_ShL;    // <<
         case TOK_ShLEq:  return STR_TOK_ShLEq;  // <<=
         case TOK_ShR:    return STR_TOK_ShR;    // >>
         case TOK_ShREq:  return STR_TOK_ShREq;  // >>=
         case TOK_Sub:    return STR_TOK_Sub;    // -
         case TOK_Sub2:   return STR_TOK_Sub2;   // --
         case TOK_SubEq:  return STR_TOK_SubEq;  // -=

      // Paired tokens.
         case TOK_BraceO: return STR_TOK_BraceO; // {
         case TOK_BraceC: return STR_TOK_BraceC; // }
         case TOK_BrackO: return STR_TOK_BrackO; // [
         case TOK_BrackC: return STR_TOK_BrackC; // ]
         case TOK_ParenO: return STR_TOK_ParenO; // (
         case TOK_ParenC: return STR_TOK_ParenC; // )

      // Whitespace tokens.
         case TOK_LnEnd: // \n
         case TOK_Space: // ' '
         case TOK_Tabul: // \t
            return STR_;

      // Non-tokens
         case TOK_Marker:
         case TOK_EOF:
            return nullptr;
      }

      return nullptr;
   }
}

// EOF

