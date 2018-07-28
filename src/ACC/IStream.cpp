//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS input stream.
//
//-----------------------------------------------------------------------------

#include "ACC/IStream.hpp"

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
   // operator IStream >> Core::Token
   //
   IStream &operator >> (IStream &in, Core::Token &out)
   {
      out.pos = in.getOrigin();

      int c = in.get();

      // Basic tokens.
      switch(c)
      {
      case ':':  GDCC_Core_Token_SetStrTok(out, Colon);  return in;
      case ',':  GDCC_Core_Token_SetStrTok(out, Comma);  return in;
      case '~':  GDCC_Core_Token_SetStrTok(out, Inv);    return in;
      case '?':  GDCC_Core_Token_SetStrTok(out, Query);  return in;
      case ';':  GDCC_Core_Token_SetStrTok(out, Semico); return in;
      case '{':  GDCC_Core_Token_SetStrTok(out, BraceO); return in;
      case '}':  GDCC_Core_Token_SetStrTok(out, BraceC); return in;
      case '[':  GDCC_Core_Token_SetStrTok(out, BrackO); return in;
      case ']':  GDCC_Core_Token_SetStrTok(out, BrackC); return in;
      case '(':  GDCC_Core_Token_SetStrTok(out, ParenO); return in;
      case ')':  GDCC_Core_Token_SetStrTok(out, ParenC); return in;
      case '\n': GDCC_Core_Token_SetStrTok(out, LnEnd);  return in;

      case '+':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(out, AddEq), in;
         if(c == '+') return GDCC_Core_Token_SetStrTok(out, Add2), in;
         in.unget();  return GDCC_Core_Token_SetStrTok(out, Add), in;

      case '&':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(out, AndEq), in;
         if(c == '&') return GDCC_Core_Token_SetStrTok(out, And2), in;
         in.unget();  return GDCC_Core_Token_SetStrTok(out, And), in;

      case '>':
         c = in.get();
         if(c == '=')    return GDCC_Core_Token_SetStrTok(out, CmpGE), in;
         if(c == '>') {c = in.get();
            if(c == '=') return GDCC_Core_Token_SetStrTok(out, ShREq), in;
            in.unget();  return GDCC_Core_Token_SetStrTok(out, ShR), in;}
         in.unget();     return GDCC_Core_Token_SetStrTok(out, CmpGT), in;

      case '<':
         c = in.get();
         if(c == '=')    return GDCC_Core_Token_SetStrTok(out, CmpLE), in;
         if(c == '<') {c = in.get();
            if(c == '=') return GDCC_Core_Token_SetStrTok(out, ShLEq), in;
            in.unget();  return GDCC_Core_Token_SetStrTok(out, ShL), in;}
         in.unget();     return GDCC_Core_Token_SetStrTok(out, CmpLT), in;

      case '.':
         // Is this actually a number?
         if(std::isdigit(in.peek())) break;

         c = in.get();
         if(c == '.') {c = in.get();
            if(c == '.') return GDCC_Core_Token_SetStrTok(out, Dot3), in;
            in.unget();  return GDCC_Core_Token_SetStrTok(out, Dot2), in;}
         in.unget();     return GDCC_Core_Token_SetStrTok(out, Dot), in;

      case '/':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(out, DivEq), in;
         in.unget();  return GDCC_Core_Token_SetStrTok(out, Div), in;

      case '=':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(out, CmpEQ), in;
         in.unget();  return GDCC_Core_Token_SetStrTok(out, Equal), in;

      case '|':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(out, OrIEq), in;
         if(c == '|') return GDCC_Core_Token_SetStrTok(out, OrI2), in;
         in.unget();  return GDCC_Core_Token_SetStrTok(out, OrI), in;

      case '%':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(out, ModEq), in;
         in.unget();  return GDCC_Core_Token_SetStrTok(out, Mod), in;

      case '*':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(out, MulEq), in;
         in.unget();  return GDCC_Core_Token_SetStrTok(out, Mul), in;

      case '!':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(out, CmpNE), in;
         in.unget();  return GDCC_Core_Token_SetStrTok(out, Not), in;

      case '-':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(out, SubEq), in;
         if(c == '-') return GDCC_Core_Token_SetStrTok(out, Sub2), in;
         if(c == '>') return GDCC_Core_Token_SetStrTok(out, Mem), in;
         in.unget();  return GDCC_Core_Token_SetStrTok(out, Sub), in;

      case '^':
         c = in.get();
         if(c == '=') return GDCC_Core_Token_SetStrTok(out, OrXEq), in;
         in.unget();  return GDCC_Core_Token_SetStrTok(out, OrX), in;

      case '#':
         c = in.get();
         if(c == '#') return GDCC_Core_Token_SetStrTok(out, Hash2), in;
         in.unget();  return GDCC_Core_Token_SetStrTok(out, Hash), in;
      }

      // Whitespace token.
      if(std::isspace(c))
      {
         std::string str;

         do str += static_cast<char>(c);
         while((c = in.get()) != EOF && (std::isspace(c) && c != '\n'));
         if(c != EOF) in.unget();

         out.str = {str.data(), str.size()};
         out.tok = Core::TOK_WSpace;
         return in;
      }

      // Quoted string/character token.
      if(c == '"' || c == '\'')
      {
         auto hold = in.holdComments();

         in.unget();

         try
         {
            std::string str = Core::ReadStringC(in, c);
            out.str = {str.data(), str.size()};
            out.tok = c == '"' ? Core::TOK_String : Core::TOK_Charac;

            return in;
         }
         catch(Core::Exception &e)
         {
            e.setOrigin(out.pos);
            throw;
         }
      }

      // Number token.
      if(std::isdigit(c) || c == '.')
      {
         in.putback(c);

         std::string str = Core::ReadNumberC(in);
         out.str = {str.data(), str.size()};
         out.tok = Core::TOK_Number;

         return in;
      }

      // Identifier token.
      if(IsIdentiChar(c))
      {
         std::string str;

         do str += static_cast<char>(std::tolower(c));
         while((c = in.get()) != EOF && IsIdentiChar(c));
         if(c != EOF) in.unget();

         out.str = {str.data(), str.size()};
         out.tok = Core::TOK_Identi;
         return in;
      }

      // EOF token.
      if(c == EOF)
      {
         out.str = Core::STRNULL;
         out.tok = Core::TOK_EOF;
         return in;
      }

      // Non-token character sequence. Might be meaningful later.
      char str[1] = {static_cast<char>(c)};
      out.str = {str, 1};
      out.tok = Core::TOK_ChrSeq;
      return in;
   }

   //
   // ParseEscape
   //
   bool ParseEscape(std::ostream &out, std::istream &in, char escape)
   {
      switch(escape)
      {
      case 'c': out.put('\x1C'); return true;

      case '\'':
      case '\"':
      case '\\':

      case 'a':
      case 'b':
      case 'f':
      case 'n':
      case 'r':
      case 't':
      case 'v':
      case 'x':

      case '0': case '1': case '2': case '3':
      case '4': case '5': case '6': case '7':
         return Core::ParseEscapeC(out, in, escape);

      default:
         return false;
      }
   }
}

// EOF

