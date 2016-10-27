//-----------------------------------------------------------------------------
//
// Copyright(C) 2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Extended multibyte and wide character utilities.
//
//-----------------------------------------------------------------------------

#define __GDCC__DirectObject
#define _GNU_SOURCE

#include <wchar.h>

#include <errno.h>
#include <string.h>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//=========================================================
// Wide string copying functions.
//

wchar_t *wcscpy(wchar_t *restrict s1, wchar_t const *restrict s2);

wchar_t *wcsncpy(wchar_t *restrict s1, wchar_t const *restrict s2, size_t n);

//
// wmemcpy
//
wchar_t *wmemcpy(wchar_t *restrict s1, wchar_t const *restrict s2, size_t n)
{
   wchar_t       *i1 = s1;
   wchar_t const *i2 = s2;

   while(n--)
      *i1++ = *i2++;

   return s1;
}

//
// wmemmove
//
wchar_t *wmemmove(wchar_t *s1, wchar_t const *s2, size_t n)
{
   if(s1 < s2)
   {
      wchar_t       *i1 = s1;
      wchar_t const *i2 = s2;

      while(n--)
         *i1++ = *i2++;
   }
   else
   {
      wchar_t       *i1 = s1 + n;
      wchar_t const *i2 = s2 + n;

      while(n--)
         *--i1 = *--i2;
   }

   return s1;
}

//=========================================================
// Wide string search functions.
//

//
// wcschr
//
wchar_t *wcschr(wchar_t const *s, wchar_t c)
{
   do
   {
      if(*s == c)
         return (wchar_t *)s;
   }
   while(*s++);

   return NULL;
}

size_t wcscspn(wchar_t const *s1, wchar_t const *s2);

wchar_t *wcspbrk(wchar_t const *s1, wchar_t const *s2);

//
// wcsrchr
//
wchar_t *wcsrchr(wchar_t const *s, wchar_t c)
{
   wchar_t const *found = NULL;

   do
   {
      if(*s == c)
         found = s;
   }
   while(*s++);

   return (wchar_t *)found;
}

size_t wcsspn(wchar_t const *s1, wchar_t const *s2);

wchar_t *wcsstr(wchar_t const *s1, wchar_t const *s2);

wchar_t *wcstok(wchar_t *restrict s1, wchar_t const *restrict s2, wchar_t **restrict ptr);

wchar_t *wmemchr(wchar_t const *s, wchar_t c, size_t n);

//=========================================================
// Miscellaneous functions.
//

//
// wcslen
//
size_t wcslen(wchar_t const *s)
{
   size_t len = 0;

   while(*s++) ++len;

   return len;
}

//
// wmemset
//
wchar_t *wmemset(wchar_t *s, wchar_t c, size_t n)
{
   for(wchar_t *i = s; n--;)
      *i++ = c;

   return s;
}

//=========================================================
// Restartable multibyte/wide character conversion functions.
//

//
// mbrlen
//
size_t mbrlen(char const *restrict s, size_t n, mbstate_t *restrict ps)
{
   return mbrtowc(NULL, s, n, ps);
}

//
// mbrtowc
//
size_t mbrtowc(wchar_t *restrict pwc, char const *restrict s, size_t n,
   mbstate_t *restrict ps)
{
   if(!n || !s || !*s)
      {if(pwc) *pwc = 0; return 0;}

   wchar_t c = *s;
   size_t len;

        if(!(c & 0x80)) {if(pwc) *pwc = c; return 1;}
   else if(!(c & 0x40)) return errno = EILSEQ, (size_t)-1;
   else if(!(c & 0x20)) c = c & 0x1F, len = 1;
   else if(!(c & 0x10)) c = c & 0x0F, len = 2;
   else if(!(c & 0x08)) c = c & 0x07, len = 3;
   else if(!(c & 0x04)) c = c & 0x03, len = 4;
   else if(!(c & 0x02)) c = c & 0x01, len = 5;
   else if(!(c & 0x01)) return errno = EILSEQ, (size_t)-1;
   else                 return errno = EILSEQ, (size_t)-1;

   char const *itr = s + 1;

   if(n <= len)
   {
      char const *end = s + n;

      while(itr != end)
      {
         if((*itr++ & 0xC0) != 0x80)
            return errno = EILSEQ, (size_t)-1;
      }

      return (size_t)-2;
   }

   char const *end = itr + len;

   while(itr != end)
   {
      wchar_t i = *itr++;

      if((i & 0xC0) != 0x80)
         return errno = EILSEQ, (size_t)-1;

      c = (c << 6) | (i & 0x3F);
   }

   if(pwc) *pwc = c;

   return itr - s;
}

