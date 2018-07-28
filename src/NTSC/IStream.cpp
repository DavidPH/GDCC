//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// NTSC input stream.
//
//-----------------------------------------------------------------------------

#include "NTSC/IStream.hpp"

#include "Core/Exception.hpp"
#include "Core/Parse.hpp"
#include "Core/Token.hpp"

#include <cctype>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::NTSC
{
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
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::NTSC
{
   IStream &operator >> (IStream &in, Core::Token &out)
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

         out.str = {str.data(), str.size()};
         out.tok = Core::TOK_Identi;
         return in;
      }

      // Quoted string token.
      if(c == '"')
      {
         auto hold = in.holdComments();

         in.unget();

         try
         {
            out.str = Core::ParseStringC(Core::ReadStringC(in, '"'), '"');
            out.tok = Core::TOK_String;

            return in;
         }
         catch(Core::Exception &e)
         {
            e.setOrigin(out.pos);
            throw;
         }
      }

      // Other character token.
      if(c != EOF)
      {
         char str[1] = {static_cast<char>(c)};

         out.str = {str, 1};
         out.tok = Core::TOK_Charac;
         return in;
      }

      // EOF token.
      out.str = Core::STRNULL;
      out.tok = Core::TOK_EOF;
      return in;
   }
}

// EOF

