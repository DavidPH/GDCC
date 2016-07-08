//-----------------------------------------------------------------------------
//
// Copyright(C) 2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Input/output.
//
// Formatted input functions.
//
//-----------------------------------------------------------------------------

#define __GDCC__DirectObject
#define _GNU_SOURCE

#include <stdio.h>

#include <ctype.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdfix.h>
#include <stdlib.h>
#include <string.h>

#include <GDCC_libc.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// ScanBufferChar
//
#define ScanBufferChar() \
   ( \
      ++strPos, \
      --fmtArg.width, \
      *strBuf++ = strChr, \
      strChr = fgetc(stream) \
   )

//
// ScanFormat_K
//
#define ScanFormat_K() \
   ScanNumberPre(); \
   ScanNumber_fix(); \
   ScanNumberPro(); \
   \
   ScanNumberRes_uk();

//
// ScanFormat_c
//
#define ScanFormat_c() \
   switch(fmtLen) \
   { \
   case SFL_: \
      strBuf = va_arg(arg, char *); \
      strChr = fgetc(stream); \
      \
      if(fmtArg.width == -1) fmtArg.width = 1; \
      \
      while(fmtArg.width) \
      { \
         if(strChr == EOF) \
            return ret ? ret : EOF; \
         \
         ScanBufferChar(); \
      } \
      \
      ungetc(strChr, stream); \
      \
      ++ret; \
      \
      break; \
      \
   default: return ret; \
   }

//
// ScanFormat_d
//
#define ScanFormat_d() \
   ScanNumberPre(); \
   ScanNumber_int10(); \
   ScanNumberPro(); \
   \
   ScanNumberRes_i(10);

//
// ScanFormat_f
//
#if !__GDCC__NoFloat
#  define ScanFormat_f() \
      ScanNumberPre(); \
      ScanNumber_flt(); \
      ScanNumberPro(); \
      \
      ScanNumberRes_f();
#else
#  define ScanFormat_f() return ret;
#endif

//
// ScanFormat_i
//
#define ScanFormat_i() \
   ScanNumberPre(); \
   ScanNumber_int(); \
   ScanNumberPro(); \
   \
   ScanNumberRes_i(0);

//
// ScanFormat_k
//
#define ScanFormat_k() \
   ScanNumberPre(); \
   ScanNumber_fix(); \
   ScanNumberPro(); \
   \
   ScanNumberRes_k();

//
// ScanFormat_n
//
#define ScanFormat_n() \
   if(fmtArg.assign) switch(fmtLen) \
   { \
   case SFL_:   *va_arg(arg, int *)           = strPos; break; \
   case SFL_h:  *va_arg(arg, short int *)     = strPos; break; \
   case SFL_hh: *va_arg(arg, signed char *)   = strPos; break; \
   case SFL_j:  *va_arg(arg, intmax_t *)      = strPos; break; \
   case SFL_l:  *va_arg(arg, long int *)      = strPos; break; \
   case SFL_ll: *va_arg(arg, long long int *) = strPos; break; \
   case SFL_t:  *va_arg(arg, ptrdiff_t *)     = strPos; break; \
   case SFL_z:  *va_arg(arg, ssize_t *)       = strPos; break; \
   default: return ret; \
   }

//
// ScanFormat_o
//
#define ScanFormat_o() \
   ScanNumberPre(); \
   ScanNumber_int8(); \
   ScanNumberPro(); \
   \
   ScanNumberRes_u(8);

//
// ScanFormat_s
//
#define ScanFormat_s() \
   switch(fmtLen) \
   { \
   case SFL_: \
      strBuf = va_arg(arg, char *); \
      strChr = fgetc(stream); \
      \
      while(fmtArg.width && !isspace(fmtChr)) \
      { \
         if(strChr == EOF) \
            return ret ? ret : EOF; \
         \
         ScanBufferChar(); \
      } \
      \
      ungetc(strChr, stream); \
      \
      ++ret; \
      \
      break; \
      \
   default: return ret; \
   }

//
// ScanFormat_u
//
#define ScanFormat_u() \
   ScanNumberPre(); \
   ScanNumber_int10(); \
   ScanNumberPro(); \
   \
   ScanNumberRes_u(10);

