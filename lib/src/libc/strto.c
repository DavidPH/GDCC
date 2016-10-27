//-----------------------------------------------------------------------------
//
// Copyright(C) 2015-2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Numeric conversion functions.
//
//-----------------------------------------------------------------------------

#define __GDCC__DirectObject

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdfix.h>
#include <stdlib.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// StrToF_Body
//
#define StrToF_Body(ctype, type, F) \
   StrToF_Pre(ctype, F); \
   StrToK_Scan(ctype); \
   StrToF_Loop(ctype, type); \
   StrToF_Pro(ctype, F)

//
// StrToF_Loop
//
#define StrToF_Loop(ctype, type) \
   /* Read number. */ \
   type val = 0; \
   \
   StrToK_Loop_E(); \
   StrToF_Loop_I(ctype, type); \
   StrToK_Loop_F(ctype, type)

//
// StrToF_Loop_I
//
#define StrToF_Loop_I(ctype, type) \
   /* Integer part. */ \
   if(base == 10) \
   { \
      if(signE) \
      { \
         /* Integer portion. */ \
         int idig = iend - iptr; \
         if(idig > valE) \
         { \
            int exp = idig - valE; \
            for(ctype const *ptr = iptr; exp; ++ptr, --exp) \
               val = val * 10 + CharToInt(*ptr); \
         } \
      } \
      else \
      { \
         int exp = valE; \
         \
         /* Integer portion. */ \
         for(ctype const *ptr = iptr; ptr != iend; ++ptr) \
            val = val * 10 + CharToInt(*ptr); \
         \
         /* Fractional portion. */ \
         for(ctype const *ptr = fptr; ptr != fend && exp; ++ptr, --exp) \
            val = val * 10 + CharToInt(*ptr); \
         \
         /* Apply any missing digits implied by exponent. */ \
         for(; exp; --exp) \
            val = val * 10; \
      } \
   } \
   else \
   { \
      int digE = valE >> 2; \
      int shft = valE & 3; \
      \
      ctype const *ptr; \
      \
      if(signE) \
      { \
         /* Integer portion. */ \
         int idig = iend - iptr; \
         if(idig >= digE) \
         { \
            int exp = idig - digE; \
            for(ptr = iptr; exp; ++ptr, --exp) \
               val = (val << 4) + ((type)CharToInt(*ptr) >> shft); \
         } \
      } \
      else \
      { \
         int exp = digE; \
         \
         /* Integer portion. */ \
         for(ptr = iptr; ptr != iend; ++ptr) \
            val = (val << 4) + ((type)CharToInt(*ptr) << shft); \
         \
         /* Fractional portion. */ \
         for(ptr = fptr; ptr != fend && exp; ++ptr, --exp) \
            val = (val << 4) + ((type)CharToInt(*ptr) << shft); \
         \
         /* Apply any missing digits implied by exponent. */ \
         for(; exp; --exp) \
            val = val << 4; \
      } \
   }

//
// StrToF_Pre
//
#define StrToF_Pre(ctype, F) \
   /* Skip leading whitespace. */ \
   while(isspace(*nptr)) ++nptr; \
   \
   /* Read sign. */ \
   bool sign; \
        if(*nptr == '-') sign = true,  ++nptr; \
   else if(*nptr == '+') sign = false, ++nptr; \
   else                  sign = false; \
   \
   /* Check for INF/INFINITY */ \
   if((nptr[0] == 'I' || nptr[0] == 'i') && \
      (nptr[1] == 'N' || nptr[1] == 'n') && \
      (nptr[2] == 'F' || nptr[2] == 'f')) \
   { \
      nptr += 3; \
      \
      if((nptr[0] == 'I' || nptr[0] == 'i') && \
         (nptr[1] == 'N' || nptr[1] == 'n') && \
         (nptr[1] == 'I' || nptr[1] == 'i') && \
         (nptr[1] == 'T' || nptr[1] == 't') && \
         (nptr[2] == 'Y' || nptr[2] == 'y')) \
      { \
         nptr += 5; \
      } \
      \
      /* Set end pointer. */ \
      if(endptr) *endptr = (ctype *)nptr; \
      \
      return sign ? -INFINITY : INFINITY; \
   } \
   \
   /* Check for NAN/NAN(...) */ \
   if((nptr[0] == 'N' || nptr[0] == 'n') && \
      (nptr[1] == 'A' || nptr[1] == 'a') && \
      (nptr[2] == 'N' || nptr[2] == 'n')) \
   { \
      nptr += 3; \
      \
      /* TODO: NAN(...) */ \
      \
      /* Set end pointer. */ \
      if(endptr) *endptr = (ctype *)nptr; \
      \
      return sign ? -NAN : NAN; \
   } \
   \
   /* Read prefix. */ \
   int base; \
   if(nptr[0] == '0' && (nptr[1] == 'X' || nptr[1] == 'x')) \
      base = 16, nptr += 2; \
   else \
      base = 10;

