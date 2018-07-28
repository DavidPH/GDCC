//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// String parsing utilities.
//
//-----------------------------------------------------------------------------

#include "Core/Parse.hpp"

#include "Core/Exception.hpp"
#include "Core/StringBuf.hpp"

#include <sstream>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::Core
{
   //
   // PutUTF8
   //
   static void PutUTF8(std::ostream &out, char32_t in)
   {
      if(in <= 0x7F)       {out.put(0x00 | ((in >>  0) & 0x7F)); goto put0;}
      if(in <= 0x7FF)      {out.put(0xC0 | ((in >>  6) & 0x1F)); goto put1;}
      if(in <= 0xFFFF)     {out.put(0xE0 | ((in >> 12) & 0x0F)); goto put2;}
      if(in <= 0x1FFFFF)   {out.put(0xF0 | ((in >> 18) & 0x07)); goto put3;}
      if(in <= 0x3FFFFFF)  {out.put(0xF8 | ((in >> 24) & 0x03)); goto put4;}
      if(in <= 0x7FFFFFFF) {out.put(0xFC | ((in >> 30) & 0x01)); goto put5;}

      out.put('\xFE');

            out.put(0x80 | ((in >> 30) & 0x3F));
      put5: out.put(0x80 | ((in >> 24) & 0x3F));
      put4: out.put(0x80 | ((in >> 18) & 0x3F));
      put3: out.put(0x80 | ((in >> 12) & 0x3F));
      put2: out.put(0x80 | ((in >>  6) & 0x3F));
      put1: out.put(0x80 | ((in >>  0) & 0x3F));
      put0: return;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   //
   // ParseEscapeC
   //
   bool ParseEscapeC(std::ostream &out, std::istream &in, char escape)
   {
      int c;

      switch(escape)
      {
      case '\'': out.put('\''); return true;
      case '\"': out.put('\"'); return true;
      case '\?': out.put('\?'); return true;
      case '\\': out.put('\\'); return true;

      case 'C': out.put('\x1C'); return true;

      case 'a': out.put('\a'); return true;
      case 'b': out.put('\b'); return true;
      case 'f': out.put('\f'); return true;
      case 'n': out.put('\n'); return true;
      case 'r': out.put('\r'); return true;
      case 't': out.put('\t'); return true;
      case 'v': out.put('\v'); return true;

      case 'x':
         for(char32_t i = 0;;) switch(c = in.get())
         {
         case '0': i = i * 16 + 0x0; break;
         case '1': i = i * 16 + 0x1; break;
         case '2': i = i * 16 + 0x2; break;
         case '3': i = i * 16 + 0x3; break;
         case '4': i = i * 16 + 0x4; break;
         case '5': i = i * 16 + 0x5; break;
         case '6': i = i * 16 + 0x6; break;
         case '7': i = i * 16 + 0x7; break;
         case '8': i = i * 16 + 0x8; break;
         case '9': i = i * 16 + 0x9; break;
         case 'A': i = i * 16 + 0xA; break;
         case 'B': i = i * 16 + 0xB; break;
         case 'C': i = i * 16 + 0xC; break;
         case 'D': i = i * 16 + 0xD; break;
         case 'E': i = i * 16 + 0xE; break;
         case 'F': i = i * 16 + 0xF; break;
         case 'a': i = i * 16 + 0xa; break;
         case 'b': i = i * 16 + 0xb; break;
         case 'c': i = i * 16 + 0xc; break;
         case 'd': i = i * 16 + 0xd; break;
         case 'e': i = i * 16 + 0xe; break;
         case 'f': i = i * 16 + 0xf; break;

         default:
            in.putback(static_cast<char>(c));
            PutUTF8(out, i);
            return true;
         }

      case '0': case '1': case '2': case '3':
      case '4': case '5': case '6': case '7':
         c = escape;
         for(char32_t n = 2, i = 0;; --n)
         {
            switch(c)
            {
            case '0': i = i * 8 + 00; break;
            case '1': i = i * 8 + 01; break;
            case '2': i = i * 8 + 02; break;
            case '3': i = i * 8 + 03; break;
            case '4': i = i * 8 + 04; break;
            case '5': i = i * 8 + 05; break;
            case '6': i = i * 8 + 06; break;
            case '7': i = i * 8 + 07; break;

            default:
               in.putback(static_cast<char>(c));
               goto octal_done;
            }

            if(n) {c = in.get(); continue;}

         octal_done:
            PutUTF8(out, i);
            return true;
         }

      default:
         return false;
      }
   }

   //
   // ParseStringC
   //
   String ParseStringC(std::istream &in, char term,
      bool (*escaper)(std::ostream &, std::istream &, char))
   {
      std::ostringstream buf;

      in.get();

      for(;;)
      {
         int c = in.get();

         if(c == EOF)
            Error({}, "unterminated string literal");

         if(static_cast<char>(c) == term)
            break;

         if(c == '\\')
         {
            if((c = in.get()) == EOF)
               Error({}, "unterminated string literal");

            if(!escaper(buf, in, static_cast<char>(c)))
            {
               char str[] = "unknown escape character: ' '";
               std::size_t len = sizeof(str) - 1;
               str[len - 2] = static_cast<char>(c);
               Error({}, {str, len});
            }
         }
         else
            buf.put(c);
      }

      auto res = buf.str();
      return {res.data(), res.size()};
   }

   //
   // ParseStringC
   //
   String ParseStringC(std::string const &inBuf, char term,
      bool (*escaper)(std::ostream &, std::istream &, char))
   {
      std::istringstream in{inBuf};
      return ParseStringC(in, term, escaper);
   }

   //
   // ParseStringC
   //
   String ParseStringC(String inStr, char term,
      bool (*escaper)(std::ostream &, std::istream &, char))
   {
      StringStream in{inStr};
      return ParseStringC(in, term, escaper);
   }

   //
   // ParseStringC
   //
   String ParseStringC(String inStr, std::size_t offset, char term,
      bool (*escaper)(std::ostream &, std::istream &, char))
   {
      StringStream in{inStr.data() + offset, inStr.size() - offset};
      return ParseStringC(in, term, escaper);
   }

   //
   // ReadStringC
   //
   std::string ReadStringC(std::istream &in, char term)
   {
      std::ostringstream buf;

      buf.put(in.get());

      for(;;)
      {
         int c = in.get();

         if(c == EOF || c == '\n')
            Error({}, "unterminated string literal");

         buf.put(c);

         if(c == term)
            break;

         if(c == '\\')
         {
            if((c = in.get()) == EOF || c == '\n')
               Error({}, "unterminated string literal");

            buf.put(c);
         }
      }

      return buf.str();
   }
}

// EOF

