//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2016 David Hill
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

#include <bits/types.h>


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
#define MB_CUR_MAX ((size_t)6)

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
typedef __size_t size_t;
#endif

//
// wchar_t
//
#ifndef __GDCC_Have__wchar_t__
#define __GDCC_Have__wchar_t__
typedef __wchar_t wchar_t;
#endif


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

#ifdef __cplusplus
extern "C" {
#endif

//
// Numeric conversion functions.
//

double atof(char const *_nptr);

int atoi(char const *_nptr);
long atol(char const *_nptr);
long long atoll(char const *_nptr);

double strtod(char const *restrict _nptr, char **restrict _endptr);
float strtof(char const *restrict _nptr, char **restrict _endptr);
long double strtold(char const *restrict _nptr, char **restrict _endptr);

long strtol(char const *restrict _nptr, char **restrict _endptr, int _base);
long long strtoll(char const *restrict _nptr, char **restrict _endptr, int _base);
unsigned long strtoul(char const *restrict _nptr, char **restrict _endptr, int _base);
unsigned long long strtoull(char const *restrict _nptr, char **restrict _endptr, int _base);

//
// Pseudo-random sequence generation functions.
//

int rand(void);
void srand(unsigned int _seed);

//
// Memory management functions.
//

void *calloc(size_t _nmemb, size_t _size);
void free(void *_ptr);
void *malloc(size_t _size);
void *realloc(void *_ptr, size_t _size);

//
// Communication with the environment.
//

void abort(void);
int atexit(void (*_func)(void));
void exit(int _status);
void _Exit(int _status);
char *getenv(char const *_name);
int system(char const *_string);

//
// Searching and sorting utilities.
//

void *bsearch(void const *_key, void const *_base, size_t _nmemb, size_t _size,
   int (*_compar)(void const *, void const *));

void qsort(void *_base, size_t _nmemb, size_t _size,
   int (*_compar)(void const *, void const *));

//
// Integer arithmetic functions.
//

int abs(int _j);
long int labs(long int _j);
long long int llabs(long long int _j);

div_t div(int _numer, int _denom);
ldiv_t ldiv(long int _numer, long int _denom);
lldiv_t lldiv(long long int _numer, long long int _denom);

//
// Multibyte/wide character conversion functions.
//

int mblen(char const *_s, size_t _n);
int mbtowc(wchar_t *restrict _pwc, char const *restrict _s, size_t _n);
int wctomb(char *_s, wchar_t _wc);

//
// Multibyte/wide string conversion functions.
//

size_t mbstowcs(wchar_t *restrict _pwcs, char const *restrict _s, size_t _n);
size_t wcstombs(char *restrict _s, wchar_t const *restrict _pwcs, size_t _n);

//
// Implementation extensions.
//

double strtod_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr);
float strtof_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr);
long double strtold_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr);

int strtoi(char const *restrict _nptr, char **restrict _endptr, int _base);
int strtoi_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr, int _base);
long strtol_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr, int _base);
long long strtoll_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr, int _base);
unsigned int strtoui(char const *restrict _nptr, char **restrict _endptr, int _base);
unsigned int strtoui_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr, int _base);
unsigned long strtoul_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr, int _base);
unsigned long long strtoull_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr, int _base);

#ifdef __cplusplus
};
#endif

#endif//__GDCC_Header__C__stdlib_h__

