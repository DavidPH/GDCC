//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Asm input stream.
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

namespace Asm
{
   //
   // operator IStream >> GDCC::Token
   //
   IStream &operator >> (IStream &in, GDCC::Token &out)
   {
      // Skip leading whitespace except for linefeed, which generates a token.
      int c = in.get();
      while(std::isspace(c) && c != '\n') c = in.get();

      out.pos = in.getOrigin();

      // Basic tokens.
      switch(c)
      {
      case ',':  GDCC_Token_SetStrTok(out, Comma);  return in;
      case '=':  GDCC_Token_SetStrTok(out, Equal);  return in;
      case '{':  GDCC_Token_SetStrTok(out, BraceO); return in;
      case '}':  GDCC_Token_SetStrTok(out, BraceC); return in;
      case '[':  GDCC_Token_SetStrTok(out, BrackO); return in;
      case ']':  GDCC_Token_SetStrTok(out, BrackC); return in;
      case '(':  GDCC_Token_SetStrTok(out, ParenO); return in;
      case ')':  GDCC_Token_SetStrTok(out, ParenC); return in;
      case '\n': GDCC_Token_SetStrTok(out, LnEnd);  return in;
      }

      // Quoted string/character token.
      if(c == '"' || c == '\'')
      {
         auto hold = in.holdComments();

         try
         {
            std::string str  = GDCC::ParseStringC(GDCC::ReadStringC(in, c));
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
      if(std::isdigit(c))
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