//
// StrToF_Pro
//
#define StrToF_Pro(ctype, F) \
   /* Set end pointer. */ \
   if(endptr) *endptr = (ctype *)nptr; \
   \
   /* If overflowed, set errno and return MIN/MAX. */ \
   if(isinf(val)) \
   { \
      errno = ERANGE; \
      return sign ? -HUGE_VAL##F : HUGE_VAL##F; \
   } \
   \
   return sign ? -val : val;

//
// StrToI_Body
//
#define StrToI_Body(ctype, type, limit, sign) \
   StrToI_Pre(); \
   StrToI_Loop(type, limit); \
   StrToI_Pro##sign(ctype, limit)

//
// StrToI_Loop
//
#define StrToI_Loop(type, limit) \
   /* Read number. */ \
   type val = 0, mul = base, max = limit##_MAX / mul; \
   int  gap = limit##_MAX % mul; \
   bool ovr = 0; \
   \
   for(int dig; (dig = CharToInt(*nptr)) < base; ++nptr) \
   { \
      /* Check for overflow. */ \
      if(val > max || (val == max && dig > gap)) ovr = 1; \
      \
      val = val * mul + dig; \
   }

//
// StrToI_Pre
//
#define StrToI_Pre() \
   /* Skip leading whitespace. */ \
   while(isspace(*nptr)) ++nptr; \
   \
   /* Read sign. */ \
   bool sign; \
        if(*nptr == '-') sign = true,  ++nptr; \
   else if(*nptr == '+') sign = false, ++nptr; \
   else                  sign = false; \
   \
   /* Read prefix. */ \
   if(!base) \
   { \
      if(*nptr == '0') \
      { \
         if(*++nptr == 'X' || *nptr == 'x') \
            base = 16, ++nptr; \
         else \
            base = 8; \
      } \
      else \
         base = 10; \
   } \
   else if(base == 16) \
   { \
      if(nptr[0] == '0' && (nptr[1] == 'X' || nptr[1] == 'x')) \
         nptr += 2; \
   }

//
// StrToI_ProS
//
#define StrToI_ProS(ctype, limit) \
   /* Set end pointer. */ \
   if(endptr) *endptr = (ctype *)nptr; \
   \
   /* If overflowed, set errno and return MIN/MAX. */ \
   if(ovr) \
   { \
      errno = ERANGE; \
      return sign ? limit##_MIN : limit##_MAX; \
   } \
   \
   return sign ? -val : val;

//
// StrToI_ProU
//
#define StrToI_ProU(ctype, limit) \
   /* Set end pointer. */ \
   if(endptr) *endptr = (ctype *)nptr; \
   \
   /* If overflowed, set errno and return MAX. */ \
   if(ovr) \
   { \
      errno = ERANGE; \
      return limit##_MAX; \
   } \
   \
   return sign ? -val : val;

//
// StrToK_Body
//
#define StrToK_Body(ctype, type, limit, sign) \
   StrToK_Pre(); \
   StrToK_Scan(ctype); \
   StrToK_Loop(ctype, type, limit); \
   StrToI_Pro##sign(ctype, limit)

