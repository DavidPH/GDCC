//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C input stream.
//
//-----------------------------------------------------------------------------

#include "IStream.hpp"

#include "GDCC/Parse.hpp"
#include "GDCC/Token.hpp"

#include <cctype>
#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace C
{
   //
   // operator IStream >> GDCC::Token
   //
   IStream &operator >> (IStream &in, GDCC::Token &out)
   {
      out.pos = in.getOrigin();

      int c = in.get();

      // Basic tokens.
      switch(c)
      {
      case ',':  GDCC_Token_SetStrTok(out, Comma);  return in;
      case '~':  GDCC_Token_SetStrTok(out, Inv);    return in;
      case '?':  GDCC_Token_SetStrTok(out, Query);  return in;
      case ';':  GDCC_Token_SetStrTok(out, Semico); return in;
      case '{':  GDCC_Token_SetStrTok(out, BraceO); return in;
      case '}':  GDCC_Token_SetStrTok(out, BraceC); return in;
      case '[':  GDCC_Token_SetStrTok(out, BrackO); return in;
      case ']':  GDCC_Token_SetStrTok(out, BrackC); return in;
      case '(':  GDCC_Token_SetStrTok(out, ParenO); return in;
      case ')':  GDCC_Token_SetStrTok(out, ParenC); return in;
      case '\n': GDCC_Token_SetStrTok(out, LnEnd);  return in;
      case ' ':  GDCC_Token_SetStrTok(out, Space);  return in;
      case '\t': GDCC_Token_SetStrTok(out, Tabul);  return in;

      case '+':
         c = in.get();
         if(c == '=') return GDCC_Token_SetStrTok(out, AddEq), in;
         if(c == '+') return GDCC_Token_SetStrTok(out, Add2), in;
         in.unget();  return GDCC_Token_SetStrTok(out, Add), in;

      case '&':
         c = in.get();
         if(c == '=') return GDCC_Token_SetStrTok(out, AndEq), in;
         if(c == '&') return GDCC_Token_SetStrTok(out, And2), in;
         in.unget();  return GDCC_Token_SetStrTok(out, And), in;

      case '>':
         c = in.get();
         if(c == '=')    return GDCC_Token_SetStrTok(out, CmpGE), in;
         if(c == '>') {c = in.get();
            if(c == '=') return GDCC_Token_SetStrTok(out, ShREq), in;
            in.unget();  return GDCC_Token_SetStrTok(out, ShR), in;}
         in.unget();     return GDCC_Token_SetStrTok(out, CmpGT), in;

      case '<':
         c = in.get();
         if(c == '%')    return GDCC_Token_SetStrTok(out, DG_BraceO), in;
         if(c == ':')    return GDCC_Token_SetStrTok(out, DG_BrackO), in;
         if(c == '=')    return GDCC_Token_SetStrTok(out, CmpLE), in;
         if(c == '<') {c = in.get();
            if(c == '=') return GDCC_Token_SetStrTok(out, ShLEq), in;
            in.unget();  return GDCC_Token_SetStrTok(out, ShL), in;}
         in.unget();     return GDCC_Token_SetStrTok(out, CmpLT), in;

      case ':':
         c = in.get();
         if(c == '>') return GDCC_Token_SetStrTok(out, DG_BrackC), in;
         in.unget();  return GDCC_Token_SetStrTok(out, Colon), in;

      case '.':
         // Is this actually a number?
         if(std::isdigit(in.peek())) break;

         c = in.get();
         if(c == '.') {c = in.get();
            if(c == '.') return GDCC_Token_SetStrTok(out, Dot3), in;
            in.unget();  return GDCC_Token_SetStrTok(out, Dot2), in;}
         in.unget();     return GDCC_Token_SetStrTok(out, Dot), in;

      case '/':
         c = in.get();
         if(c == '=') return GDCC_Token_SetStrTok(out, DivEq), in;
         in.unget();  return GDCC_Token_SetStrTok(out, Div), in;

      case '=':
         c = in.get();
         if(c == '=') return GDCC_Token_SetStrTok(out, CmpEQ), in;
         in.unget();  return GDCC_Token_SetStrTok(out, Equal), in;

      case '|':
         c = in.get();
         if(c == '=') return GDCC_Token_SetStrTok(out, OrIEq), in;
         if(c == '|') return GDCC_Token_SetStrTok(out, OrI2), in;
         in.unget();  return GDCC_Token_SetStrTok(out, OrI), in;

      case '%':
         c = in.get();
         if(c == '>')       return GDCC_Token_SetStrTok(out, DG_BraceC), in;
         if(c == '=')       return GDCC_Token_SetStrTok(out, ModEq), in;
         if(c == ':') {c = in.get();
            if(c == '%') {c = in.get();
               if(c == ':') return GDCC_Token_SetStrTok(out, DG_Hash2), in;
               in.unget();}
            in.unget();     return GDCC_Token_SetStrTok(out, DG_Hash), in;}
         in.unget();        return GDCC_Token_SetStrTok(out, Mod), in;

      case '*':
         c = in.get();
         if(c == '=') return GDCC_Token_SetStrTok(out, MulEq), in;
         in.unget();  return GDCC_Token_SetStrTok(out, Mul), in;

      case '!':
         c = in.get();
         if(c == '=') return GDCC_Token_SetStrTok(out, CmpNE), in;
         in.unget();  return GDCC_Token_SetStrTok(out, Not), in;

      case '-':
         c = in.get();
         if(c == '=') return GDCC_Token_SetStrTok(out, SubEq), in;
         if(c == '-') return GDCC_Token_SetStrTok(out, Sub2), in;
         if(c == '>') return GDCC_Token_SetStrTok(out, Mem), in;
         in.unget();  return GDCC_Token_SetStrTok(out, Sub), in;

      case '^':
         c = in.get();
         if(c == '=') return GDCC_Token_SetStrTok(out, OrXEq), in;
         in.unget();  return GDCC_Token_SetStrTok(out, OrX), in;

      case '#':
         c = in.get();
         if(c == '#') return GDCC_Token_SetStrTok(out, Hash2), in;
         in.unget();  return GDCC_Token_SetStrTok(out, Hash), in;
      }

      // Quoted string/character token.
      if(c == '"' || c == '\'')
      {
         auto hold = in.holdComments();

         try
         {
            std::string str  = GDCC::ReadStringC(in, c);
            std::size_t hash = GDCC::HashString(str.data(), str.size());
            out.str = GDCC::AddString(str.data(), str.size(), hash);
            out.tok = c == '"' ? GDCC::TOK_String : GDCC::TOK_Charac;

            return in;
         }
         catch(GDCC::ParseError const &e)
         {
            std::cerr << "ERROR: " << out.pos << ": " << e.what() << '\n';
            throw EXIT_FAILURE;
         }
      }

      // Number token.
      if(std::isdigit(c) || c == '.')
      {
         in.putback(c);

         std::string str  = GDCC::ReadNumberC(in);
         std::size_t hash = GDCC::HashString(str.data(), str.size());
         out.str = GDCC::AddString(str.data(), str.size(), hash);
         out.tok = GDCC::TOK_Number;

         return in;
      }

      // Identifier token.
      if(std::isalpha(c) || c == '_')
      {
         std::string str;

         do str += static_cast<char>(c);
         while((c = in.get()) != EOF && (std::isalnum(c) || c == '_'));
         if(c != EOF) in.unget();

         std::size_t hash = GDCC::HashString(str.data(), str.size());
         out.str = GDCC::AddString(str.data(), str.size(), hash);
         out.tok = GDCC::TOK_Identi;
         return in;
      }

      // EOF token.
      if(c == EOF)
      {
         out.str = GDCC::STRNULL;
         out.tok = GDCC::TOK_EOF;
         return in;
      }

      std::cerr << "ERROR: " << out.pos << ": bad token character: '"
         << static_cast<char>(c) << "'\n";
      throw EXIT_FAILURE;
   }
}

// EOF

