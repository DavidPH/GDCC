//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
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
            for(unsigned int i = 0;;) switch(c = in.get())
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
               out.put(i);
               return true;
            }

         case '0': case '1': case '2': case '3':
         case '4': case '5': case '6': case '7':
            c = escape;
            for(unsigned int n = 2, i = 0;; --n)
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
               out.put(i);
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
               throw ParseExceptStr({}, "unterminated string literal");

            if(static_cast<char>(c) == term)
               break;

            if(c == '\\')
            {
               if((c = in.get()) == EOF)
                  throw ParseExceptStr({}, "unterminated string literal");

               if(!escaper(buf, in, static_cast<char>(c)))
               {
                  char str[] = "unknown escape character: ' '";
                  std::size_t len = sizeof(str) - 1;
                  str[len - 2] = static_cast<char>(c);
                  throw ParseExceptStr({}, {str, len});
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
               throw ParseExceptStr({}, "unterminated string literal");

            buf.put(c);

            if(c == term)
               break;

            if(c == '\\')
            {
               if((c = in.get()) == EOF || c == '\n')
                  throw ParseExceptStr({}, "unterminated string literal");

               buf.put(c);
            }
         }

         return buf.str();
      }
   }
}

// EOF

