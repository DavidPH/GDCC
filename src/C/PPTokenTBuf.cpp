//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Preprocessor token converter token buffer.
//
//-----------------------------------------------------------------------------

#include "PPTokenTBuf.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace C
{
   //
   // PPTokenTBuf::underflow
   //
   void PPTokenTBuf::underflow()
   {
      if(tptr() != tend()) return;

      switch((buf[0] = src.get()).tok)
      {
      case GDCC::TOK_Identi:
         if(IsKeyWrd(buf[0].str))
            buf[0].tok = GDCC::TOK_KeyWrd;
         break;

      case GDCC::TOK_Number:
              if(IsNumInt(buf[0].str)) buf[0].tok = GDCC::TOK_NumInt;
         else if(IsNumFix(buf[0].str)) buf[0].tok = GDCC::TOK_NumFix;
         else if(IsNumFlt(buf[0].str)) buf[0].tok = GDCC::TOK_NumFlt;
         else
         {
            std::cerr << "ERROR: " << buf[0].pos << ": not valid number: '"
               << buf[0].str << "'\n";
            throw EXIT_FAILURE;
         }
         break;

      default:
         break;
      }

      sett(buf, buf, buf + 1);
   }

   //
   // PPTokenTBuf::IsKeyWrd
   //
   bool PPTokenTBuf::IsKeyWrd(GDCC::String str)
   {
      switch(static_cast<GDCC::StringIndex>(str))
      {
      case GDCC::STR__Alignas:
      case GDCC::STR__Alignof:
      case GDCC::STR__Atomic:
      case GDCC::STR__Bool:
      case GDCC::STR__Complex:
      case GDCC::STR__Generic:
      case GDCC::STR__Imaginary:
      case GDCC::STR__Noreturn:
      case GDCC::STR__Static_assert:
      case GDCC::STR__Thread_local:
      case GDCC::STR_auto:
      case GDCC::STR_break:
      case GDCC::STR_case:
      case GDCC::STR_char:
      case GDCC::STR_const:
      case GDCC::STR_continue:
      case GDCC::STR_default:
      case GDCC::STR_do:
      case GDCC::STR_double:
      case GDCC::STR_else:
      case GDCC::STR_enum:
      case GDCC::STR_extern:
      case GDCC::STR_float:
      case GDCC::STR_for:
      case GDCC::STR_goto:
      case GDCC::STR_if:
      case GDCC::STR_inline:
      case GDCC::STR_int:
      case GDCC::STR_long:
      case GDCC::STR_register:
      case GDCC::STR_restrict:
      case GDCC::STR_return:
      case GDCC::STR_short:
      case GDCC::STR_signed:
      case GDCC::STR_sizeof:
      case GDCC::STR_static:
      case GDCC::STR_struct:
      case GDCC::STR_switch:
      case GDCC::STR_typedef:
      case GDCC::STR_union:
      case GDCC::STR_unsigned:
      case GDCC::STR_void:
      case GDCC::STR_volatile:
      case GDCC::STR_while:
         return true;

      default:
         return false;
      }
   }

   //
   // PPTokenTBuf::IsNumInt
   //
   bool PPTokenTBuf::IsNumInt(GDCC::String str)
   {
      char const *itr = str.begin(), *end = str.end();

      if(*itr == '0')
      {
         ++itr;

         // hexadecimal-constant
         if(*itr == 'X' || *itr == 'x')
            itr = SkipHex(itr, end);

         // octal-constant
         else
            itr = SkipOct(itr, end);
      }

      // decimal-constant
      else if('1' <= *itr && *itr <= '9')
         itr = SkipDec(++itr, end);

      else
         return false;

      // integer-suffix(opt)
      if(itr == end) return true;

      auto upr = [&itr](std::size_t i) {return std::toupper(itr[i]);};

      // unsigned-suffix long-suffix(opt)
      if(end - itr == 1 && upr(0) == 'U') return true;
      if(end - itr == 2 && upr(0) == 'U' && upr(1) == 'L') return true;

      // unsigned-suffix long-long-suffix
      if(end - itr == 3 && upr(0) == 'U' && upr(1) == 'L' && itr[2] == itr[1]) return true;

      // long-suffix unsigned-suffix(opt)
      if(end - itr == 1 && upr(0) == 'L') return true;
      if(end - itr == 2 && upr(0) == 'L' && upr(1) == 'U') return true;

      // long-long-suffix unsigned-suffix(opt)
      if(end - itr == 2 && upr(0) == 'L' && itr[1] == itr[0]) return true;
      if(end - itr == 3 && upr(0) == 'L' && itr[1] == itr[0] && upr(2) == 'U') return true;

      return false;
   }

   //
   // PPTokenTBuf::IsNumFix
   //
   bool PPTokenTBuf::IsNumFix(GDCC::String str)
   {
      char const *itr = str.begin(), *end = str.end();

      itr = SkipFlt(itr, end);

      // unsigned-suffix(opt)
      if(itr == end) return false;
      if(*itr == 'U' || *itr == 'u') ++itr;

      // fxp-suffix(opt)
      if(itr == end) return false;
      if(*itr == 'L' || *itr == 'l' || *itr == 'H' || *itr == 'h') ++itr;

      // fixed-qual
      if(itr == end) return false;
      return (*itr == 'K' || *itr == 'k' || *itr == 'R' || *itr == 'r') && ++itr == end;
   }

   //
   // PPTokenTBuf::IsNumFlt
   //
   bool PPTokenTBuf::IsNumFlt(GDCC::String str)
   {
      char const *itr = str.begin(), *end = str.end();

      itr = SkipFlt(itr, end);

      // floating-suffix(opt)
      if(itr == end) return true;
      return (*itr == 'F' || *itr == 'f' || *itr == 'L' || *itr == 'l') && ++itr == end;
   }

   //
   // PPTokenTBuf::SkipDec
   //
   char const *PPTokenTBuf::SkipDec(char const *itr, char const *end)
   {
      for(; itr != end; ++itr)
         if('0' > *itr || *itr > '9') break;

      return itr;
   }

   //
   // PPTokenTBuf::SkipFlt
   //
   char const *PPTokenTBuf::SkipFlt(char const *itr, char const *end)
   {
      if(itr[0] == '0' && (itr[1] == 'X' || itr[1] == 'x'))
      {
         itr = SkipHex(itr + 2, end);

         if(itr != end && *itr == '.')
            itr = SkipHex(itr + 1, end);

         if(itr != end && (*itr == 'P' || *itr == 'p'))
         {
            if(++itr != end && (*itr == '+' || *itr == '-')) ++itr;

            itr = SkipDec(itr, end);
         }
      }
      else
      {
         itr = SkipDec(itr, end);

         if(itr != end && *itr == '.')
            itr = SkipDec(itr + 1, end);

         if(itr != end && (*itr == 'E' || *itr == 'e'))
         {
            if(++itr != end && (*itr == '+' || *itr == '-')) ++itr;

            itr = SkipDec(itr, end);
         }
      }

      return itr;
   }

   //
   // PPTokenTBuf::SkipHex
   //
   char const *PPTokenTBuf::SkipHex(char const *itr, char const *end)
   {
      for(; itr != end; ++itr)
         if(!std::isxdigit(*itr)) break;

      return itr;
   }

   //
   // PPTokenTBuf::SkipOct
   //
   char const *PPTokenTBuf::SkipOct(char const *itr, char const *end)
   {
      for(; itr != end; ++itr)
         if('0' > *itr || *itr > '7') break;

      return itr;
   }
}

// EOF