//
// ScanFormat_x
//
#define ScanFormat_x() \
   ScanNumberPre(); \
   ScanNumber_int0x();\
   ScanNumber_int16(); \
   ScanNumberPro(); \
   \
   ScanNumberRes_u(16);

//
// ScanFormatCases
//
#define ScanFormatCases() \
   case 'A': ScanFormat_f(); break; \
   case 'E': ScanFormat_f(); break; \
   case 'F': ScanFormat_f(); break; \
   case 'G': ScanFormat_f(); break; \
   case 'K': ScanFormat_K(); break; \
   case 'R': return ret; /* TODO */ \
   case 'X': ScanFormat_x(); break; \
   case '[': return ret; /* TODO */ \
   case 'a': ScanFormat_f(); break; \
   case 'c': ScanFormat_c(); break; \
   case 'd': ScanFormat_d(); break; \
   case 'e': ScanFormat_f(); break; \
   case 'f': ScanFormat_f(); break; \
   case 'g': ScanFormat_f(); break; \
   case 'i': ScanFormat_i(); break; \
   case 'k': ScanFormat_k(); break; \
   case 'n': ScanFormat_n(); break; \
   case 'o': ScanFormat_o(); break; \
   case 'p': return ret; /* TODO */ \
   case 'r': return ret; /* TODO */ \
   case 's': ScanFormat_s(); break; \
   case 'u': ScanFormat_u(); break; \
   case 'x': ScanFormat_x(); break;

//
// ScanFormatDecl
//
#define ScanFormatDecl() \
   ScanFormatArg fmtArg; \
   ScanFormatLen fmtLen; \
   char          fmtChr; \
   \
   char *strBuf; \
   int   strChr; \
   int   strPos = 0; \
   \
   int ret = 0;

//
// ScanFormatLoop
//
#define ScanFormatLoop() \
   while((fmtChr = *format++)) \
   { \
      if(fmtChr == '%') \
      { \
         ScanParseSpecifier(); \
         \
         ScanFormatSwitch(); \
      } \
      else if(isspace(fmtChr)) \
      { \
         while(isspace(strChr = fgetc(stream))) \
            ++strPos; \
         \
         ungetc(strChr, stream); \
         \
         while(isspace(*format)) ++format; \
      } \
      else \
      { \
         if(fmtChr != (strChr = fgetc(stream))) \
            return ungetc(strChr, stream), ret; \
         else \
            ++strPos; \
      } \
   }

//
// ScanFormatSwitch
//
#define ScanFormatSwitch() \
   switch(fmtChr) \
   { \
      ScanFormatCases(); \
   \
   default: return ret; \
   }

//
// ScanNumber_fix
//
#define ScanNumber_fix() \
   if(strChr == '0' && (ScanBufferChar() == 'X' || strChr == 'x')) \
   { \
      ScanBufferChar(); \
      ScanNumber_fix16(); \
   } \
   else \
      ScanNumber_fix10()

//
// ScanNumber_fix10
//
#define ScanNumber_fix10() \
   do \
   { \
      ScanNumber_int10(); \
      if(strChr == '.') {ScanBufferChar(); ScanNumber_int10();} \
      ScanNumber_fixE('E', 'e'); \
   } \
   while(0)

//
// ScanNumber_fix16
//
#define ScanNumber_fix16() \
   do \
   { \
      ScanNumber_int16(); \
      if(strChr == '.') {ScanBufferChar(); ScanNumber_int16();} \
      ScanNumber_fixE('P', 'p'); \
   } \
   while(0)

//
// ScanNumber_fixE
//
#define ScanNumber_fixE(E, e) \
   if(strChr == (E) || strChr == (e)) \
   { \
      if(ScanBufferChar() == '+' || strChr == '-') ScanBufferChar(); \
      ScanNumber_int10(); \
   } \
   else \
      ((void)0)

