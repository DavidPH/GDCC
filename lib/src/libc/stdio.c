//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2015 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Input/output.
//
//-----------------------------------------------------------------------------

#include <stdio.h>

#include <errno.h>
#include <stdarg.h>
#include <string.h>

#include <GDCC.h>

#if __GDCC_Target__ZDoom__
#include <ACS_ZDoom.h>
#endif


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

[[no_init]]
static char Buffer_stderr[BUFSIZ];

[[no_init]]
static char Buffer_stdout[BUFSIZ];


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

FILE __stderr = {Buffer_stderr, sizeof(Buffer_stderr), 0, 0};
FILE __stdin  = {0, 0, 0, 0};
FILE __stdout = {Buffer_stdout, sizeof(Buffer_stdout), 0, 0};


//----------------------------------------------------------------------------|
// Global Functions                                                           |
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
// File access functions.
//

//
// fclose
//
int fclose(FILE *stream)
{
   return EOF;
}

//
// fflush
//
int fflush(FILE *stream)
{
   return 0;
}

//
// fopen
//
FILE *fopen(char const *restrict filename, char const *restrict mode)
{
   return NULL;
}

//
// freopen
//
FILE *freopen(char const *restrict filename, char const *restrict mode,
   FILE *restrict stream)
{
   return NULL;
}

//
// setbuf
//
void setbuf(FILE *restrict stream, char *restrict buf)
{
   if(buf)
      setvbuf(stream, buf, _IOFBF, BUFSIZ);
   else
      setvbuf(stream, buf, _IONBF, 0);
}

//
// setvbuf
//
int setvbuf(FILE *restrict stream, char *restrict buf, int mode, size_t size)
{
   // Only buffered I/O is possible currently.
   if(mode == _IONBF)
      return EOF;

   // Buffer must be large enough for current buffer contents.
   if(size < stream->bufPos)
      return EOF;

   if(!buf)
   {
      // If possible, reuse the associated static buffer.
      if(stream == stderr)
      {
         if(size <= sizeof(Buffer_stderr))
         {
            buf  = Buffer_stderr;
            size = sizeof(Buffer_stderr);
            goto buf_good;
         }
      }
      else if(stream == stdout)
      {
         if(size <= sizeof(Buffer_stdout))
         {
            buf  = Buffer_stdout;
            size = sizeof(Buffer_stdout);
            goto buf_good;
         }
      }

      // TODO: Allocate buffer.
      return EOF;
   }

buf_good:
   if(stream->bufPos)
      memcpy(buf, stream->buf, stream->bufPos);

   stream->buf    = buf;
   stream->bufLen = size;
}

//=========================================================
// Formatted input/output functions.
//

//
// fscanf
//
int fscanf(FILE *restrict stream, char const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = vfscanf(stream, format, arg);
   va_end(arg);

   return ret;
}

//
// scanf
//
int scanf(char const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = vfscanf(stdin, format, arg);
   va_end(arg);

   return ret;
}

//
// sscanf
//
int sscanf(char const *restrict s, char const *restrict format, ...)
{
   va_list arg;
   int     ret;

   va_start(arg, format);
   ret = vsscanf(s, format, arg);
   va_end(arg);

   return ret;
}

//
// vfscanf
//
int vfscanf(FILE *restrict stream, char const *restrict format, __va_list arg)
{
   return EOF;
}

//
// vscanf
//
int vscanf(char const *restrict format, __va_list arg)
{
   return vfscanf(stdin, format, arg);
}

//
// vsscanf
//
int vsscanf(char const *restrict s, char const *restrict format, __va_list arg)
{
   return EOF;
}

//=========================================================
// Character input/output functions.
//

//
// fgetc
//
int fgetc(FILE *stream)
{
   return EOF;
}

//
// fgets
//
char *fgets(char *restrict s, int n, FILE *restrict stream)
{
   return NULL;
}

//
// fputc
//
int fputc(int c, FILE *stream)
{
   if(c == '\n') return __fendl(stream) == EOF ? EOF : c;

   if(stream->bufPos == stream->bufLen)
      return stream->flags |= _FILEFLAG_OVR, EOF;

   stream->buf[stream->bufPos++] = c;
   return c;
}

