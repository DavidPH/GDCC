//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// MageDefs input stream.
//
//-----------------------------------------------------------------------------

#include "IStream.hpp"

#include "GDCC/Token.hpp"

#include <cctype>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// IsIdentifierC
//
static bool IsIdentifierC(int c)
{
   return std::isalnum(c) || c == '_' ||
     (c & 0x80) || // UTF-8.
      c == '.' || // Number component.
      c == '/' || // Path separator.
      c == ':';   // Common name component.
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace MageDefs
{
   IStream &operator >> (IStream &in, GDCC::Token &out)
   {
      // Skip leading whitespace.
      int c = in.get();
      while(std::isspace(c)) c = in.get();

      // Identifier/number token.
      if(IsIdentifierC(c))
      {
         std::string str;

         do str += static_cast<char>(c);
         while((c = in.get()) != EOF && IsIdentifierC(c));
         if(c != EOF) in.unget();

         std::size_t hash = GDCC::HashString(str.data(), str.size());
         out.str = GDCC::AddString(str.data(), str.size(), hash);
         out.tok = GDCC::TOK_Identifier;
         return in;
      }

      // Quoted string token.
      if(c == '"')
      {
         std::string str;

         in.disableComments();
         while((c = in.get()) != EOF && c != '"')
            str += static_cast<char>(c);
         in.enableComments();

         std::size_t hash = GDCC::HashString(str.data(), str.size());
         out.str = GDCC::AddString(str.data(), str.size(), hash);
         out.tok = GDCC::TOK_String;
         return in;
      }

      // Other character token.
      if(c != EOF)
      {
         char str[1] = {static_cast<char>(c)};

         std::size_t hash = GDCC::HashString(str, 1);
         out.str = GDCC::AddString(str, 1, hash);
         out.tok = GDCC::TOK_Character;
         return in;
      }

      // EOF token.
      out.str = GDCC::STRNULL;
      out.tok = GDCC::TOK_EOF;
      return in;
   }
}

// EOF

