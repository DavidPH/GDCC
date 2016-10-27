//-----------------------------------------------------------------------------
//
// Copyright(C) 2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// Extended multibyte and wide character utilities.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__wchar_h__
#define __GDCC_Header__C__wchar_h__

#include <bits/types.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// NULL
//
#ifndef NULL
#define NULL 0
#endif

//
// WCHAR_MIN
//
#ifndef WCHAR_MIN
#define WCHAR_MIN 0
#endif

//
// WCHAR_MAX
//
#ifndef WCHAR_MAX
#define WCHAR_MAX 0xFFFFFFFF
#endif

//
// WEOF
//
#ifndef WEOF
#define WEOF (-1)
#endif


//----------------------------------------------------------------------------|
// Types                                                                      |
//

struct tm;

//
// wchar_t
//
#ifndef __GDCC_Have__wchar_t__
#define __GDCC_Have__wchar_t__
typedef __wchar_t wchar_t;
#endif

//
// size_t
//
#ifndef __GDCC_Have__size_t__
#define __GDCC_Have__size_t__
typedef __size_t size_t;
#endif

//
// mbstate_t
//
#ifndef __GDCC_Have__mbstate_t__
#define __GDCC_Have__mbstate_t__
typedef struct __mbstate_t {int;} mbstate_t;
#endif

//
// wint_t
//
#ifndef __GDCC_Have__wint_t__
#define __GDCC_Have__wint_t__
typedef __wint_t wint_t;
#endif

struct tm;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// Formatted wide character input/output functions.
//

int fwprintf(struct __FILE *restrict _stream, wchar_t const *restrict _format, ...);
int fwscanf(struct __FILE *restrict _stream, wchar_t const *restrict _format, ...);
int swprintf(wchar_t *restrict _s, size_t _n, wchar_t const *restrict _format, ...);
int swscanf(wchar_t const *restrict _s, wchar_t const *restrict _format, ...);
int vfwprintf(struct __FILE *restrict _stream, wchar_t const *restrict _format, __va_list _arg);
int vfwscanf(struct __FILE *restrict _stream, wchar_t const *restrict _format, __va_list _arg);
int vswprintf(wchar_t *restrict _s, size_t _n, wchar_t const *restrict _format, __va_list _arg);
int vswscanf(wchar_t const *restrict _s, wchar_t const *restrict _format, __va_list _arg);
int vwprintf(wchar_t const *restrict _format, __va_list _arg);
int vwscanf(wchar_t const *restrict _format, __va_list _arg);
int wprintf(wchar_t const *restrict _format, ...);
int wscanf(wchar_t const *restrict _format, ...);

//
// Wide character input/output functions.
//

wint_t fgetwc(struct __FILE *_stream);
wchar_t *fgetws(wchar_t *restrict _s, int _n, struct __FILE *restrict _stream);
wint_t fputwc(wchar_t _c, struct __FILE *_stream);
int fputws(wchar_t const *restrict _s, struct __FILE *restrict _stream);
int fwide(struct __FILE *_stream, int _mode);
wint_t getwc(struct __FILE *_stream);
wint_t getwchar(void);
wint_t putwc(wchar_t _c, struct __FILE *_stream);
wint_t putwchar(wchar_t _c);
wint_t ungetwc(wint_t _c, struct __FILE *_stream);

//
// General wide string utilities.
//

//
// Wide string numeric conversion functions.
//

double wcstod(wchar_t const *restrict _nptr, wchar_t **restrict _endptr);
float wcstof(wchar_t const *restrict _nptr, wchar_t **restrict _endptr);
long double wcstold(wchar_t const *restrict _nptr, wchar_t **restrict _endptr);

int wcstoi(wchar_t const *restrict _nptr, wchar_t **restrict _endptr, int _base);
long wcstol(wchar_t const *restrict _nptr, wchar_t **restrict _endptr, int _base);
long long wcstoll(wchar_t const *restrict _nptr, wchar_t **restrict _endptr, int _base);
unsigned int wcstoui(wchar_t const * restrict _nptr, wchar_t **restrict _endptr, int _base);
unsigned long wcstoul(wchar_t const * restrict _nptr, wchar_t **restrict _endptr, int _base);
unsigned long long wcstoull(wchar_t const *restrict _nptr, wchar_t **restrict _endptr, int _base);

//
// Wide string copying functions.
//

wchar_t *wcscpy(wchar_t *restrict _s1, wchar_t const *restrict _s2);
wchar_t *wcsncpy(wchar_t *restrict _s1, wchar_t const *restrict _s2, size_t _n);

wchar_t *wmemcpy(wchar_t *restrict _s1, wchar_t const *restrict _s2, size_t _n);
wchar_t *wmemmove(wchar_t *_s1, wchar_t const *_s2, size_t _n);

//
// Wide string concatenation functions.
//

wchar_t *wcscat(wchar_t *restrict _s1, wchar_t const *restrict _s2);
wchar_t *wcsncat(wchar_t *restrict _s1, wchar_t const *restrict _s2, size_t _n);

//
// Wide string comparison functions.
//

int wcscmp(wchar_t const *_s1, wchar_t const *_s2);
int wcscoll(wchar_t const *_s1, wchar_t const *_s2);
int wcsncmp(wchar_t const *_s1, wchar_t const *_s2, size_t _n);
int wcsncmp(wchar_t const *_s1, wchar_t const *_s2, size_t _n);
int wmemcmp(wchar_t const *_s1, wchar_t const *_s2, size_t _n);

//
// Wide string search functions.
//

wchar_t *wcschr(wchar_t const *_s, wchar_t _c);
size_t wcscspn(wchar_t const *_s1, wchar_t const *_s2);
wchar_t *wcspbrk(wchar_t const *_s1, wchar_t const *_s2);
wchar_t *wcsrchr(wchar_t const *_s, wchar_t _c);
size_t wcsspn(wchar_t const *_s1, wchar_t const *_s2);
wchar_t *wcsstr(wchar_t const *_s1, wchar_t const *_s2);
wchar_t *wcstok(wchar_t *restrict _s1, wchar_t const *restrict _s2, wchar_t **restrict _ptr);
wchar_t *wmemchr(wchar_t const *_s, wchar_t _c, size_t _n);

//
// Miscellaneous functions.
//

size_t wcslen(wchar_t const *_s);
wchar_t *wmemset(wchar_t *_s, wchar_t _c, size_t _n);

//
// Wide character time conversion functions.
//

size_t wcsftime(wchar_t *restrict _s, size_t _maxsize,
   wchar_t const *restrict _format, struct tm const *restrict _timeptr);

//
// Extended multibyte/wide character conversion utilities.
//

//
// Single-byte/wide character conversion functions.
//

wint_t btowc(int _c);
int wctob(wint_t _c);

//
// Conversion state functions.
//

int mbsinit(const mbstate_t *_ps);

//
// Restartable multibyte/wide character conversion functions.
//

size_t mbrlen(char const *restrict _s, size_t _n, mbstate_t *restrict _ps);
size_t mbrtowc(wchar_t *restrict _pwc, char const *restrict _s, size_t _n,
   mbstate_t *restrict _ps);
size_t wcrtomb(char *restrict _s, wchar_t _wc, mbstate_t *restrict _ps);

//
// Restartable multibyte/wide string conversion functions.
//

size_t mbsrtowcs(wchar_t *restrict _dst, char const **restrict _src, size_t _len,
   mbstate_t *restrict _ps);
size_t wcsrtombs(char *restrict _dst, wchar_t const **restrict _src, size_t _len,
   mbstate_t *restrict _ps);

#endif//__GDCC_Header__C__wchar_h__

