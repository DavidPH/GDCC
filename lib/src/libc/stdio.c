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
#define _GNU_SOURCE

#include <stdio.h>

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

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
   if(stream->_flag & (_FILEFLAG_EOF | _FILEFLAG_ERR))
      return EOF;

   // Buffer read.
   if(stream->_get._ptr != stream->_get._end)
      return *stream->_get._ptr++;

   // Not a readable stream.
   if(!stream->_fn.read)
      return stream->_flag |= _FILEFLAG_EOF, EOF;

   // Unbuffered read.
   if(!stream->_get._len)
   {
      char buf[1];
      ssize_t res = stream->_fn.read(stream->_cookie, buf, 1);
      if(res <  0) return stream->_flag |= _FILEFLAG_ERR, EOF;
      if(res == 0) return stream->_flag |= _FILEFLAG_EOF, EOF;
      return buf[0];
   }

   // Buffered read.
   ssize_t res = stream->_fn.read(stream->_cookie, stream->_get._buf, stream->_get._len);
   if(res <  0) return stream->_flag |= _FILEFLAG_ERR, EOF;
   if(res == 0) return stream->_flag |= _FILEFLAG_EOF, EOF;
   stream->_get._ptr = stream->_get._buf;
   stream->_get._end = stream->_get._buf + res;
   return *stream->_get._ptr++;
}

//
// fgets
//
char *fgets(char *restrict s, int n, FILE *restrict stream)
{
   if(stream->_flag & (_FILEFLAG_EOF | _FILEFLAG_ERR))
      return NULL;

   char *itr = s;
   if(n) while(--n)
   {
      int c = fgetc(stream);

      if(c == EOF)
      {
         if(itr == s || (stream->_flag & _FILEFLAG_ERR))
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
   if(stream->_flag & _FILEFLAG_ERR)
      return EOF;

   // Not a writable stream.
   if(!stream->_fn.write)
      return EOF;

      // Unbuffered write.
   if(stream->_put._len == 0)
   {
      char buf[1] = {c};
      if(stream->_fn.write(stream->_cookie, buf, 1) != 1)
         return stream->_flag |= _FILEFLAG_ERR, EOF;
      return c;
   }

   // Line-buffered streams flush on linefeed.
   if(c == '\n' && stream->_flag & _FILEFLAG_LBF)
   {
      // No need to check for space, as there is always a byte reserved.
      *stream->_put._ptr++ = c;

      // Flush buffer.
      size_t  len = stream->_put._ptr - stream->_put._buf;
      ssize_t res = stream->_fn.write(stream->_cookie, stream->_put._buf, len);
      if(res != len) return stream->_flag |= _FILEFLAG_ERR, EOF;
      stream->_put._ptr = stream->_put._buf;

      // Return success.
      return c;
   }

   // Flush buffer if full.
   if(stream->_put._ptr == stream->_put._end)
   {
      size_t  len = stream->_put._ptr - stream->_put._buf;
      ssize_t res = stream->_fn.write(stream->_cookie, stream->_put._buf, len);
      if(res != len) return stream->_flag |= _FILEFLAG_ERR, EOF;
      stream->_put._ptr = stream->_put._buf;
   }

   // Buffered write.
   *stream->_put._ptr++ = c;

   // Return success.
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
   return fgetc(stream);
}

//
// getchar
//
int getchar(void)
{
   return fgetc(stdin);
}

//
// putc
//
int putc(int c, FILE *stream)
{
   return fputc(c, stream);
}

//
// putchar
//
int putchar(int c)
{
   return fputc(c, stdout);
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

   // No unget space.
   // TODO: Attempt to seek back the stream.
   if(stream->_get._ptr == stream->_get._buf)
      return EOF;

   *--stream->_get._ptr = c;

   stream->_flag &= ~_FILEFLAG_EOF;

   return c;
}

//=========================================================
// Direct input/output functions.
//

//
// fread
//
size_t fread(void *restrict ptr, size_t size, size_t nmemb, FILE *restrict stream)
{
   size_t res = 0;
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
size_t fwrite(void const *restrict ptr, size_t size, size_t nmemb, FILE *restrict stream)
{
   size_t res = 0;

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
   long int off = ftell(stream);
   if(off == EOF)
      return EOF;

   *pos = off;
   return 0;
}

//
// fseek
//
int fseek(FILE *stream, long int offset, int whence)
{
   if(!stream->_fn.seek) return EOF;

   if(fflush(stream) == EOF)
      return EOF;

   off_t off = offset;
   if(stream->_fn.seek(stream->_cookie, &off, whence) == -1)
      return EOF;

   return 0;
}

//
// fsetpos
//
int fsetpos(FILE *stream, fpos_t const *pos)
{
   return fseek(stream, *pos, SEEK_SET);
}

//
// ftell
//
long int ftell(FILE *stream)
{
   if(!stream->_fn.seek) return EOF;

   if(fflush(stream) == EOF)
      return EOF;

   off_t off = 0;
   if(stream->_fn.seek(stream->_cookie, &off, SEEK_CUR) == -1)
      return EOF;

   return off;
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
   stream->_flag &= ~(_FILEFLAG_EOF | _FILEFLAG_ERR);
}

//
// feof
//
int feof(FILE *stream)
{
   return stream->_flag & _FILEFLAG_EOF;
}

//
// ferror
//
int ferror(FILE *stream)
{
   return stream->_flag & _FILEFLAG_ERR;
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

