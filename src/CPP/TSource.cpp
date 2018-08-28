//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C tokenizing.
//
//-----------------------------------------------------------------------------

#include "CPP/TSource.hpp"

#include "Core/Exception.hpp"
#include "Core/Parse.hpp"
#include "Core/Token.hpp"
#include "Core/Warning.hpp"

#include <cctype>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::CPP
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


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CPP
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
   static Core::Token &ReadString(std::istream &in, Core::Token &out, std::string &str, int c)
   {
      // Parse character/string.
      in.unget();

      try
      {
         str    += Core::ReadStringC(in, c);
         out.str = {str.data(), str.size()};
         out.tok = c == '"' ? Core::TOK_String : Core::TOK_Charac;

         return out;
      }
      catch(Core::Exception &e)
      {
         e.setOrigin(out.pos);
         throw;
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CPP
{
   //
   // TSource::v_disableToken
   //
   bool TSource::v_disableToken(Core::TokenType type)
   {
      if(type == Core::TOK_Header)
         return tokHeader ? tokHeader = false, true : false;

      return false;
   }

   //
   // TSource::v_enableToken
   //
   bool TSource::v_enableToken(Core::TokenType type)
   {
      if(type == Core::TOK_Header)
         return !tokHeader ? tokHeader = true, false : true;

      return true;
   }

   //
   // TSource::v_getToken
   //
   Core::Token TSource::v_getToken()
   {
      Core::Token tok;

      tok.pos = orig.getOrigin();

      // Possibly check for special header token.
      if(tokHeader && TryHeader(in, tok))
         return tok;

      int c = in.get();

      // Basic tokens.
      switch(c)
      {
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
    //case ' ':  return GDCC_Core_Token_SetStrTok(tok, Space);
    //case '\t': return GDCC_Core_Token_SetStrTok(tok, Tabul);

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
         if(c == '%')    return GDCC_Core_Token_SetStrTok(tok, DG_BraceO);
         if(c == ':')    return GDCC_Core_Token_SetStrTok(tok, DG_BrackO);
         if(c == '=')    return GDCC_Core_Token_SetStrTok(tok, CmpLE);
         if(c == '<') {c = in.get();
            if(c == '=') return GDCC_Core_Token_SetStrTok(tok, ShLEq);
            in.unget();  return GDCC_Core_Token_SetStrTok(tok, ShL);}
         in.unget();     return GDCC_Core_Token_SetStrTok(tok, CmpLT);

      case ':':
         c = in.get();
         if(c == '>') return GDCC_Core_Token_SetStrTok(tok, DG_BrackC);
         in.unget();  return GDCC_Core_Token_SetStrTok(tok, Colon);

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
         if(c == '>')       return GDCC_Core_Token_SetStrTok(tok, DG_BraceC);
         if(c == '=')       return GDCC_Core_Token_SetStrTok(tok, ModEq);
         if(c == ':') {c = in.get();
            if(c == '%') {c = in.get();
               if(c == ':') return GDCC_Core_Token_SetStrTok(tok, DG_Hash2);
               in.unget();}
            in.unget();     return GDCC_Core_Token_SetStrTok(tok, DG_Hash);}
         in.unget();        return GDCC_Core_Token_SetStrTok(tok, Mod);

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
         std::string str;
         return ReadString(in, tok, str, c);
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

         do str += static_cast<char>(c);
         while((c = in.get()) != EOF && IsIdentiChar(c));

         // Character/string with encoding prefix.
         if(c == '"')
         {
            if(str == "L" || str == "U" || str == "u" || str == "u8" ||
               str == "c" || str == "s")
               return ReadString(in, tok, str, c);

            WarnUnknownEncodingPrefix(tok.pos,
               "splitting unknown string prefix: ", str);

            in.unget();
         }
         else if(c == '\'')
         {
            if(str == "L" || str == "U" || str == "u")
               return ReadString(in, tok, str, c);

            WarnUnknownEncodingPrefix(tok.pos,
               "splitting unknown character prefix: ", str);

            in.unget();
         }
         else if(c != EOF)
            in.unget();

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

   //
   // TSource::SkipCommentB
   //
   Core::Token &TSource::SkipCommentB(std::istream &in, Core::Token &out)
   {
      for(int o = EOF, c; (c = in.get()) != EOF && !(o == '*' && c == '/'); o = c) {}

      out.str = Core::STR_;
      out.tok = Core::TOK_WSpace;

      return out;
   }

   //
   // TSource::SkipCommentL
   //
   Core::Token &TSource::SkipCommentL(std::istream &in, Core::Token &out)
   {
      for(int c; (c = in.get()) != EOF;) {if(c == '\n') {in.unget(); break;}}

      out.str = Core::STR_;
      out.tok = Core::TOK_WSpace;

      return out;
   }

   //
   // TSource::TryHeader
   //
   bool TSource::TryHeader(std::istream &in, Core::Token &out)
   {
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
}

// EOF