//
// StrToK_Loop
//
#define StrToK_Loop(ctype, type, limit) \
   /* Read number. */ \
   type val = 0, max = roundfx(limit##_MAX / base, 0); \
   bool ovr = false; \
   \
   StrToK_Loop_E(); \
   StrToK_Loop_I(ctype, type, limit); \
   StrToK_Loop_F(ctype, type)

//
// StrToK_Loop_E
//
#define StrToK_Loop_E() \
   /* Exponent part. */ \
   bool signE; \
   int  valE = 0; \
   if(base == 10 ? *nptr == 'E' || *nptr == 'e' : *nptr == 'P' || *nptr == 'p') \
   { \
      ++nptr; \
      \
           if(*nptr == '-') signE = true,  ++nptr; \
      else if(*nptr == '+') signE = false, ++nptr; \
      else                  signE = false; \
      \
      int  maxE = INT_MAX / 10; \
      int  gapE = INT_MAX % 10; \
      bool ovrE = false; \
      \
      /* Convert exponent digits. */ \
      for(int dig; (dig = CharToInt(*nptr)) < 10; ++nptr) \
      { \
         /* Check for overflow. */ \
         if(valE > maxE || (valE == maxE && dig > gapE)) ovrE = true; \
         valE = valE * 10 + dig; \
      } \
      \
      if(ovrE) valE = INT_MAX; \
   } \
   else \
      signE = false;

//
// StrToK_Loop_F
//
#define StrToK_Loop_F(ctype, type) \
   /* Fractional part. */ \
   if(base == 10) \
   { \
      type valF = 0; \
      \
      if(signE) \
      { \
         int exp = valE; \
         \
         /* Fractional portion. */ \
         for(ctype const *ptr = fend; ptr != fptr;) \
            valF = (valF + CharToInt(*--ptr)) / 10; \
         \
         /* Integer portion. */ \
         for(ctype const *ptr = iend; ptr != iptr && exp; --exp) \
            valF = (valF + CharToInt(*--ptr)) / 10; \
         \
         /* Apply any missing digits implied by exponent. */ \
         for(; exp; --exp) \
            valF = valF / 10; \
      } \
      else \
      { \
         /* Fractional portion. */ \
         int fdig = fend - fptr; \
         if(fdig > valE) \
         { \
            int exp = fdig - valE; \
            \
            for(ctype const *ptr = fend; exp; --exp) \
               valF = (valF + CharToInt(*--ptr)) / 10; \
         } \
      } \
      \
      val += valF; \
   } \
   else \
   { \
      type valF = 0; \
      int  digE = valE >> 2; \
      int  shft = valE & 3; \
      \
      ctype const *ptr; \
      \
      if(signE) \
      { \
         int exp = digE; \
         \
         /* Fractional portion. */ \
         for(ptr = fend; ptr != fptr;) \
            valF = (valF + ((type)CharToInt(*--ptr) >> shft)) >> 4; \
         \
         /* Integer portion. */ \
         for(ptr = iend; ptr != iptr && exp; --exp) \
            valF = (valF + ((type)CharToInt(*--ptr) >> shft)) >> 4; \
         \
         /* Apply any missing digits implied by exponent. */ \
         for(; exp; --exp) \
            valF = valF >> 4; \
      } \
      else \
      { \
         /* Fractional portion. */ \
         int fdig = fend - fptr; \
         if(fdig >= digE) \
         { \
            int exp = fdig - digE; \
            \
            for(ptr = fend; exp; --exp) \
               valF = (valF + ((type)CharToInt(*--ptr) << shft)) >> 4; \
         } \
      } \
      \
      val += valF; \
   }

//
// StrToK_Loop_I
//
#define StrToK_Loop_I(ctype, type, limit) \
   /* Integer part. */ \
   if(base == 10) \
   { \
      int gap = limit##_MAX % base; \
      \
      if(signE) \
      { \
         /* Integer portion. */ \
         int idig = iend - iptr; \
         if(idig > valE) \
         { \
            int exp = idig - valE; \
            for(ctype const *ptr = iptr; exp; ++ptr, --exp) \
            { \
               int dig = CharToInt(*ptr); \
               \
               /* Check for overflow. */ \
               if(val > max || (val == max && dig > gap)) ovr = true; \
               \
               val = val * 10 + dig; \
            } \
         } \
      } \
      else \
      { \
         int exp = valE; \
         \
         /* Integer portion. */ \
         for(ctype const *ptr = iptr; ptr != iend; ++ptr) \
         { \
            int dig = CharToInt(*ptr); \
            \
            /* Check for overflow. */ \
            if(val > max || (val == max && dig > gap)) ovr = true; \
            \
            val = val * 10 + dig; \
         } \
         \
         /* Fractional portion. */ \
         for(ctype const *ptr = fptr; ptr != fend && exp; ++ptr, --exp) \
         { \
            int dig = CharToInt(*ptr); \
            \
            /* Check for overflow. */ \
            if(val > max || (val == max && dig > gap)) ovr = true; \
            \
            val = val * 10 + dig; \
         } \
         \
         /* Apply any missing digits implied by exponent. */ \
         for(; exp; --exp) \
         { \
            /* Check for overflow. */ \
            if(val > max) ovr = true; \
            \
            val = val * 10; \
         } \
      } \
   } \
   else \
   { \
      int digE = valE >> 2; \
      int shft = valE & 3; \
      \
      ctype const *ptr; \
      \
      if(signE) \
      { \
         /* Integer portion. */ \
         int idig = iend - iptr; \
         if(idig >= digE) \
         { \
            int exp = idig - digE; \
            for(ptr = iptr; exp; ++ptr, --exp) \
            { \
               /* Check for overflow. */ \
               if(val > max) ovr = true; \
               \
               val = (val << 4) + ((type)CharToInt(*ptr) >> shft); \
            } \
         } \
      } \
      else \
      { \
         int exp = digE; \
         \
         /* Integer portion. */ \
         for(ptr = iptr; ptr != iend; ++ptr) \
         { \
            /* Check for overflow. */ \
            if(val > max) ovr = true; \
            \
            val = (val << 4) + ((type)CharToInt(*ptr) << shft); \
         } \
         \
         /* Fractional portion. */ \
         for(ptr = fptr; ptr != fend && exp; ++ptr, --exp) \
         { \
            /* Check for overflow. */ \
            if(val > max) ovr = true; \
            \
            val = (val << 4) + ((type)CharToInt(*ptr) << shft); \
         } \
         \
         /* Apply any missing digits implied by exponent. */ \
         for(; exp; --exp) \
         { \
            /* Check for overflow. */ \
            if(val > max) ovr = true; \
            \
            val = val << 4; \
         } \
      } \
   }

//
// StrToK_Pre
//
#define StrToK_Pre() \
   /* Skip leading whitespace. */ \
   while(isspace(*nptr)) ++nptr; \
   \
   /* Read sign. */ \
   bool sign; \
        if(*nptr == '-') sign = true,  ++nptr; \
   else if(*nptr == '+') sign = false, ++nptr; \
   else                  sign = false; \
   \
   /* Read prefix. */ \
   int base; \
   if(nptr[0] == '0' && (nptr[1] == 'X' || nptr[1] == 'x')) \
      base = 16, nptr += 2; \
   else \
      base = 10;

//
// StrToK_Scan
//
#define StrToK_Scan(ctype) \
   /* Scan integer portion. */ \
   \
   /* Skip leading zeroes and save pointer to start of integer portion. */ \
   while(*nptr == '0') ++nptr; \
   ctype const *iptr = nptr; \
   \
   /* Scan to end of integer portion. */ \
   for(int dig; (dig = CharToInt(*nptr)) < base; ++nptr) {} \
   \
   /* Save pointer to end of integer portion. */ \
   ctype const *iend = nptr; \
   \
   /* Scan fractional portion, if any. */ \
   ctype const *fptr, *fend; \
   if(*nptr == '.') \
   { \
      /* Save pointer to start of fractional portion. */ \
      fptr = ++nptr; \
      \
      /* Scan to end of fractional portion. */ \
      for(int dig; (dig = CharToInt(*nptr)) < base; ++nptr) {} \
      \
      /* Save pointer to end of fractional portion. */ \
      fend = nptr; \
   } \
   else \
   { \
      /* Store pointers to non-digit for fractional part. */ \
      fptr = fend = nptr; \
   }


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// CharToInt
//
[[call("StkCall")]]
static int CharToInt(int c)
{
   switch(c)
   {
   case '0':           return  0;
   case '1':           return  1;
   case '2':           return  2;
   case '3':           return  3;
   case '4':           return  4;
   case '5':           return  5;
   case '6':           return  6;
   case '7':           return  7;
   case '8':           return  8;
   case '9':           return  9;
   case 'A': case 'a': return 10;
   case 'B': case 'b': return 11;
   case 'C': case 'c': return 12;
   case 'D': case 'd': return 13;
   case 'E': case 'e': return 14;
   case 'F': case 'f': return 15;
   case 'G': case 'g': return 16;
   case 'H': case 'h': return 17;
   case 'I': case 'i': return 18;
   case 'J': case 'j': return 19;
   case 'K': case 'k': return 20;
   case 'L': case 'l': return 21;
   case 'M': case 'm': return 22;
   case 'N': case 'n': return 23;
   case 'O': case 'o': return 24;
   case 'P': case 'p': return 25;
   case 'Q': case 'q': return 26;
   case 'R': case 'r': return 27;
   case 'S': case 's': return 28;
   case 'T': case 't': return 29;
   case 'U': case 'u': return 30;
   case 'V': case 'v': return 31;
   case 'W': case 'w': return 32;
   case 'X': case 'x': return 33;
   case 'Y': case 'y': return 34;
   case 'Z': case 'z': return 35;
   default: return INT_MAX;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//=========================================================
// Functions for greatest-width integer types. (inttypes.h)
//

//
// strtoimax
//
intmax_t strtoimax(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToI_Body(char, intmax_t, INTMAX, S);
}

//
// strtoumax
//
uintmax_t strtoumax(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToI_Body(char, uintmax_t, UINTMAX, U);
}

//
// wcstoimax
//
intmax_t wcstoimax(wchar_t const *restrict nptr, wchar_t **restrict endptr, int base)
{
   StrToI_Body(wchar_t, intmax_t, INTMAX, S);
}

//
// wcstoumax
//
uintmax_t wcstoumax(wchar_t const *restrict nptr, wchar_t **restrict endptr, int base)
{
   StrToI_Body(wchar_t, uintmax_t, UINTMAX, U);
}

//=========================================================
// Implementation extensions. (inttypes.h)
//

//
// strtoimax_str
//
intmax_t strtoimax_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr, int base)
{
   StrToI_Body(char __str_ars, intmax_t, INTMAX, S);
}

//
// strtoumax_str
//
uintmax_t strtoumax_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr, int base)
{
   StrToI_Body(char __str_ars, uintmax_t, UINTMAX, U);
}

//=========================================================
// Numeric conversion functions. (stdfix.h)
//

short fract strtofxhr(char const *restrict nptr, char **restrict endptr);
fract strtofxr(char const *restrict nptr, char **restrict endptr);
long fract strtofxlr(char const *restrict nptr, char **restrict endptr);

//
// strtofxhk
//
short accum strtofxhk(char const *restrict nptr, char **restrict endptr)
{
   StrToK_Body(char, short accum, SACCUM, S);
}

//
// strtofxk
//
accum strtofxk(char const *restrict nptr, char **restrict endptr)
{
   StrToK_Body(char, accum, ACCUM, S);
}

//
// strtofxlk
//
long accum strtofxlk(char const *restrict nptr, char **restrict endptr)
{
   StrToK_Body(char, long accum, LACCUM, S);
}

unsigned short fract strtofxuhr(char const *restrict nptr, char **restrict endptr);
unsigned fract strtofxur(char const *restrict nptr, char **restrict endptr);
unsigned long fract strtofxulr(char const *restrict nptr, char **restrict endptr);

//
// strtofxuhk
//
unsigned short accum strtofxuhk(char const *restrict nptr, char **restrict endptr)
{
   StrToK_Body(char, unsigned short accum, USACCUM, U);
}

//
// strtofxuk
//
unsigned accum strtofxuk(char const *restrict nptr, char **restrict endptr)
{
   StrToK_Body(char, unsigned accum, UACCUM, U);
}

//
// strtofxulk
//
unsigned long accum strtofxulk(char const *restrict nptr, char **restrict endptr)
{
   StrToK_Body(char, unsigned long accum, ULACCUM, U);
}

//=========================================================
// Implementation extensions. (stdfix.h)
//

short fract strtofxhr_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr);
fract strtofxr_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr);
long fract strtofxlr_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr);

