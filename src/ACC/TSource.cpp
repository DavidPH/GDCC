//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS tokenizing.
//
//-----------------------------------------------------------------------------

#include "ACC/TSource.hpp"

#include "Core/Exception.hpp"
#include "Core/Parse.hpp"
#include "Core/Token.hpp"

#include <cctype>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // IsIdentiChar
   //
   static bool IsIdentiChar(int c)
   {
      return std::isalnum(c) || c == '_' || c > 0x80;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // TSource::v_getToken
   //
   Core::Token TSource::v_getToken()
   {
      Core::Token tok;

      tok.pos = orig.getOrigin();

      int c = in.get();

      // Basic tokens.
      switch(c)
      {
      case ':':  return GDCC_Core_Token_SetStrTok(tok, Colon);
      case ',':  return GDCC_Core_Token_SetStrTok(tok, Comma);
      case '~':  return GDCC_Core_Token_SetStrTok(tok, Inv);
      case '?':  return GDCC_Core_Token_SetStrTok(tok, Query);
      case ';':  return GDCC_Core_Token_SetStrTok(tok, Semico);
      case '{':  return GDCC_Core_Token_SetStrTok(tok, BraceO);
      case '}':  return GDCC_Core_Token_SetStrTok(tok, BraceC);
      case '[':  return GDCC_Core_Token_SetStrTok(tok, BrackO);
      case ']':  return GDCC_Core_Token_SetStrTok(tok, BrackC);
      case '(':  return GDCC_Core_Token_SetStrTok(tok, ParenO);
      case ')':  return GDCC_Core_Token_SetStrTok(tok, ParenC);
      case '\n': return GDCC_Core_Token_SetStrTok(tok, LnEnd);

      case '+':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(tok, AddEq);
         if(c == '+') return GDCC_Core_Token_SetStrTok(tok, Add2);
         in.unget();  return GDCC_Core_Token_SetStrTok(tok, Add);

      case '&':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(tok, AndEq);
         if(c == '&') return GDCC_Core_Token_SetStrTok(tok, And2);
         in.unget();  return GDCC_Core_Token_SetStrTok(tok, And);

      case '>':
         c = in.get();
         if(c == '=')    return GDCC_Core_Token_SetStrTok(tok, CmpGE);
         if(c == '>') {c = in.get();
            if(c == '=') return GDCC_Core_Token_SetStrTok(tok, ShREq);
            in.unget();  return GDCC_Core_Token_SetStrTok(tok, ShR);}
         in.unget();     return GDCC_Core_Token_SetStrTok(tok, CmpGT);

      case '<':
         c = in.get();
         if(c == '=')    return GDCC_Core_Token_SetStrTok(tok, CmpLE);
         if(c == '<') {c = in.get();
            if(c == '=') return GDCC_Core_Token_SetStrTok(tok, ShLEq);
            in.unget();  return GDCC_Core_Token_SetStrTok(tok, ShL);}
         in.unget();     return GDCC_Core_Token_SetStrTok(tok, CmpLT);

      case '.':
         // Is this actually a number?
         if(std::isdigit(in.peek())) break;

         c = in.get();
         if(c == '.') {c = in.get();
            if(c == '.') return GDCC_Core_Token_SetStrTok(tok, Dot3);
            in.unget();  return GDCC_Core_Token_SetStrTok(tok, Dot2);}
         in.unget();     return GDCC_Core_Token_SetStrTok(tok, Dot);

      case '/':
         c = in.get();
         if(c == '/') return SkipCommentL(in, tok);
         if(c == '*') return SkipCommentB(in, tok);
         if(c == '=') return GDCC_Core_Token_SetStrTok(tok, DivEq);
         in.unget();  return GDCC_Core_Token_SetStrTok(tok, Div);

      case '=':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(tok, CmpEQ);
         in.unget();  return GDCC_Core_Token_SetStrTok(tok, Equal);

      case '|':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(tok, OrIEq);
         if(c == '|') return GDCC_Core_Token_SetStrTok(tok, OrI2);
         in.unget();  return GDCC_Core_Token_SetStrTok(tok, OrI);

      case '%':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(tok, ModEq);
         in.unget();  return GDCC_Core_Token_SetStrTok(tok, Mod);

      case '*':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(tok, MulEq);
         in.unget();  return GDCC_Core_Token_SetStrTok(tok, Mul);

      case '!':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(tok, CmpNE);
         in.unget();  return GDCC_Core_Token_SetStrTok(tok, Not);

      case '-':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(tok, SubEq);
         if(c == '-') return GDCC_Core_Token_SetStrTok(tok, Sub2);
         if(c == '>') return GDCC_Core_Token_SetStrTok(tok, Mem);
         in.unget();  return GDCC_Core_Token_SetStrTok(tok, Sub);

      case '^':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(tok, OrXEq);
         in.unget();  return GDCC_Core_Token_SetStrTok(tok, OrX);

      case '#':
         c = in.get();
         if(c == '#') return GDCC_Core_Token_SetStrTok(tok, Hash2);
         in.unget();  return GDCC_Core_Token_SetStrTok(tok, Hash);
      }

      // Whitespace token.
      if(std::isspace(c))
      {
         std::string str;

         do str += static_cast<char>(c);
         while((c = in.get()) != EOF && (std::isspace(c) && c != '\n'));
         if(c != EOF) in.unget();

         tok.str = {str.data(), str.size()};
         tok.tok = Core::TOK_WSpace;
         return tok;
      }

      // Quoted string/character token.
      if(c == '"' || c == '\'')
      {
         in.unget();

         try
         {
            std::string str = Core::ReadStringC(in, c);
            tok.str = {str.data(), str.size()};
            tok.tok = c == '"' ? Core::TOK_String : Core::TOK_Charac;

            return tok;
         }
         catch(Core::Exception &e)
         {
            e.setOrigin(tok.pos);
            throw;
         }
      }

      // Number token.
      if(std::isdigit(c) || c == '.')
      {
         in.putback(c);

         std::string str = Core::ReadNumberC(in);
         tok.str = {str.data(), str.size()};
         tok.tok = Core::TOK_Number;

         return tok;
      }

      // Identifier token.
      if(IsIdentiChar(c))
      {
         std::string str;

         do str += static_cast<char>(std::tolower(c));
         while((c = in.get()) != EOF && IsIdentiChar(c));
         if(c != EOF) in.unget();

         tok.str = {str.data(), str.size()};
         tok.tok = Core::TOK_Identi;
         return tok;
      }

      // EOF token.
      if(c == EOF)
      {
         tok.str = Core::STRNULL;
         tok.tok = Core::TOK_EOF;
         return tok;
      }

      // Non-token character sequence. Might be meaningful later.
      char str[1] = {static_cast<char>(c)};
      tok.str = {str, 1};
      tok.tok = Core::TOK_ChrSeq;
      return tok;
   }
}

// EOF

