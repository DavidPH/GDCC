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

#include <wchar.h>

#include <errno.h>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// mbctowc
//


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

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
size_t wcrtomb(char *restrict s, wchar_t wc, mbstate_t *restrict ps);

// EOF

