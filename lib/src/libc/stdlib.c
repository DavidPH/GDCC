//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// General utilities.
//
//-----------------------------------------------------------------------------

#define _GNU_SOURCE

#include <stdlib.h>

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>

#include <GDCC.h>

#if __GDCC_Family__ZDoom__
#include <ACS_ZDoom.h>
#endif


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define ExitMax 256


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

static mbstate_t MBState;

[[no_init]]
static void (*ExitArr[ExitMax])(void);
static void (**ExitPtr)(void) = ExitArr;

static unsigned long RandSeed = 1;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

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

   #if __GDCC_Family__ZDoom__
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
   #if __GDCC_Family__ZDoom__
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
   #if __GDCC_Family__ZDoom__
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
   return mbrtowc(NULL, s, n, &MBState);
}

//
// mbtowc
//
int mbtowc(wchar_t *restrict pwc, char const *restrict s, size_t n)
{
   return mbrtowc(pwc, s, n, &MBState);
}

//
// wctomb
//
int wctomb(char *s, wchar_t wc)
{
   return wcrtomb(s, wc, &MBState);
}

//=========================================================
// Multibyte/wide string conversion functions.
//

//
// mbstowcs
//
size_t mbstowcs(wchar_t *pwcs, char const *s, size_t n)
{
   return mbsrtowcs(pwcs, &s, n, &MBState);
}

//
// wcstombs
//
size_t wcstombs(char *s, wchar_t const *pwcs, size_t n)
{
   return wcsrtombs(s, &pwcs, n, &MBState);
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

// EOF

