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
// String handling.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__string_h__
#define __GDCC_Header__C__string_h__


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
typedef unsigned size_t;
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

void *memcpy(void *restrict s1, void const *restrict s2, size_t n);
void *memmove(void *s1, void const *s2, size_t n);
char *strcpy(char *restrict s1, char const *restrict s2);
char *strncpy(char *restrict s1, char const *restrict s2, size_t n);

//
// Concatenation functions.
//

char *strcat(char *restrict s1, char const *restrict s2);
char *strncat(char *restrict s1, char const *restrict s2, size_t n);

//
// Comparison functions.
//

int memcmp(void const *s1, void const *s2, size_t n);
int strcmp(char const *s1, char const *s2);
int strcoll(char const *s1, char const *s2);
int strncmp(char const *s1, char const *s2, size_t n);
size_t strxfrm(char *restrict s1, char const *restrict s2, size_t n);

//
// Search functions
//

void *memchr(void const *s, int c, size_t n);
char *strchr(char const *s, int c);
size_t strcspn(char const *s1, char const *s2);
char *strpbrk(char const *s1, char const *s2);
char *strrchr(char const *s, int c);
size_t strspn(char const *s1, char const *s2);
char *strstr(char const *s1, char const *s2);
char *strtok(char *restrict s1, char const *restrict s2);

//
// Miscellaneous functions.
//

void *memset(void *s, int c, size_t n);
char *strerror(int errnum);
size_t strlen(char const *s);

#ifdef __cplusplus
};
#endif

#endif//__GDCC_Header__C__string_h__