//
// ScanNumber_flt
//
#define ScanNumber_flt() \
   if(strChr == 'I' || strChr == 'i') \
   { \
      if((ScanBufferChar() == 'N' || strChr == 'n') && \
         (ScanBufferChar() == 'F' || strChr == 'f')) \
      { \
         ScanBufferChar(); \
         \
         if(strChr == 'I' || strChr == 'i') \
         { \
            if((ScanBufferChar() == 'N' || strChr == 'n') && \
               (ScanBufferChar() == 'I' || strChr == 'i') && \
               (ScanBufferChar() == 'T' || strChr == 't') && \
               (ScanBufferChar() == 'Y' || strChr == 'y')) \
            { \
               ScanBufferChar(); \
            } \
            else \
            { \
               ungetc(strChr, stream); \
               return ret ? ret : (strChr == EOF ? EOF : 0); \
            } \
         } \
      } \
      else \
      { \
         ungetc(strChr, stream); \
         return ret ? ret : (strChr == EOF ? EOF : 0); \
      } \
   } \
   else if(strChr == 'N' || strChr == 'n') \
   { \
      if((ScanBufferChar() == 'A' || strChr == 'a') && \
         (ScanBufferChar() == 'N' || strChr == 'n')) \
      { \
         ScanBufferChar(); \
      } \
      else \
      { \
         ungetc(strChr, stream); \
         return ret ? ret : (strChr == EOF ? EOF : 0); \
      } \
   } \
   else \
      ScanNumber_fix()

//
// ScanNumber_int
//
#define ScanNumber_int() \
   if(strChr == '0') \
   { \
      if(ScanBufferChar() == 'X' || strChr == 'x') \
      { \
         ScanBufferChar(); \
         ScanNumber_int16(); \
      } \
      else \
         ScanNumber_int8(); \
   } \
   else \
      ScanNumber_int10();

//
// ScanNumber_int0x
//
#define ScanNumber_int0x() \
   if(strChr == '0' && (ScanBufferChar() == 'X' || strChr == 'x')) \
      ScanBufferChar(); \
   else \
      ((void)0)

//
// ScanNumber_int8
//
#define ScanNumber_int8() \
   while(fmtArg.width && '0' <= strChr && strChr <= '7') \
      ScanBufferChar()

//
// ScanNumber_int10
//
#define ScanNumber_int10() \
   while(fmtArg.width && '0' <= strChr && strChr <= '9') \
      ScanBufferChar()

//
// ScanNumber_int16
//
#define ScanNumber_int16() \
   while(fmtArg.width && isxdigit(strChr)) \
      ScanBufferChar()

//
// ScanNumberPre
//
#define ScanNumberPre() \
   strBuf = __GDCC__FormatBuf; \
   strChr = fgetc(stream); \
   \
   /* Skip whitespace. */ \
   while(isspace(strChr)) \
   { \
      ++strPos; \
      strChr = fgetc(stream); \
   } \
   \
   /* Safety limit. This is not strictly allowed by the standard, but */ \
   /* buffering is required for non-integer conversions and a limit of */ \
   /* 8192 characters does not seem unreasonable. I consider the */ \
   /* possibility of non-crafted data hitting this limit to be negligible. */ \
   /* Whereas the cost of a reallocating buffer seems unreasonable. */ \
   if(fmtArg.width >= __GDCC__FormatBufLen) \
      fmtArg.width = __GDCC__FormatBufLen - 1;\
   \
   /* Read optional sign. */ \
   if(fmtArg.width && (strChr == '+' || strChr == '-')) \
      ScanBufferChar();

//
// ScanNumberPro
//
#define ScanNumberPro() \
   ungetc(strChr, stream); \
   \
   /* No match. */ \
   if(strBuf == __GDCC__FormatBuf) \
      return ret ? ret : (strChr == EOF ? EOF : 0); \
   \
   *strBuf = '\0';

//
// ScanNumberRes_f
//
#define ScanNumberRes_f() \
   if(fmtArg.assign) switch(fmtLen) \
   { \
   case SFL_L:  *va_arg(arg, double *)      = strtod(__GDCC__FormatBuf, NULL);  ++ret; break; \
   case SFL_:   *va_arg(arg, float *)       = strtof(__GDCC__FormatBuf, NULL);  ++ret; break; \
   case SFL_l:  *va_arg(arg, double *)      = strtod(__GDCC__FormatBuf, NULL);  ++ret; break; \
   case SFL_ll: *va_arg(arg, long double *) = strtold(__GDCC__FormatBuf, NULL); ++ret; break; \
   default: return ret; \
   }