//
// fputs
//
int fputs(char const *restrict s, FILE *restrict stream)
{
   while(*s) if(fputc(*s, stream) == EOF) return EOF;
   return 1;
}

//
// getc
//
int getc(FILE *stream)
{
   return EOF;
}

//
// getchar
//
int getchar(void)
{
   return EOF;
}

//
// putc
//
int putc(int c, FILE *stream)
{
   if(c == '\n') return __fendl(stream) == EOF ? EOF : c;

   if(stream->bufPos == stream->bufLen)
      return stream->flags |= _FILEFLAG_OVR, EOF;

   stream->buf[stream->bufPos++] = c;
   return c;
}

//
// putchar
//
int putchar(int c)
{
   if(c == '\n') return __fendl(stdout) == EOF ? EOF : c;

   if(stdout->bufPos == stdout->bufLen)
      return stdout->flags |= _FILEFLAG_OVR, EOF;

   stdout->buf[stdout->bufPos++] = c;
   return c;
}

//
// puts
//
int puts(char const *s)
{
   while(*s) if(putchar(*s) == EOF) return EOF;
   if(__fendl(stdout) == EOF) return EOF;
   return 1;
}

//
// ungetc
//
int ungetc(int c, FILE *stream)
{
   return EOF;
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
   return 0;
}

//
// fwrite
//
size_t fwrite(void const *restrict ptr, size_t size, size_t nmemb,
   FILE *restrict stream)
{
   size_t res    = 0;
   size_t bufLen = stream->bufLen;
   size_t bufPos = stream->bufPos;
   char  *buf    = stream->buf;
   char   c;

   for(char const *itr = ptr, *end = itr + size * nmemb; itr != end;)
   {
      if(bufPos + size > bufLen)
         return stream->flags |= _FILEFLAG_OVR, res;

      for(size_t i = size; i--;)
      {
         if((c = *itr++) == '\n')
         {
            stream->bufPos = bufPos;

            if(__fendl(stream) == EOF)
               return res;

            bufPos = stream->bufPos;
         }
         else
            buf[bufPos++] = c;
      }

      ++res;
   }

   stream->bufPos = bufPos;
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
   errno = EBADF;
   return EOF;
}

//
// fseek
//
int fseek(FILE *stream, long int offset, int whence)
{
   errno = EBADF;
   return EOF;
}

//
// fsetpos
//
int fsetpos(FILE *stream, fpos_t const *pos)
{
   errno = EBADF;
   return EOF;
}

//
// ftell
//
long int ftell(FILE *stream)
{
   errno = EBADF;
   return -1L;
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
   stream->flags = 0;
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
   return stream->flags & (_FILEFLAG_ERR | _FILEFLAG_OVR);
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
// __fendl
//
int __fendl(FILE *stream)
{
   #if __GDCC_Target__ZDoom__
   ACS_BeginPrint();
   ACS_PrintGlobalCharRange((int)stream->buf, __GDCC__Sta, 0, stream->bufPos);
   ACS_EndLog();

   stream->bufPos = 0;

   return '\n';
   #else
   return EOF;
   #endif
}

//
// __fwrite_str
//
size_t __fwrite_str(void const __str_ars *restrict ptr, size_t size,
   size_t nmemb, FILE *stream)
{
   size_t res    = 0;
   size_t bufLen = stream->bufLen;
   size_t bufPos = stream->bufPos;
   char  *buf    = stream->buf;
   char   c;

   for(char const __str_ars *itr = ptr, *end = itr + size * nmemb; itr != end;)
   {
      if(bufPos + size > bufLen)
         return stream->flags |= _FILEFLAG_OVR, res;

      for(size_t i = size; i--;)
      {
         if((c = *itr++) == '\n')
         {
            stream->bufPos = bufPos;

            if(__fendl(stream) == EOF)
               return res;

            bufPos = stream->bufPos;
         }
         else
            buf[bufPos++] = c;
      }

      ++res;
   }

   stream->bufPos = bufPos;
   return res;
}

// EOF

