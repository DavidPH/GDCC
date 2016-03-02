//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Input/output.
//
//-----------------------------------------------------------------------------

#define __GDCC__DirectObject

#include <stdio.h>

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <GDCC.h>

#if __GDCC_Family__ZDACS__
#include <ACS_ZDoom.h>
#endif


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//=========================================================
// Operations on files.
//

//
// remove
//
int remove(char const *filename)
{
   return EOF;
}

//
// rename
//
int rename(char const *oldname, char const *newname)
{
   return EOF;
}

//
// tmpfile
//
FILE *tmpfile(void)
{
   return NULL;
}

//
// tmpnam
//
char *tmpnam(char *s)
{
   return NULL;
}

//=========================================================
// Character input/output functions.
//

//
// fgetc
//
int fgetc(FILE *stream)
{
   if(stream->flags & _FILEFLAG_EOF)
      return EOF;

   if(stream->buf_get.buf_ptr == stream->buf_get.buf_end)
      return stream->fn.fn_fetch(stream);

   return *stream->buf_get.buf_ptr++;
}

//
// fgets
//
char *fgets(char *restrict s, int n, FILE *restrict stream)
{
   if(stream->flags & _FILEFLAG_EOF)
      return NULL;

   char *itr = s;
   if(n) while(--n)
   {
      int c = fgetc(stream);

      if(c == EOF)
      {
         if(itr == s || (stream->flags & _FILEFLAG_ERR))
           return NULL;

         break;
      }

      *itr++ = c;
      if(c == '\n') break;
   }

   *itr = '\0';
   return s;
}

//
// fputc
//
int fputc(int c, FILE *stream)
{
   if(stream->buf_put.buf_ptr == stream->buf_put.buf_end ||
      (stream->buf_put.buf_mode == _IOLBF && c == '\n'))
      return stream->fn.fn_flush(stream, c);

   *stream->buf_put.buf_ptr++ = c;

   return c;
}

//
// fputs
//
int fputs(char const *restrict s, FILE *restrict stream)
{
   while(*s)
   {
      if(fputc(*s++, stream) == EOF)
         return EOF;
   }

   return 1;
}

//
// getc
//
int getc(FILE *stream)
{
   if(stream->flags & _FILEFLAG_EOF)
      return EOF;

   if(stream->buf_get.buf_ptr == stream->buf_get.buf_end)
      return stream->fn.fn_fetch(stream);

   return *stream->buf_get.buf_ptr++;
}

//
// getchar
//
int getchar(void)
{
   if(stdin->flags & _FILEFLAG_EOF)
      return EOF;

   if(stdin->buf_get.buf_ptr == stdin->buf_get.buf_end)
      return stdin->fn.fn_fetch(stdin);

   return *stdin->buf_get.buf_ptr++;
}

//
// putc
//
int putc(int c, FILE *stream)
{
   if(stream->buf_put.buf_ptr == stream->buf_put.buf_end ||
      (stream->buf_put.buf_mode == _IOLBF && c == '\n'))
      return stream->fn.fn_flush(stream, c);

   *stream->buf_put.buf_ptr++ = c;

   return c;
}

//
// putchar
//
int putchar(int c)
{
   if(stdout->buf_put.buf_ptr == stdout->buf_put.buf_end ||
      (stdout->buf_put.buf_mode == _IOLBF && c == '\n'))
      return stdout->fn.fn_flush(stdout, c);

   *stdout->buf_put.buf_ptr++ = c;

   return c;
}

//
// puts
//
int puts(char const *s)
{
   while(*s)
   {
      if(putchar(*s++) == EOF)
         return EOF;
   }

   if(putchar('\n') == EOF)
      return EOF;

   return 1;
}

//
// ungetc
//
int ungetc(int c, FILE *stream)
{
   if(c == EOF)
      return EOF;

   if(stream->buf_get.buf_ptr == stream->buf_get.buf_beg ||
      *(stream->buf_get.buf_ptr - 1) != c)
      return stream->fn.fn_unget(stream, c);

   --stream->buf_get.buf_ptr;

   stream->flags &= ~_FILEFLAG_EOF;

   return c;
}

//=========================================================
// Direct input/output functions.
//

//
// fread
//
size_t fread(void *restrict ptr, size_t size, size_t nmemb,
   FILE *restrict stream)
{
   size_t res;
   int c;

   for(char *itr = ptr; nmemb--; itr += size, ++res)
   {
      for(size_t i = size; i--;)
      {
         if((c = fgetc(stream)) == EOF)
            return res;

         *itr++ = c;
      }
   }

   return res;
}

//
// fwrite
//
size_t fwrite(void const *restrict ptr, size_t size, size_t nmemb,
   FILE *restrict stream)
{
   size_t res;

   for(char const *itr = ptr; nmemb--; ++res)
   {
      for(size_t i = size; i--;)
      {
         if(fputc(*itr++, stream) == EOF)
            return res;
      }
   }

   return res;
}

//=========================================================
// File positioning functions.
//

//
// fgetpos
//
int fgetpos(FILE *restrict stream, fpos_t *restrict pos)
{
   if(stream->fn.fn_getpos(stream, pos) == EOF)
      return EOF;

   return 0;
}

//
// fseek
//
int fseek(FILE *stream, long int offset, int whence)
{
   fpos_t pos;

   if(whence == SEEK_SET)
   {
      __ltofpos(&pos, offset);
      if(stream->fn.fn_setpos(stream, &pos) == EOF)
         return EOF;

      return 0;
   }

   if(whence == SEEK_CUR)
   {
      if(stream->fn.fn_getpos(stream, &pos) == EOF)
         return EOF;

      __ltofpos(&pos, offset + __fpostol(&pos));
      if(stream->fn.fn_setpos(stream, &pos) == EOF)
         return EOF;

      return 0;
   }

   // SEEK_END is not supported.

   return EOF;
}

//
// fsetpos
//
int fsetpos(FILE *stream, fpos_t const *pos)
{
   if(stream->fn.fn_setpos(stream, pos) == EOF)
      return EOF;

   return 0;
}

//
// ftell
//
long int ftell(FILE *stream)
{
   fpos_t pos;
   if(stream->fn.fn_getpos(stream, &pos) == EOF)
      return -1L;

   return __fpostol(&pos);
}

//
// rewind
//
void rewind(FILE *stream)
{
   if(!fseek(stream, 0, SEEK_SET))
      clearerr(stream);
}

//=========================================================
// Error-handling functions.
//

//
// clearerr
//
void clearerr(FILE *stream)
{
   stream->flags &= ~(_FILEFLAG_EOF | _FILEFLAG_ERR);
}

//
// feof
//
int feof(FILE *stream)
{
   return stream->flags & _FILEFLAG_EOF;
}

//
// ferror
//
int ferror(FILE *stream)
{
   return stream->flags & _FILEFLAG_ERR;
}

//
// perror
//
void perror(char const *s)
{
   char const *err = strerror(errno);

   if(s && *s)
      fprintf(stderr, "%s: %s\n", s, err);
   else
      fprintf(stderr, "%s\n", err);
}

//=========================================================
// Implementation extensions.
//

//
// __fwrite_str
//
size_t __fwrite_str(void const __str_ars *restrict ptr, size_t size,
   size_t nmemb, FILE *stream)
{
   size_t res = 0;

   for(char const __str_ars *itr = ptr; nmemb--; ++res)
   {
      for(size_t i = size; i--;)
      {
         if(fputc(*itr++, stream) == EOF)
            return res;
      }
   }

   return res;
}

// EOF