//
// ScanNumberRes_i
//
#define ScanNumberRes_i(base) \
   if(fmtArg.assign) switch(fmtLen) \
   { \
   case SFL_:   *va_arg(arg, int *)           = strtoi(__GDCC__FormatBuf, NULL, base);    ++ret; break; \
   case SFL_h:  *va_arg(arg, short int *)     = strtoi(__GDCC__FormatBuf, NULL, base);    ++ret; break; \
   case SFL_hh: *va_arg(arg, signed char *)   = strtoi(__GDCC__FormatBuf, NULL, base);    ++ret; break; \
   case SFL_j:  *va_arg(arg, intmax_t *)      = strtoimax(__GDCC__FormatBuf, NULL, base); ++ret; break; \
   case SFL_l:  *va_arg(arg, long int *)      = strtol(__GDCC__FormatBuf, NULL, base);    ++ret; break; \
   case SFL_ll: *va_arg(arg, long long int *) = strtoll(__GDCC__FormatBuf, NULL, base);   ++ret; break; \
   case SFL_t:  *va_arg(arg, ptrdiff_t *)     = strtoi(__GDCC__FormatBuf, NULL, base);    ++ret; break; \
   case SFL_z:  *va_arg(arg, ssize_t *)       = strtoi(__GDCC__FormatBuf, NULL, base);    ++ret; break; \
   default: return ret; \
   }

//
// ScanNumberRes_k
//
#define ScanNumberRes_k() \
   if(fmtArg.assign) switch(fmtLen) \
   { \
   case SFL_:  *va_arg(arg, accum *)       = strtofxk(__GDCC__FormatBuf, NULL);  ++ret; break; \
   case SFL_h: *va_arg(arg, short accum *) = strtofxhk(__GDCC__FormatBuf, NULL); ++ret; break; \
   case SFL_l: *va_arg(arg, long accum *)  = strtofxlk(__GDCC__FormatBuf, NULL); ++ret; break; \
   default: return ret; \
   }

//
// ScanNumberRes_u
//
#define ScanNumberRes_u(base) \
   if(fmtArg.assign) switch(fmtLen) \
   { \
   case SFL_:   *va_arg(arg, unsigned *)           = strtoui(__GDCC__FormatBuf, NULL, base);   ++ret; break; \
   case SFL_h:  *va_arg(arg, short unsigned *)     = strtoui(__GDCC__FormatBuf, NULL, base);   ++ret; break; \
   case SFL_hh: *va_arg(arg, unsigned char *)      = strtoui(__GDCC__FormatBuf, NULL, base);   ++ret; break; \
   case SFL_j:  *va_arg(arg, uintmax_t *)          = strtoumax(__GDCC__FormatBuf, NULL, base); ++ret; break; \
   case SFL_l:  *va_arg(arg, long unsigned *)      = strtoul(__GDCC__FormatBuf, NULL, base);   ++ret; break; \
   case SFL_ll: *va_arg(arg, long long unsigned *) = strtoull(__GDCC__FormatBuf, NULL, base);  ++ret; break; \
   case SFL_t:  *va_arg(arg, __uptrdiff_t *)       = strtoui(__GDCC__FormatBuf, NULL, base);   ++ret; break; \
   case SFL_z:  *va_arg(arg, size_t *)             = strtoui(__GDCC__FormatBuf, NULL, base);   ++ret; break; \
   default: return ret; \
   }

//
// ScanNumberRes_uk
//
#define ScanNumberRes_uk() \
   if(fmtArg.assign) switch(fmtLen) \
   { \
   case SFL_:  *va_arg(arg, unsigned accum *)       = strtofxuk(__GDCC__FormatBuf, NULL);  ++ret; break; \
   case SFL_h: *va_arg(arg, short unsigned accum *) = strtofxuhk(__GDCC__FormatBuf, NULL); ++ret; break; \
   case SFL_l: *va_arg(arg, long unsigned accum *)  = strtofxulk(__GDCC__FormatBuf, NULL); ++ret; break; \
   default: return ret; \
   }

//
// ScanParseLength
//
#define ScanParseLength() \
   switch(*format) \
   { \
   case 'h': fmtLen = *++format == 'h' ? ++format, SFL_hh : SFL_h; break; \
   case 'l': fmtLen = *++format == 'l' ? ++format, SFL_ll : SFL_l; break; \
   case 'L': fmtLen = SFL_L; ++format; break; \
   case 'j': fmtLen = SFL_j; ++format; break; \
   case 't': fmtLen = SFL_t; ++format; break; \
   case 'z': fmtLen = SFL_z; ++format; break; \
   default:  fmtLen = SFL_;            break; \
   }

