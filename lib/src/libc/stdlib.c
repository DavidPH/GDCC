//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2015 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// General utilities.
//
//-----------------------------------------------------------------------------

#include <stdlib.h>

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#include <GDCC.h>

#if __GDCC_Target__ZDoom__
#include <ACS_ZDoom.h>
#endif


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define ExitMax 256

//
// StrToInt_Body
//
#define StrToInt_Body(ctype, type, limit, sign) \
   StrToInt_Pre(); \
   StrToInt_Loop(type, limit); \
   StrToInt_Pro##sign(ctype, limit)

//
// StrToInt_Loop
//
#define StrToInt_Loop(type, limit) \
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
// StrToInt_Pre
//
#define StrToInt_Pre() \
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
// StrToInt_ProS
//
#define StrToInt_ProS(ctype, limit) \
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
// StrToInt_ProU
//
#define StrToInt_ProU(ctype, limit) \
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


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

[[no_init]]
static void (*ExitArr[ExitMax])(void);
static void (**ExitPtr)(void) = ExitArr;

static unsigned long RandSeed = 1;


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
// Global Functions                                                           |
//

//=========================================================
// Numeric conversion functions.
//

//
// atof
//
double atof(char const *nptr)
{
   return strtod(nptr, NULL);
}

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

//
// strtod
//
double strtod(char const *restrict nptr, char **restrict endptr)
{
   // TODO

   if(endptr) *endptr = (char *)nptr;

   return 0;
}

//
// strtof
//
float strtof(char const *restrict nptr, char **restrict endptr)
{
   // TODO

   if(endptr) *endptr = (char *)nptr;

   return 0;
}

//
// strtold
//
long double strtold(char const *restrict nptr, char **restrict endptr)
{
   // TODO

   if(endptr) *endptr = (char *)nptr;

   return 0;
}

//
// strtoi
//
int strtoi(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToInt_Body(char, int, INT, S);
}

//
// strtol
//
long strtol(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToInt_Body(char, long, LONG, S);
}

//
// strtoll
//
long long strtoll(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToInt_Body(char, long long, LLONG, S);
}

//
// strtoui
//
unsigned int strtoui(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToInt_Body(char, unsigned int, UINT, U);
}

//
// strtoul
//
unsigned long strtoul(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToInt_Body(char, unsigned long, ULONG, U);
}

//
// strtoull
//
unsigned long long strtoull(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToInt_Body(char, unsigned long long, ULLONG, U);
}

//=========================================================
// Pseudo-random sequence generation functions.
//

//
// rand
//
int rand(void)
{
   RandSeed ^= (RandSeed << 13);
   RandSeed ^= (RandSeed >>  7);
   RandSeed ^= (RandSeed << 17);
   return (unsigned int)RandSeed & RAND_MAX;
}

//
// srand
//
void srand(unsigned int seed)
{
   RandSeed = seed;
}

//=========================================================
// Memory management functions.
//

//
// calloc
//
void *calloc(size_t nmemb, size_t size)
{
   char __sta *ptr = __GDCC__alloc(0, size *= nmemb);

   if(!ptr) return 0;

   for(char __sta *itr = ptr; size--;)
      *itr++ = 0;

   return ptr;
}

//
// free
//
void free(void *ptr)
{
   __GDCC__alloc((void __sta *)ptr, 0);
}

//
// malloc
//
void *malloc(size_t size)
{
   return __GDCC__alloc(0, size);
}

//
// realloc
//
void *realloc(void *ptr, size_t size)
{
   return __GDCC__alloc((void __sta *)ptr, size);
}

//=========================================================
// Communication with the environment.
//

//
// abort
//
void abort(void)
{
   fprintf(stderr, "Aborted.\n");

   #if __GDCC_Target__ZDoom__
   [[address(1), call("AsmFunc")]]
   extern void ACS_ScriptRet(void);

   ACS_Exit_Normal(0);
   ACS_ScriptRet();
   #endif
}

//
// atexit
//
int atexit(void (*func)(void))
{
   if(ExitPtr == ExitArr + ExitMax) return 1;

   *ExitPtr++ = func;

   return 0;
}

