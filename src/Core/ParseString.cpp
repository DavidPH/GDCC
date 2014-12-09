//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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
      char ParseEscapeC(std::istream &in)
      {
         switch(int c = in.get())
         {
         case EOF:
            throw ParseExceptStr({}, "unterminated string literal");

         case '\'': return '\'';
         case '\"': return '\"';
         case '\?': return '\?';
         case '\\': return '\\';

         case 'C': return '\x1C';

         case 'a': return '\a';
         case 'b': return '\b';
         case 'f': return '\f';
         case 'n': return '\n';
         case 'r': return '\r';
         case 't': return '\t';
         case 'v': return '\v';

         case 'x':
            for(unsigned int i = 0;;) switch(c = in.get())
            {
            case EOF:
               throw ParseExceptStr({}, "unterminated string literal");

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
               return static_cast<char>(i);
            }

         case '0': case '1': case '2': case '3':
         case '4': case '5': case '6': case '7':
            for(unsigned int n = 2, i = 0;; --n)
            {
               switch(c)
               {
               case EOF:
                  throw ParseExceptStr({}, "unterminated string literal");

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
               return static_cast<char>(i);
            }

         default:
            char str[] = "unknown escape character: ' '";
            std::size_t len = sizeof(str) - 1;
            str[len - 2] = static_cast<char>(c);
            throw ParseExceptStr({}, {str, len});
         }
      }

      //
      // ParseStringC
      //
      std::string ParseStringC(std::istream &in)
      {
         std::string buf;
         int term = in.get();

         for(int c; (c = in.get()) != EOF && c != term;)
         {
            if(c == '\\')
               buf += ParseEscapeC(in);
            else
               buf += static_cast<char>(c);
         }

         return buf;
      }

      //
      // ParseStringC
      //
      String ParseStringC(std::string const &inBuf)
      {
         std::istringstream in{inBuf};
         auto data = ParseStringC(in);
         return {data.data(), data.size()};
      }

      //
      // ParseStringC
      //
      String ParseStringC(String inStr)
      {
         StringStream in{inStr};
         auto data = ParseStringC(in);
         return {data.data(), data.size()};
      }

      //
      // ParseStringC
      //
      String ParseStringC(String inStr, std::size_t offset)
      {
         StringStream in{inStr.data() + offset, inStr.size() - offset};
         auto data = ParseStringC(in);
         return {data.data(), data.size()};
      }

      //
      // ReadEscapeC
      //
      void ReadEscapeC(std::istream &in, std::string &out)
      {
         switch(int c = in.get())
         {
         case EOF:
            throw ParseExceptStr({}, "unterminated string literal");

         case '\'': case '\"': case '\?': case '\\':

         case 'C':

         case 'a': case 'b': case 'f': case 'n':
         case 'r': case 't': case 'v':
            out += static_cast<char>(c);
            return;

         case 'x':
            out += 'x';
            for(;;) switch(c = in.get())
            {
            case EOF:
               throw ParseExceptStr({}, "unterminated string literal");

            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
               out += static_cast<char>(c);
               break;

            default:
               in.putback(static_cast<char>(c));
               return;
            }

         case '0': case '1': case '2': case '3':
         case '4': case '5': case '6': case '7':
            for(unsigned int n = 2;; --n)
            {
               switch(c)
               {
               case EOF:
                  throw ParseExceptStr({}, "unterminated string literal");

               case '0': case '1': case '2': case '3':
               case '4': case '5': case '6': case '7':
                  out += static_cast<char>(c);
                  break;

               default:
                  in.putback(static_cast<char>(c));
                  goto octal_done;
               }

               if(n) {c = in.get(); continue;}

            octal_done:
               return;
            }

         default:
            char str[] = "unknown escape character: ' '";
            std::size_t len = sizeof(str) - 1;
            str[len - 2] = static_cast<char>(c);
            throw ParseExceptStr({}, {str, len});
         }
      }

      //
      // ReadStringC
      //
      std::string ReadStringC(std::istream &in, char term)
      {
         std::string buf{term};

         for(int c = in.get();; c = in.get())
         {
            if(c == EOF || c == '\n')
               throw ParseExceptStr({}, "unterminated string literal");

            buf += static_cast<char>(c);

            if(c == term)
               break;

            if(c == '\\')
               ReadEscapeC(in, buf);
         }

         return buf;
      }
   }
}

// EOF