//
// ScanParseSpecifier
//
#define ScanParseSpecifier() \
   /* Read flag, if any. */ \
   fmtArg.assign = *format == '*' ? ++format, false : true; \
   \
   /* Read width, if any. */ \
   if('0' <= (fmtChr = *format) && fmtChr <= '9') \
   { \
      fmtArg.width = 0; \
      do fmtArg.width = fmtArg.width * 10 + (fmtChr - '0'); \
      while('0' <= (fmtChr = *++format) && fmtChr <= '9'); \
   } \
   else \
      fmtArg.width = -1; \
   \
   /* Read length modifier, if any. */ \
   ScanParseLength(); \
   \
   /* Read format specifier. */ \
   fmtChr = *format++;


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// ScanFormatArg
//
typedef struct ScanFormatArg
{
   unsigned width;
   bool     assign;
} ScanFormatArg;

//
// ScanFormatLen
//
typedef enum ScanFormatLen
{
   SFL_,
   SFL_L,
   SFL_h,
   SFL_hh,
   SFL_j,
   SFL_l,
   SFL_ll,
   SFL_t,
   SFL_z,
} ScanFormatLen;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//=========================================================
// Formatted input functions.
//

//
// fscanf
//
int fscanf(FILE *restrict stream, char const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = vfscanf(stream, format, arg);
   va_end(arg);

   return ret;
}

//
// scanf
//
int scanf(char const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = vfscanf(stdin, format, arg);
   va_end(arg);

   return ret;
}

//
// sscanf
//
int sscanf(char const *restrict s, char const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = vsscanf(s, format, arg);
   va_end(arg);

   return ret;
}

//
// vfscanf
//
int vfscanf(FILE *stream, char const *format, __va_list arg)
{
   ScanFormatDecl();
   ScanFormatLoop();

   return ret;
}

//
// vscanf
//
int vscanf(char const *restrict format, __va_list arg)
{
   return vfscanf(stdin, format, arg);
}

//
// vsscanf
//
int vsscanf(char const *restrict s, char const *restrict format, __va_list arg)
{
   return vfscanf(__fmemopen_sta_r(s, strlen(s)), format, arg);
}

//=========================================================
// Implementation extensions.
//

//
// __fscanf_str
//
int __fscanf_str(FILE *restrict stream, char __str_ars const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = __vfscanf_str(stream, format, arg);
   va_end(arg);

   return ret;
}

//
// __scanf_str
//
int __scanf_str(char __str_ars const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = __vfscanf_str(stdin, format, arg);
   va_end(arg);

   return ret;
}

//
// __sscanf_str
//
int __sscanf_str(char const *restrict s, char __str_ars const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = __vsscanf_str(s, format, arg);
   va_end(arg);

   return ret;
}

//
// __strscanf
//
int __strscanf(char __str_ars const *restrict s,
   char const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = __vstrscanf(s, format, arg);
   va_end(arg);

   return ret;
}

//
// __strscanf_str
//
int __strscanf_str(char __str_ars const *restrict s,
   char __str_ars const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = __vstrscanf_str(s, format, arg);
   va_end(arg);

   return ret;
}

//
// __vfscanf_str
//
int __vfscanf_str(FILE *restrict stream, char __str_ars const *restrict format,
   __va_list arg)
{
   ScanFormatDecl();
   ScanFormatLoop();

   return ret;
}

//
// __vscanf_str
//
int __vscanf_str(char __str_ars const *restrict format, __va_list arg)
{
   return __vfscanf_str(stdin, format, arg);
}

//
// __vsscanf_str
//
int __vsscanf_str(char const *restrict s,
   char __str_ars const *restrict format, __va_list arg)
{
   return __vfscanf_str(__fmemopen_sta_r(s, strlen(s)), format, arg);
}

//
// __vstrscanf
//
int __vstrscanf(char __str_ars const *restrict s,
   char const *restrict format, __va_list arg)
{
   return vfscanf(__fmemopen_sta_r_str(s, strlen_str(s)), format, arg);
}

//
// __vstrscanf_str
//
int __vstrscanf_str(char __str_ars const *restrict s,
   char __str_ars const *restrict format, __va_list arg)
{
   return __vfscanf_str(__fmemopen_sta_r_str(s, strlen_str(s)), format, arg);
}

// EOF