//
// strtofxhk_str
//
short accum strtofxhk_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr)
{
   StrToK_Body(char __str_ars, short accum, SACCUM, S);
}

//
// strtofxk_str
//
accum strtofxk_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr)
{
   StrToK_Body(char __str_ars, accum, ACCUM, S);
}

//
// strtofxlk_str
//
long accum strtofxlk_str(char const __str_ars *restrict nptr, char __str_ars **restrict endptr)
{
   StrToK_Body(char __str_ars, long accum, LACCUM, S);
}

unsigned short fract strtofxuhr_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr);
unsigned fract strtofxur_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr);
unsigned long fract strtofxulr_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr);

//
// strtofxuhk_str
//
unsigned short accum strtofxuhk_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr)
{
   StrToK_Body(char __str_ars, unsigned short accum, USACCUM, U);
}

//
// strtofxuk_str
//
unsigned accum strtofxuk_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr)
{
   StrToK_Body(char __str_ars, unsigned accum, UACCUM, U);
}

//
// strtofxulk_str
//
unsigned long accum strtofxulk_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr)
{
   StrToK_Body(char __str_ars, unsigned long accum, ULACCUM, U);
}

//=========================================================
// Numeric conversion functions. (stdlib.h)
//

#if !__GDCC__NoFloat
//
// atof
//
double atof(char const *nptr)
{
   return strtod(nptr, NULL);
}
#endif

