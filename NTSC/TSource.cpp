//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// NTS tokenizing.
//
//-----------------------------------------------------------------------------

#include "NTSC/TSource.hpp"

#include "Core/Exception.hpp"
#include "Core/Parse.hpp"
#include "Core/Token.hpp"
#include "Core/Warning.hpp"

#include <cctype>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::NTSC
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
         out.str = Core::ParseStringC(str += Core::ReadStringC(in, c), c);
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

namespace GDCC::NTSC
{
   //
   // TSource::v_getToken
   //
   Core::Token TSource::v_getToken()
   {
      // Skip leading whitespace and comments.
      int c = in.get();
      for(;; c = in.get())
      {
         if(std::isspace(c)) continue;
         if(c == '#') {SkipComment(in); continue;}
         break;
      }

      Core::Token tok;

      tok.pos = orig.getOrigin();

      // Basic tokens.
      switch(c)
      {
      case ',': return GDCC_Core_Token_SetStrTok(tok, Comma);
      case '=': return GDCC_Core_Token_SetStrTok(tok, Equal);
      case ';': return GDCC_Core_Token_SetStrTok(tok, Semico);
      case '{': return GDCC_Core_Token_SetStrTok(tok, BraceO);
      case '}': return GDCC_Core_Token_SetStrTok(tok, BraceC);
      case '(': return GDCC_Core_Token_SetStrTok(tok, ParenO);
      case ')': return GDCC_Core_Token_SetStrTok(tok, ParenC);

      case '.':
         // Is this actually a number?
         if(std::isdigit(in.peek())) break;

         return GDCC_Core_Token_SetStrTok(tok, Dot);
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

   //
   // TSource::SkipComment
   //
   void TSource::SkipComment(std::istream &in)
   {
      for(int c; (c = in.get()) != EOF;) {if(c == '\n') {in.unget(); break;}}
   }
}

// EOF