//
// exit
//
void exit(int status)
{
   while(ExitPtr != ExitArr)
      (*--ExitPtr)();

   _Exit(status);
}

//
// _Exit
//
void _Exit(int status)
{
   #if __GDCC_Target__ZDoom__
   [[address(1), call("AsmFunc")]]
   extern void ACS_ScriptRet(void);

   ACS_Exit_Normal(0);
   ACS_ScriptRet();
   #endif
}

//
// getenv
//
char *getenv(char const *name)
{
   #if __GDCC_Target__ZDoom__
   [[no_init]]
   static char buf[1024];

   union {__str s; int i;} s;

   ACS_BeginPrint();
   ACS_PrintGlobalCharArray((int)name, __GDCC__Sta);
   s.s = ACS_GetCVarString(ACS_EndStrParam());

   if(!s.i) return NULL;

   if(!ACS_StrCpyToGlobalCharRange((int)buf, __GDCC__Sta, 0, 1023, s.s, 0))
      buf[1023] = '\0';

   return buf;
   #else
   return NULL;
   #endif
}

//
// system
//
int system(char const *string)
{
   return 0;
}

//=========================================================
// Integer arithmetic functions.
//

//
// abs
//
int abs(int j)
{
   return j < 0 ? -j : j;
}

//
// labs
//
long int labs(long int j)
{
   return j < 0 ? -j : j;
}

//
// llabs
//
long long int llabs(long long int j)
{
   return j < 0 ? -j : j;
}

//
// div
//
div_t div(int numer, int denom)
{
   return __div(numer, denom);
}

//
// ldiv
//
ldiv_t ldiv(long int numer, long int denom)
{
   return __div(numer, denom);
}

//
// lldiv
//
lldiv_t lldiv(long long int numer, long long int denom)
{
   return __div(numer, denom);
}

//=========================================================
// Multibyte/wide character conversion functions.
//

//
// mblen
//
int mblen(char const *s, size_t n)
{
   if(!s) return 0;

   if(!n) return -1;

   return 1;
}

//
// mbtowc
//
int mbtowc(wchar_t *restrict pwc, char const *restrict s, size_t n)
{
   if(!s) return 0;

   if(!n) return -1;

   if(pwc) *pwc = *s;

   return !!*s;
}

//
// wctomb
//
int wctomb(char *s, wchar_t wc)
{
   if(!s) return 0;

   *s = wc;

   return 1;
}

//=========================================================
// Multibyte/wide string conversion functions.
//

//
// mbstowcs
//
size_t mbstowcs(wchar_t *restrict pwcs, char const *restrict s, size_t n)
{
   size_t i = 0;

   while(*s)
   {
      if(i++ == n) return n;

      *pwcs++ = *s++;
   }

   *pwcs = 0;

   return i;
}

//
// wcstombs
//
size_t wcstombs(char *restrict s, wchar_t const *restrict pwcs, size_t n)
{
   size_t i = 0;

   while(*pwcs)
   {
      if(i++ == n) return n;

      *s++ = *pwcs++;
   }

   *s = 0;

   return i;
}

//=========================================================
// Functions for greatest-width integer types.
//

//
// imaxabs
//
intmax_t imaxabs(intmax_t j)
{
   return j < 0 ? -j : j;
}

//
// imaxdiv
//
imaxdiv_t imaxdiv(intmax_t numer, intmax_t denom)
{
   return __div(numer, denom);
}

//
// strtoimax
//
intmax_t strtoimax(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToInt_Body(char, intmax_t, INTMAX, S);
}

//
// strtoumax
//
uintmax_t strtoumax(char const *restrict nptr, char **restrict endptr, int base)
{
   StrToInt_Body(char, uintmax_t, UINTMAX, U);
}

//
// wcstoimax
//
intmax_t wcstoimax(wchar_t const *restrict nptr, wchar_t **restrict endptr, int base)
{
   StrToInt_Body(wchar_t, intmax_t, INTMAX, S);
}

//
// wcstoumax
//
uintmax_t wcstoumax(wchar_t const *restrict nptr, wchar_t **restrict endptr, int base)
{
   StrToInt_Body(wchar_t, uintmax_t, UINTMAX, U);
}

// EOF