//
// atoi
//
int atoi(char const *nptr)
{
   return strtoi(nptr, NULL, 10);
}

//
// atol
//
long atol(char const *nptr)
{
   return strtol(nptr, NULL, 10);
}

//
// atoll
//
long long atoll(char const *nptr)
{
   return strtoll(nptr, NULL, 10);
}

#if !__GDCC__NoFloat
//
// strtod
//
double strtod(char const *restrict nptr, char **restrict endptr)
{
   StrToF_Body(char, double, );
}

//
// strtof
//
float strtof(char const *restrict nptr, char **restrict endptr)
{
   StrToF_Body(char, float, F);
}

//
// strtold
//
long double strtold(char const *restrict nptr, char **restrict endptr)
{
   StrToF_Body(char, long double, L);
}
#endif

//
// strtol
//
long strtol(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToI_Body(char, long, LONG, S);
}

//
// strtoll
//
long long strtoll(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToI_Body(char, long long, LLONG, S);
}

//
// strtoul
//
unsigned long strtoul(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToI_Body(char, unsigned long, ULONG, U);
}

//
// strtoull
//
unsigned long long strtoull(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToI_Body(char, unsigned long long, ULLONG, U);
}

//=========================================================
// Implementation extensions. (stdlib.h)
//

