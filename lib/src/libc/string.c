//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2017 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// String handling.
//
//-----------------------------------------------------------------------------

#define _GNU_SOURCE

#include <string.h>

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

#if __GDCC_Family__ZDACS__
#include <ACS_ZDoom.h>
#endif


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// StrStrImpl
//
#define StrStrImpl(type, conv, name_case, name_str) \
   size_t wordLen = strlen##name_str(word); \
   \
   /* Special cases. */ \
   if(wordLen == 0) return (type *)text; \
   if(wordLen == 1) return str##name_case##chr##name_str(text, *word); \
   \
   /* Allocate partial match table. */ \
   size_t wordTabAuto[128], *wordTab; \
   if(wordLen <= 128) \
      wordTab = wordTabAuto; \
   else \
      wordTab = malloc(sizeof(size_t) * wordLen); \
   \
   /* Memory exhaustion, fall back to a no alloc algorithm. */ \
   if(!wordTab) \
   { \
      while((text = str##name_case##chr##name_str(text, *word))) \
      { \
         for(type const *textItr = text + 1, *wordItr = word + 1;; ++textItr, ++wordItr) \
         { \
            if(!*wordItr) return (type *)text; \
            if(!*textItr) return NULL; \
            \
            if(conv(*textItr) != conv(*wordItr)) break; \
         } \
         \
         ++text; \
      } \
      \
      return NULL; \
   } \
   \
   /* Index 0 has special handling, so just leave wordTab[0] alone entirely. */ \
   wordTab[1] = 0; \
   \
   /* Compute partial match table. */ \
   for(size_t pos = 2, cnd = 0; pos != wordLen;) \
   { \
      if(conv(word[cnd]) == conv(word[pos - 1])) \
         wordTab[pos++] = ++cnd; \
      else if(cnd) \
         cnd = wordTab[cnd]; \
      else \
         wordTab[pos++] = 0; \
   } \
   \
   /* Perform search. */ \
   type const *textItr = text; \
   type const *textEnd = text + strlen##name_str(text); \
   size_t      wordIdx = 0; \
   \
   while(textItr + wordIdx != textEnd) \
   { \
      /* Character match? */ \
      if(conv(word[wordIdx]) == conv(textItr[wordIdx])) \
      { \
         /* Word match? */ \
         if(++wordIdx == wordLen) \
         { \
            if(wordTab != wordTabAuto) free(wordTab); \
            return (type *)textItr; \
         } \
      } \
      else \
      { \
         /* Not at start of word match? */ \
         if(wordIdx) \
         { \
            textItr += wordIdx - wordTab[wordIdx]; \
            wordIdx = wordTab[wordIdx]; \
         } \
         \
         /* First character failure, just advance text iterator. */ \
         else \
            ++textItr; \
      } \
   } \
   \
   if(wordTab != wordTabAuto) free(wordTab); \
   return NULL;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//=========================================================
// Copying functions.
//

//
// memcpy
//
void *memcpy(void *restrict s1, void const *restrict s2, size_t n)
{
   char       *i1 = s1;
   char const *i2 = s2;

   while(n--)
      *i1++ = *i2++;

   return s1;
}

//
// memmove
//
void *memmove(void *s1, void const *s2, size_t n)
{
   if(s1 < s2)
   {
      char       *i1 = s1;
      char const *i2 = s2;

      while(n--)
         *i1++ = *i2++;
   }
   else
   {
      char       *i1 = (char       *)s1 + n;
      char const *i2 = (char const *)s2 + n;

      while(n--)
         *--i1 = *--i2;
   }

   return s1;
}

//
// strcpy
//
char *strcpy(char *restrict s1, char const *restrict s2)
{
   for(char *i = s1; (*i = *s2); ++i, ++s2) {}

   return s1;
}

//
// strncpy
//
char *strncpy(char *restrict s1, char const *restrict s2, size_t n)
{
   char *i = s1;

   for(; (*i = *s2) && n; --n, ++i, ++s2) {}

   for(; n; --n) *i++ = 0;

   return s1;
}

//=========================================================
// Concatenation functions.
//

//
// strcat
//
char *strcat(char *restrict s1, char const *restrict s2)
{
   char *i = s1;

   while(*i) ++i;

   for(; (*i = *s2); ++i, ++s2) {}

   return s1;
}

//
// strncat
//
char *strncat(char *restrict s1, char const *restrict s2, size_t n)
{
   char *i = s1;

   while(*i) ++i;

   char *end = i + n;

   while(*s2 && n--) *i++ = *s2++;

   *i = 0;

   return s1;
}

//=========================================================
// Comparison functions.
//

//
// memcmp
//
int memcmp(void const *s1, void const *s2, size_t n)
{
   for(char const *i1 = s1, *i2 = s2; n--; ++i1, ++i2)
   {
      if(*i1 != *i2)
         return *i1 - *i2;
   }

   return 0;
}

//
// strcmp
//
int strcmp(char const *s1, char const *s2)
{
   for(; *s1 && *s2; ++s1, ++s2)
   {
      if(*s1 != *s2)
         return *s1 - *s2;
   }

   return *s1 - *s2;
}

//
// strcoll
//
int strcoll(char const *s1, char const *s2)
{
   return strcmp(s1, s2);
}

//
// strncmp
//
int strncmp(char const *s1, char const *s2, size_t n)
{
   for(; *s1 && *s2 && n--; ++s1, ++s2)
   {
      if(*s1 != *s2)
         return *s1 - *s2;
   }

   return *s1 - *s2;
}

//
// strxfrm
//
size_t strxfrm(char *restrict s1, char const *restrict s2, size_t n)
{
   size_t n2 = strlen(s2);

   if(n2 >= n) return n2;

   memcpy(s1, s2, n2 + 1);
   return n2;
}

//=========================================================
// Search functions
//

//
// memchr
//
void *memchr(void const *s, int c, size_t n)
{
   for(char const *i = s; n--; ++i)
   {
      if(*i == (char)c)
         return (void *)i;
   }

   return NULL;
}

//
// strchr
//
char *strchr(char const *s, int c)
{
   do
   {
      if(*s == (char)c)
         return (char *)s;
   }
   while(*s++);

   return NULL;
}

//
// strcspn
//
size_t strcspn(char const *s1, char const *s2)
{
   char const *i = s1;

   while(*i && strchr(s2, *i)) ++i;

   return i - s1;
}

//
// strpbrk
//
char *strpbrk(char const *s1, char const *s2)
{
   while(*s1)
   {
      if(strchr(s2, *s1))
         return (char *)s1;

      ++s1;
   }

   return NULL;
}

//
// strrchr
//
char *strrchr(char const *s, int c)
{
   char const *found = NULL;

   do
   {
      if(*s == (char)c)
         found = s;
   }
   while(*s++);

   return (char *)found;
}

//
// strspn
//
size_t strspn(char const *s1, char const *s2)
{
   size_t n = 0;

   while(*s1 && strchr(s2, *s1++)) ++n;

   return n;
}

//
// strstr
//
char *strstr(char const *text, char const *word)
{
   StrStrImpl(char,,,);
}

//
// strtok
//
char *strtok(char *restrict s1, char const *restrict s2)
{
   static char *tok;

   char *tmp;

   if(s1) tok = s1;

   if(!tok) return NULL;

   // Skip any separators.
   tok += strcspn(tok, s2);

   // If tok points to the null-terminator, there are no more tokens.
   if(!*tok) return NULL;

   // Store the pointer to the token in a temporary.
   tmp = tok;

   // Look for the end of the token.
   tok = strpbrk(tok, s2);

   // If no end is found, then we're at the last token.
   if(!tok) return tmp;

   // Null-terminate the token and point to the start of the next.
   *tok++ = 0;

   return tmp;
}

//=========================================================
// Miscellaneous functions.
//

//
// memset
//
void *memset(void *s, int c, size_t n)
{
   for(char *i = s; n--;)
      *i++ = (char)c;

   return s;
}

//
// strerror
//
char *strerror(int errnum)
{
   switch(errnum)
   {
   case EBADF:  return (char *)"bad file descriptor";
   case EDOM:   return (char *)"domain error";
   case EILSEQ: return (char *)"illegal sequence";
   case ERANGE: return (char *)"range error";
   default:     return (char *)"unknown error";
   }
}

//
// strlen
//
size_t strlen(char const *s)
{
   size_t n = 0;

   while(*s++) ++n;

   return n;
}

//=========================================================
// GNU extensions.
//

//
// strcasestr
//
char *strcasestr(char const *text, char const *word)
{
   StrStrImpl(char, toupper, case,);
}

//=========================================================
// Implementation extensions.
//

//
// strcasechr
//
char *strcasechr(char const *s, int c)
{
   c = toupper(c);

   do
   {
      if(toupper(*s) == (char)c)
         return (char *)s;
   }
   while(*s++);

   return NULL;
}

//
// strcasechr_str
//
char __str_ars *strcasechr_str(char __str_ars const *s, int c)
{
   c = toupper(c);

   do
   {
      if(toupper(*s) == (char)c)
         return (char __str_ars *)s;
   }
   while(*s++);

   return NULL;
}

//
// strcasestr_str
//
char __str_ars *strcasestr_str(char __str_ars const *text, char __str_ars const *word)
{
   StrStrImpl(char __str_ars, toupper, case, _str);
}

//
// strchr_str
//
char __str_ars *strchr_str(char __str_ars const *s, int c)
{
   do
   {
      if(*s == (char)c)
         return (char __str_ars *)s;
   }
   while(*s++);

   return NULL;
}

//
// strlen_str
//
size_t strlen_str(char __str_ars const *s)
{
   #if __GDCC_Family__ZDACS__
   {
      union {char __str_ars const *s; struct {__str lo; int hi;};} u = {s};

      if(!u.hi) return ACS_StrLen(u.lo);

      int n = ACS_StrLen(u.lo);
      if(n <= u.hi)
         return u.hi - n;
   }
   #endif

   size_t n = 0;

   while(*s++) ++n;

   return n;
}

//
// strstr_str
//
char __str_ars *strstr_str(char __str_ars const *text, char __str_ars const *word)
{
   StrStrImpl(char __str_ars,,, _str);
}

// EOF

