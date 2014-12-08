//-----------------------------------------------------------------------------
//
// Copyright(C) 2014 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// General utilities.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__stdlib_h__
#define __GDCC_Header__C__stdlib_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// EXIT_FAILURE
//
#define EXIT_FAILURE 1

//
// EXIT_SUCCESS
//
#define EXIT_SUCCESS 0

//
// MB_CUR_MAX
//
#define MB_CUR_MAX ((size_t)1)

//
// NULL
//
#ifndef NULL
#define NULL 0
#endif

//
// RAND_MAX
//
#define RAND_MAX 0x7FFFFFFF


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// div_t
//
typedef __div_t div_t;

//
// ldiv_t
//
typedef long __div_t ldiv_t;

//
// lldiv_t
//
typedef long long __div_t lldiv_t;

//
// size_t
//
#ifndef __GDCC_Have__size_t__
#define __GDCC_Have__size_t__
typedef unsigned size_t;
#endif

//
// wchar_t
//
#ifndef __GDCC_Have__wchar_t__
#define __GDCC_Have__wchar_t__
typedef unsigned wchar_t;
#endif


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

#ifdef __cplusplus
extern "C" {
#endif

//
// Numeric conversion functions.
//

double atof(char const *nptr);

int atoi(char const *nptr);
long atol(char const *nptr);
long long atoll(char const *nptr);

double strtod(char const *restrict nptr, char **restrict endptr);
float strtof(char const *restrict nptr, char **restrict endptr);
long double strtold(char const *restrict nptr, char **restrict endptr);

int strtoi(char const *restrict nptr, char **restrict endptr, int base);
long strtol(char const *restrict nptr, char **restrict endptr, int base);
long long strtoll(char const *restrict nptr, char **restrict endptr, int base);
unsigned int strtoui(char const *restrict nptr, char **restrict endptr, int base);
unsigned long strtoul(char const *restrict nptr, char **restrict endptr, int base);
unsigned long long strtoull(char const *restrict nptr, char **restrict endptr, int base);

//
// Pseudo-random sequence generation functions.
//

int rand(void);
void srand(unsigned int seed);

//
// Memory management functions.
//

void *calloc(size_t nmemb, size_t size);
void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);

//
// Communication with the environment.
//

void abort(void);
int atexit(void (*func)(void));
void exit(int status);
void _Exit(int status);
char *getenv(char const *name);
int system(char const *string);

//
// Searching and sorting utilities.
//

void *bsearch(void const *key, void const *base, size_t nmemb, size_t size,
   int (*compar)(void const *, void const *));

void qsort(void *base, size_t nmemb, size_t size,
   int (*compar)(void const *, void const *));

//
// Integer arithmetic functions.
//

int abs(int j);
long int labs(long int j);
long long int llabs(long long int j);

div_t div(int numer, int denom);
ldiv_t ldiv(long int numer, long int denom);
lldiv_t lldiv(long long int numer, long long int denom);

//
// Multibyte/wide character conversion functions.
//

int mblen(char const *s, size_t n);
int mbtowc(wchar_t *restrict pwc, char const *restrict s, size_t n);
int wctomb(char *s, wchar_t wc);

//
// Multibyte/wide string conversion functions.
//

size_t mbstowcs(wchar_t *restrict pwcs, char const *restrict s, size_t n);
size_t wcstombs(char *restrict s, wchar_t const *restrict pwcs, size_t n);

#ifdef __cplusplus
};
#endif

#endif//__GDCC_Header__C__stdlib_h__

