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
// String handling.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__string_h__
#define __GDCC_Header__C__string_h__

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


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// size_t
//
#ifndef __GDCC_Have__size_t__
#define __GDCC_Have__size_t__
typedef __size_t size_t;
#endif


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

#ifdef __cplusplus
extern "C" {
#endif

//
// Copying functions.
//

void *memcpy(void *restrict _s1, void const *restrict _s2, size_t _n);
void *memmove(void *_s1, void const *_s2, size_t _n);
char *strcpy(char *restrict _s1, char const *restrict _s2);
char *strncpy(char *restrict _s1, char const *restrict _s2, size_t _n);

//
// Concatenation functions.
//

char *strcat(char *restrict _s1, char const *restrict _s2);
char *strncat(char *restrict _s1, char const *restrict _s2, size_t _n);

//
// Comparison functions.
//

int memcmp(void const *_s1, void const *_s2, size_t n_);
int strcmp(char const *_s1, char const *_s2);
int strcoll(char const *_s1, char const *_s2);
int strncmp(char const *_s1, char const *_s2, size_t _n);
size_t strxfrm(char *restrict _s1, char const *restrict _s2, size_t _n);

//
// Search functions
//

void *memchr(void const *_s, int _c, size_t _n);
char *strchr(char const *_s, int _c);
size_t strcspn(char const *_s1, char const *_s2);
char *strpbrk(char const *_s1, char const *_s2);
char *strrchr(char const *_s, int _c);
size_t strspn(char const *_s1, char const *_s2);
char *strstr(char const *_s1, char const *_s2);
char *strtok(char *restrict _s1, char const *restrict _s2);

//
// Miscellaneous functions.
//

void *memset(void *_s, int _c, size_t _n);
char *strerror(int _errnum);
size_t strlen(char const *_s);

//
// GNU extensions.
//

#if defined(_GNU_SOURCE)
char *strcasestr(char const *_s1, char const *_s2);
#endif

//
// Implementation extensions.
//

char *strcasechr(char const *_s, int _c);
char __str_ars *strcasechr_str(char __str_ars const *_s, int _c);
char __str_ars *strcasestr_str(char __str_ars const *_s1, char __str_ars const *_s2);
char __str_ars *strchr_str(char __str_ars const *_s, int _c);
size_t strlen_str(char __str_ars const *_s);
char __str_ars *strstr_str(char __str_ars const *_s1, char __str_ars const *_s2);

#ifdef __cplusplus
};
#endif

#endif//__GDCC_Header__C__string_h__

