//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Preprocessor token converter token buffer.
//
//-----------------------------------------------------------------------------

#include "CPP/PPTokenTBuf.hpp"

#include <cctype>
#include <iostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CPP
{
   //
   // PPTokenTBuf::underflow
   //
   void PPTokenTBuf::underflow()
   {
      if(tptr() != tend()) return;

      switch((buf[0] = src.get()).tok)
      {
      case Core::TOK_Identi:
         if(IsKeyWrd(buf[0].str))
            buf[0].tok = Core::TOK_KeyWrd;
         break;

      case Core::TOK_Number:
               if(IsNumInt(buf[0].str)) buf[0].tok = Core::TOK_NumInt;
         else if(IsNumFix(buf[0].str)) buf[0].tok = Core::TOK_NumFix;
         else if(IsNumFlt(buf[0].str)) buf[0].tok = Core::TOK_NumFlt;
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
   bool PPTokenTBuf::IsKeyWrd(Core::String str)
   {
      switch(str)
      {
      case Core::STR__Alignas:
      case Core::STR__Alignof:
      case Core::STR__Atomic:
      case Core::STR__Bool:
      case Core::STR__Complex:
      case Core::STR__Generic:
      case Core::STR__Imaginary:
      case Core::STR__Noreturn:
      case Core::STR__Static_assert:
      case Core::STR__Thread_local:
      case Core::STR_auto:
      case Core::STR_break:
      case Core::STR_case:
      case Core::STR_char:
      case Core::STR_const:
      case Core::STR_continue:
      case Core::STR_default:
      case Core::STR_do:
      case Core::STR_double:
      case Core::STR_else:
      case Core::STR_enum:
      case Core::STR_extern:
      case Core::STR_float:
      case Core::STR_for:
      case Core::STR_goto:
      case Core::STR_if:
      case Core::STR_inline:
      case Core::STR_int:
      case Core::STR_long:
      case Core::STR_register:
      case Core::STR_restrict:
      case Core::STR_return:
      case Core::STR_short:
      case Core::STR_signed:
      case Core::STR_sizeof:
      case Core::STR_static:
      case Core::STR_struct:
      case Core::STR_switch:
      case Core::STR_typedef:
      case Core::STR_union:
      case Core::STR_unsigned:
      case Core::STR_void:
      case Core::STR_volatile:
      case Core::STR_while:
         return true;

      default:
         return false;
      }
   }

   //
   // PPTokenTBuf::IsNumInt
   //
   bool PPTokenTBuf::IsNumInt(Core::String str)
   {
      char const *itr = str.begin(), *end = str.end();

      if(*itr == '0')
      {
         ++itr;

         // hexadecimal-constant
         if(*itr == 'X' || *itr == 'x')
            itr = SkipHex(itr + 1, end);

         // binary-constant
         else if(*itr == 'B' || *itr == 'b')
            itr = SkipBin(itr + 1, end);

         // octal-constant
         else
            itr = SkipOct(itr, end);
      }

      // decimal-constant
      else if('1' <= *itr && *itr <= '9')
         itr = SkipDec(itr + 1, end);

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
   bool PPTokenTBuf::IsNumFix(Core::String str)
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
   bool PPTokenTBuf::IsNumFlt(Core::String str)
   {
      char const *itr = str.begin(), *end = str.end();

      itr = SkipFlt(itr, end);

      // Alternative suffix syntax for FIXED_LITERAL pragma.
      auto upr = [&itr](std::size_t i) {return std::toupper(itr[i]);};

      if(end - itr == 2 && upr(0) == 'L' && upr(1) == 'F') return true;
      if(end - itr == 3 && upr(0) == 'L' && itr[1] == itr[0] && upr(2) == 'F') return true;

      // floating-suffix(opt)
      if(itr == end) return true;
      return (*itr == 'F' || *itr == 'f' || *itr == 'L' || *itr == 'l') && ++itr == end;
   }

   //
   // PPTokenTBuf::SkipBin
   //
   char const *PPTokenTBuf::SkipBin(char const *itr, char const *end)
   {
      for(; itr != end; ++itr)
         if('0' > *itr || *itr > '1') break;

      return itr;
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