#if !__GDCC__NoFloat
//
// strtod_str
//
double strtod_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr)
{
   StrToF_Body(char __str_ars, double, );
}

//
// strtof_str
//
float strtof_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr)
{
   StrToF_Body(char __str_ars, float, F);
}

//
// strtold_str
//
long double strtold_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr)
{
   StrToF_Body(char __str_ars, long double, );
}
#endif

//
// strtoi
//
int strtoi(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToI_Body(char, int, INT, S);
}

//
// strtoi_str
//
int strtoi_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr, int base)
{
   StrToI_Body(char __str_ars, int, INT, S);
}

//
// strtol_str
//
long strtol_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr, int base)
{
   StrToI_Body(char __str_ars, long, LONG, S);
}

//
// strtoll_str
//
long long strtoll_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr, int base)
{
   StrToI_Body(char __str_ars, long long, LLONG, S);
}

//
// strtoui
//
unsigned int strtoui(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToI_Body(char, unsigned int, UINT, U);
}

//
// strtoui_str
//
unsigned int strtoui_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr, int base)
{
   StrToI_Body(char __str_ars, unsigned int, UINT, U);
}

//
// strtoul_str
//
unsigned long strtoul_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr, int base)
{
   StrToI_Body(char __str_ars, unsigned long, ULONG, U);
}

//
// strtoull_str
//
unsigned long long strtoull_str(char __str_ars const *restrict nptr, char __str_ars **restrict endptr, int base)
{
   StrToI_Body(char __str_ars, unsigned long long, ULLONG, U);
}

// EOF

