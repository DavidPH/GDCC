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

#include "GDCC/Parse.hpp"
#include "GDCC/Token.hpp"

#include <cctype>
#include <iostream>


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

      out.pos = in.getOrigin();

      // Identifier/number token.
      if(IsIdentifierC(c))
      {
         std::string str;

         do str += static_cast<char>(c);
         while((c = in.get()) != EOF && IsIdentifierC(c));
         if(c != EOF) in.unget();

         std::size_t hash = GDCC::HashString(str.data(), str.size());
         out.str = GDCC::AddString(str.data(), str.size(), hash);
         out.tok = GDCC::TOK_Identi;
         return in;
      }

      // Quoted string token.
      if(c == '"')
      {
         auto hold = in.holdComments();

         try
         {
            std::string str  = GDCC::ParseStringC(GDCC::ReadStringC(in, '"'));
            std::size_t hash = GDCC::HashString(str.data(), str.size());
            out.str = GDCC::AddString(str.data(), str.size(), hash);
            out.tok = GDCC::TOK_String;

            return in;
         }
         catch(GDCC::ParseError const &e)
         {
            std::cerr << "ERROR: " << out.pos << ": " << e.what() << '\n';
            throw EXIT_FAILURE;
         }
      }

      // Other character token.
      if(c != EOF)
      {
         char str[1] = {static_cast<char>(c)};

         std::size_t hash = GDCC::HashString(str, 1);
         out.str = GDCC::AddString(str, 1, hash);
         out.tok = GDCC::TOK_Charac;
         return in;
      }

      // EOF token.
      out.str = GDCC::STRNULL;
      out.tok = GDCC::TOK_EOF;
      return in;
   }
}

// EOF

