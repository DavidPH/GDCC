//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C input stream.
//
//-----------------------------------------------------------------------------

#include "CPP/IStream.hpp"

#include "Core/Exception.hpp"
#include "Core/Parse.hpp"
#include "Core/Token.hpp"
#include "Core/Warning.hpp"

#include <cctype>
#include <iostream>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // --warn-unknown-encoding-prefix
      //
      static Core::Warning WarnUnknownEncodingPrefix
         {&Core::WarnExtra, "--warn-unknown-encoding-prefix"};
      static Core::WarnOpt WarnUnknownEncodingPrefixOpt
      {
         &Core::GetWarnOptList(), Option::Base::Info()
            .setName("warn-unknown-encoding-prefix")
            .setGroup("warnings")
            .setDescS("Warns about unknown encoding prefixes.")
            .setDescL("Warns when an unknown encoding prefix is encountered "
               "and taken as a separate identifier.\n\n"
               "Enabled by --warn-extra."),

         &WarnUnknownEncodingPrefix
      };
   }
}


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // IsIdentiChar
      //
      static bool IsIdentiChar(int c)
      {
         return std::isalnum(c) || c == '_' || c >= 0x80;
      }

      //
      // ReadString
      //
      static IStream &ReadString(IStream &in, Core::Token &out,
         std::string &str, int c)
      {
         // Parse character/string.
         auto hold = in.holdComments();

         in.unget();

         try
         {
            str    += Core::ReadStringC(in, c);
            out.str = {str.data(), str.size()};
            out.tok = c == '"' ? Core::TOK_String : Core::TOK_Charac;

            return in;
         }
         catch(Core::ParseException &e)
         {
            e.setOrigin(out.pos);
            throw;
         }
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // IStreamHeader::GetHeader
      //
      bool IStreamHeader::GetHeader(std::istream &in, Core::Token &out)
      {
         while(std::isspace(in.peek())) in.get();

         int c = in.get();

         // System header.
         if(c == '<')
         {
            std::string str;

            while((c = in.get()) != EOF && c != '>')
               str += static_cast<char>(c);

            out.str = {str.data(), str.size()};
            out.tok = Core::TOK_Header;

            return true;
         }

         // String header.
         if(c == '"')
         {
            std::string str;

            while((c = in.get()) != EOF && c != '"')
               str += static_cast<char>(c);

            out.str = {str.data(), str.size()};
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
         if(in.needHeader)
         {
            in.needHeader = false;
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

            out.str = {str.data(), str.size()};
            out.tok = Core::TOK_WSpace;
            return in;
         }

         // Quoted string/character token.
         if(c == '"' || c == '\'')
         {
            std::string str;
            return ReadString(in, out, str, c);
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

            do str += static_cast<char>(c);
            while((c = in.get()) != EOF && IsIdentiChar(c));

            // Character/string with encoding prefix.
            if(c == '"')
            {
               if(str == "L" || str == "U" || str == "u" || str == "u8" ||
                  str == "c" || str == "s")
                  return ReadString(in, out, str, c);

               WarnUnknownEncodingPrefix(out.pos,
                  "splitting unknown string prefix: ", str);

               in.unget();
            }
            else if(c == '\'')
            {
               if(str == "L" || str == "U" || str == "u")
                  return ReadString(in, out, str, c);

               WarnUnknownEncodingPrefix(out.pos,
                  "splitting unknown character prefix: ", str);

               in.unget();
            }
            else if(c != EOF)
               in.unget();

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
   }
}

// EOF

