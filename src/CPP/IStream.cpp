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

#include "CPP/IStream.hpp"

#include "Core/Parse.hpp"
#include "Core/Token.hpp"

#include <cctype>
#include <iostream>


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace CPP
   {
      bool IStream::NeedHeader = false;
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // IStream::GetHeader
      //
      bool IStream::GetHeader(std::istream &in, Core::Token &out)
      {
         while(std::isspace(in.peek())) in.get();

         int c = in.get();

         // System header.
         if(c == '<')
         {
            std::string str;

            while((c = in.get()) != EOF && c != '>')
               str += static_cast<char>(c);

            std::size_t hash = Core::HashString(str.data(), str.size());
            out.str = Core::AddString(str.data(), str.size(), hash);
            out.tok = Core::TOK_Header;

            return true;
         }

         // String header.
         if(c == '"')
         {
            std::string str;

            while((c = in.get()) != EOF && c != '"')
               str += static_cast<char>(c);

            std::size_t hash = Core::HashString(str.data(), str.size());
            out.str = Core::AddString(str.data(), str.size(), hash);
            out.tok = Core::TOK_HdrStr;

            return true;
         }

         in.unget();
         return false;
      }

      //
      // operator IStream >> Core::Token
      //
      IStream &operator >> (IStream &in, Core::Token &out)
      {
         out.pos = in.getOrigin();

         // Possibly check for special header token.
         if(IStream::NeedHeader)
         {
            IStream::NeedHeader = false;
            if(IStream::GetHeader(in, out))
               return in;
         }

         int c = in.get();

         // Basic tokens.
         switch(c)
         {
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
      //case ' ':  GDCC_Core_Token_SetStrTok(out, Space);  return in;
      //case '\t': GDCC_Core_Token_SetStrTok(out, Tabul);  return in;

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
            if(c == '%')    return GDCC_Core_Token_SetStrTok(out, DG_BraceO), in;
            if(c == ':')    return GDCC_Core_Token_SetStrTok(out, DG_BrackO), in;
            if(c == '=')    return GDCC_Core_Token_SetStrTok(out, CmpLE), in;
            if(c == '<') {c = in.get();
               if(c == '=') return GDCC_Core_Token_SetStrTok(out, ShLEq), in;
               in.unget();  return GDCC_Core_Token_SetStrTok(out, ShL), in;}
            in.unget();     return GDCC_Core_Token_SetStrTok(out, CmpLT), in;

         case ':':
            c = in.get();
            if(c == '>') return GDCC_Core_Token_SetStrTok(out, DG_BrackC), in;
            in.unget();  return GDCC_Core_Token_SetStrTok(out, Colon), in;

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
            if(c == '>')       return GDCC_Core_Token_SetStrTok(out, DG_BraceC), in;
            if(c == '=')       return GDCC_Core_Token_SetStrTok(out, ModEq), in;
            if(c == ':') {c = in.get();
               if(c == '%') {c = in.get();
                  if(c == ':') return GDCC_Core_Token_SetStrTok(out, DG_Hash2), in;
                  in.unget();}
               in.unget();     return GDCC_Core_Token_SetStrTok(out, DG_Hash), in;}
            in.unget();        return GDCC_Core_Token_SetStrTok(out, Mod), in;

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

            std::size_t hash = Core::HashString(str.data(), str.size());
            out.str = Core::AddString(str.data(), str.size(), hash);
            out.tok = Core::TOK_WSpace;
            return in;
         }

         // Quoted string/character token.
         if(c == '"' || c == '\'')
         {
            auto hold = in.holdComments();

            try
            {
               std::string str  = Core::ReadStringC(in, c);
               std::size_t hash = Core::HashString(str.data(), str.size());
               out.str = Core::AddString(str.data(), str.size(), hash);
               out.tok = c == '"' ? Core::TOK_String : Core::TOK_Charac;

               return in;
            }
            catch(Core::ParseError const &e)
            {
               std::cerr << "ERROR: " << out.pos << ": " << e.what() << '\n';
               throw EXIT_FAILURE;
            }
         }

         // Number token.
         if(std::isdigit(c) || c == '.')
         {
            in.putback(c);

            std::string str  = Core::ReadNumberC(in);
            std::size_t hash = Core::HashString(str.data(), str.size());
            out.str = Core::AddString(str.data(), str.size(), hash);
            out.tok = Core::TOK_Number;

            return in;
         }

         // Identifier token.
         if(std::isalpha(c) || c == '_')
         {
            std::string str;

            do str += static_cast<char>(c);
            while((c = in.get()) != EOF && (std::isalnum(c) || c == '_'));
            if(c != EOF) in.unget();

            std::size_t hash = Core::HashString(str.data(), str.size());
            out.str = Core::AddString(str.data(), str.size(), hash);
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
         std::size_t hash = Core::HashString(str, 1);
         out.str = Core::AddString(str, 1, hash);
         out.tok = Core::TOK_ChrSeq;
         return in;
      }
   }
}

// EOF