//
// wcrtomb
//
size_t wcrtomb(char *restrict s, wchar_t wc, mbstate_t *restrict ps)
{
   #define put(c) (*s++ = (char)(c))
   #define ret(n) if(s) len = (n); else return (n)

   size_t len;

   if(wc <= 0x7F)       {ret(1); put(0x00 | ((wc >>  0) & 0x7F)); goto put0;}
   if(wc <= 0x7FF)      {ret(2); put(0xC0 | ((wc >>  6) & 0x1F)); goto put1;}
   if(wc <= 0xFFFF)     {ret(3); put(0xE0 | ((wc >> 12) & 0x0F)); goto put2;}
   if(wc <= 0x1FFFFF)   {ret(4); put(0xF0 | ((wc >> 18) & 0x07)); goto put3;}
   if(wc <= 0x3FFFFFF)  {ret(5); put(0xF8 | ((wc >> 24) & 0x03)); goto put4;}
   if(wc <= 0x7FFFFFFF) {ret(6); put(0xFC | ((wc >> 30) & 0x01)); goto put5;}

   return errno = EILSEQ, (size_t)-1;

   put5: put(0x80 | ((wc >> 24) & 0x3F));
   put4: put(0x80 | ((wc >> 18) & 0x3F));
   put3: put(0x80 | ((wc >> 12) & 0x3F));
   put2: put(0x80 | ((wc >>  6) & 0x3F));
   put1: put(0x80 | ((wc >>  0) & 0x3F));
   put0: return len;

   #undef put
}

//=========================================================
// Restartable multibyte/wide string conversion functions.
//

//
// mbsrtowcs
//
size_t mbsrtowcs(wchar_t *restrict dst, char const **restrict src, size_t len,
   mbstate_t *restrict ps)
{
   char const *mbs = *src;
   size_t      l   = strlen(mbs);
   size_t      n   = 0;
   size_t      res;

   if(!dst) len = (size_t)-1;

   for(;; ++dst, ++n, l -= res)
   {
      // Check length.
      if(n == len) return *src = mbs, n;

      // Check null terminator.
      if(!*mbs) return dst ? *dst = '\0' : 0, *src = NULL, n;

      // Convert character.
      res = mbrtowc(dst, mbs, l, ps);
      if(res == (size_t)-2) return *src = mbs, n;
      if(res == (size_t)-1) return *src = mbs, res;
      mbs += res;
   }
}

//
// wcsrtombs
//
size_t wcsrtombs(char *restrict dst, wchar_t const **restrict src, size_t len,
   mbstate_t *restrict ps)
{
   wchar_t const *wcs = *src;
   size_t         n   = 0;
   size_t         res;
   wchar_t        wc;

   if(!dst) len = (size_t)-1;

   for(;; ++wcs, n += res, len -= res)
   {
      wc = *wcs;

      // Check length/validity.
      res = wcrtomb(NULL, wc, ps);
      if(res == (size_t)-1) return *src = wcs, res;
      if(res >  len)        return *src = wcs, n;

      // Check null terminator.
      if(!wc) return dst ? *dst = '\0' : 0, *src = NULL, n;

      // Convert character.
      if(dst)
      {
         wcrtomb(dst, wc, ps);
         dst += res;
      }
   }
}

// EOF

