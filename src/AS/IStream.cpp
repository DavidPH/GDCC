//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// AS input stream.
//
//-----------------------------------------------------------------------------

#include "AS/IStream.hpp"

#include "Core/Parse.hpp"
#include "Core/Token.hpp"

#include <cctype>
#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      //
      // operator IStream >> Core::Token
      //
      IStream &operator >> (IStream &in, Core::Token &out)
      {
         // Skip leading whitespace except for linefeed, which generates a token.
         int c = in.get();
         while(std::isspace(c) && c != '\n') c = in.get();

         out.pos = in.getOrigin();

         // Basic tokens.
         switch(c)
         {
         case ',':  GDCC_Core_Token_SetStrTok(out, Comma);  return in;
         case '/':  GDCC_Core_Token_SetStrTok(out, Div);    return in;
         case '%':  GDCC_Core_Token_SetStrTok(out, Mod);    return in;
         case '*':  GDCC_Core_Token_SetStrTok(out, Mul);    return in;
         case '?':  GDCC_Core_Token_SetStrTok(out, Query);  return in;
         case '{':  GDCC_Core_Token_SetStrTok(out, BraceO); return in;
         case '}':  GDCC_Core_Token_SetStrTok(out, BraceC); return in;
         case '[':  GDCC_Core_Token_SetStrTok(out, BrackO); return in;
         case ']':  GDCC_Core_Token_SetStrTok(out, BrackC); return in;
         case '(':  GDCC_Core_Token_SetStrTok(out, ParenO); return in;
         case ')':  GDCC_Core_Token_SetStrTok(out, ParenC); return in;
         case '\n': GDCC_Core_Token_SetStrTok(out, LnEnd);  return in;

         case '+':
            c = in.get();
            if(c == '+') return GDCC_Core_Token_SetStrTok(out, Add2), in;
            in.unget();  return GDCC_Core_Token_SetStrTok(out, Add), in;

         case '&':
            c = in.get();
            if(c == '&') return GDCC_Core_Token_SetStrTok(out, And2), in;
            in.unget();  return GDCC_Core_Token_SetStrTok(out, And), in;

         case '>':
            c = in.get();
            if(c == '=') return GDCC_Core_Token_SetStrTok(out, CmpGE), in;
            if(c == '>') return GDCC_Core_Token_SetStrTok(out, ShR), in;
            in.unget();  return GDCC_Core_Token_SetStrTok(out, CmpGT), in;

         case '<':
            c = in.get();
            if(c == '=') return GDCC_Core_Token_SetStrTok(out, CmpLE), in;
            if(c == '<') return GDCC_Core_Token_SetStrTok(out, ShL), in;
            in.unget();  return GDCC_Core_Token_SetStrTok(out, CmpLT), in;

         case '=':
            c = in.get();
            if(c == '=') return GDCC_Core_Token_SetStrTok(out, CmpEQ), in;
            in.unget();  return GDCC_Core_Token_SetStrTok(out, Equal), in;

         case '!':
            c = in.get();
            if(c == '=') return GDCC_Core_Token_SetStrTok(out, CmpNE), in;
            in.unget();  return GDCC_Core_Token_SetStrTok(out, Not), in;

         case '|':
            c = in.get();
            if(c == '|') return GDCC_Core_Token_SetStrTok(out, OrI2), in;
            in.unget();  return GDCC_Core_Token_SetStrTok(out, OrI), in;

         case '^':
            c = in.get();
            if(c == '^') return GDCC_Core_Token_SetStrTok(out, OrX2), in;
            in.unget();  return GDCC_Core_Token_SetStrTok(out, OrX), in;

         case '-':
            c = in.get();
            if(c == '-') return GDCC_Core_Token_SetStrTok(out, Sub2), in;
            in.unget();  return GDCC_Core_Token_SetStrTok(out, Sub), in;
         }

         // Quoted string/character token.
         if(c == '"' || c == '\'')
         {
            auto hold = in.holdComments();

            try
            {
               std::string str  = Core::ParseStringC(Core::ReadStringC(in, c));
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
         if(std::isdigit(c))
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

         std::cerr << "ERROR: " << out.pos << ": bad token character: '"
            << static_cast<char>(c) << "'\n";
         throw EXIT_FAILURE;
      }
   }
}

// EOF

