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
#include <stdlib.h>
#include <string.h>

#include <GDCC.h>

#if __GDCC_Target__ZDoom__
#include <ACS_ZDoom.h>
#endif


//----------------------------------------------------------------------------|
// Static Prototypes                                                          |
//

static int FILE_fn_fail_close(FILE *stream);
static int FILE_fn_fail_fetch(FILE *stream);
static int FILE_fn_fail_flush(FILE *stream, int c);
static int FILE_fn_fail_getpos(FILE *stream, fpos_t *pos);
static int FILE_fn_fail_open(FILE *stream, char const *filename, char const *mode);
static int FILE_fn_fail_reopen(FILE *stream, char const *filename, char const *mode);
static int FILE_fn_fail_setbuf(FILE *stream, char *buf, size_t size, int mode);
static int FILE_fn_fail_setpos(FILE *stream, fpos_t const *pos);
static int FILE_fn_fail_unget(FILE *stream, int c);

static int FILE_fn_pass_close(FILE *stream);
static int FILE_fn_pass_open(FILE *stream, char const *filename, char const *mode);

static int FILE_fn_stdout_flush(FILE *stream, int c);
static int FILE_fn_stdout_setbuf(FILE *stream, char *buf, size_t size, int mode);


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

[[no_init]]
static char Buffer_stderr[BUFSIZ];

[[no_init]]
static char Buffer_stdout[BUFSIZ];


//----------------------------------------------------------------------------|
// Extern Variables                                                           |
//

//
// __stderr
//
FILE __stderr =
{
   {
      .fn_close  = FILE_fn_fail_close,
      .fn_fetch  = FILE_fn_fail_fetch,
      .fn_flush  = FILE_fn_stdout_flush,
      .fn_getpos = FILE_fn_fail_getpos,
      .fn_open   = FILE_fn_fail_open,
      .fn_reopen = FILE_fn_fail_reopen,
      .fn_setbuf = FILE_fn_stdout_setbuf,
      .fn_setpos = FILE_fn_fail_setpos,
      .fn_unget  = FILE_fn_fail_unget,
   },

   {NULL, NULL, NULL, 0},
   {Buffer_stderr, Buffer_stderr, Buffer_stderr + sizeof(Buffer_stderr), _IOLBF},

   NULL, 0
};

//
// __stdin
//
FILE __stdin =
{
   {
      .fn_close  = FILE_fn_fail_close,
      .fn_fetch  = FILE_fn_fail_fetch,
      .fn_flush  = FILE_fn_fail_flush,
      .fn_getpos = FILE_fn_fail_getpos,
      .fn_open   = FILE_fn_fail_open,
      .fn_reopen = FILE_fn_fail_reopen,
      .fn_setbuf = FILE_fn_fail_setbuf,
      .fn_setpos = FILE_fn_fail_setpos,
      .fn_unget  = FILE_fn_fail_unget,
   },

   {NULL, NULL, NULL, 0},
   {NULL, NULL, NULL, 0},

   NULL, 0
};

//
// __stdout
//
FILE __stdout =
{
   {
      .fn_close  = FILE_fn_fail_close,
      .fn_fetch  = FILE_fn_fail_fetch,
      .fn_flush  = FILE_fn_stdout_flush,
      .fn_getpos = FILE_fn_fail_getpos,
      .fn_open   = FILE_fn_fail_open,
      .fn_reopen = FILE_fn_fail_reopen,
      .fn_setbuf = FILE_fn_stdout_setbuf,
      .fn_setpos = FILE_fn_fail_setpos,
      .fn_unget  = FILE_fn_fail_unget,
   },

   {NULL, NULL, NULL, 0},
   {Buffer_stdout, Buffer_stdout, Buffer_stdout + sizeof(Buffer_stdout), _IOLBF},

   NULL, 0
};


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// FILE_fn_fail_close
//
static int FILE_fn_fail_close(FILE *stream)
{
   return EOF;
}

//
// FILE_fn_fail_fetch
//
static int FILE_fn_fail_fetch(FILE *stream)
{
   stream->flags |= _FILEFLAG_EOF;
   return EOF;
}

//
// FILE_fn_fail_flush
//
static int FILE_fn_fail_flush(FILE *stream, int c)
{
   return EOF;
}

//
// FILE_fn_fail_getpos
//
static int FILE_fn_fail_getpos(FILE *stream, fpos_t *pos)
{
   errno = EBADF;
   return EOF;
}

//
// FILE_fn_fail_open
//
static int FILE_fn_fail_open(FILE *stream, char const *filename, char const *mode)
{
   return EOF;
}

//
// FILE_fn_fail_reopen
//
static int FILE_fn_fail_reopen(FILE *stream, char const *filename, char const *mode)
{
   return EOF;
}

//
// FILE_fn_fail_setbuf
//
static int FILE_fn_fail_setbuf(FILE *stream, char *buf, size_t size, int mode)
{
   return EOF;
}

//
// FILE_fn_fail_setpos
//
static int FILE_fn_fail_setpos(FILE *stream, fpos_t const *pos)
{
   errno = EBADF;
   return EOF;
}

//
// FILE_fn_fail_unget
//
static int FILE_fn_fail_unget(FILE *stream, int c)
{
   return EOF;
}

//
// FILE_fn_pass_close
//
static int FILE_fn_pass_close(FILE *stream)
{
   return 0;
}

//
// FILE_fn_pass_open
//
static int FILE_fn_pass_open(FILE *stream)
{
   return 0;
}

//
// FILE_fn_stdout_flush
//
static int FILE_fn_stdout_flush(FILE *stream, int c)
{
   #if __GDCC_Target__ZDoom__
   // Because flushing in this way generates a spurious linefeed, do not honor
   // generic flush requests.
   if(c == EOF)
      return 0;

   ACS_BeginPrint();
   ACS_PrintGlobalCharRange((int)stream->buf_put.buf_beg, __GDCC__Sta, 0,
      stream->buf_put.buf_ptr - stream->buf_put.buf_beg);
   if(c != '\n')
      ACS_PrintChar(c);
   ACS_EndLog();

   stream->buf_put.buf_ptr = stream->buf_put.buf_beg;

   return c != EOF ? c : 0;
   #else
   return EOF;
   #endif
}

//
// FILE_fn_stdout_setbuf
//
static int FILE_fn_stdout_setbuf(FILE *stream, char *buf, size_t size, int mode)
{
   // Only buffered I/O is possible currently.
   if(mode == _IONBF)
      return EOF;

   size_t used = stream->buf_put.buf_ptr - stream->buf_put.buf_beg;

   // Buffer must be large enough for current buffer contents.
   if(size < used)
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
      else
      {
         if(size <= sizeof(Buffer_stdout))
         {
            buf  = Buffer_stdout;
            size = sizeof(Buffer_stdout);
            goto buf_good;
         }
      }

      return EOF;
   }

buf_good:
   if(stream->buf_put.buf_ptr != stream->buf_put.buf_beg)
      memcpy(buf, stream->buf_put.buf_beg, used);

   stream->buf_put.buf_beg  = buf;
   stream->buf_put.buf_ptr  = buf + used;
   stream->buf_put.buf_end  = buf + size;
 //stream->buf_put.buf_mode = _IOLBF;
}


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
// File access functions.
//

//
// fclose
//
int fclose(FILE *stream)
{
   int res = 0;

   if(stream->fn.fn_flush(stream, EOF) == EOF)
      res = EOF;

   if(stream->fn.fn_close(stream) == EOF)
      res = EOF;

   free(stream);

   return res;
}

//
// fflush
//
int fflush(FILE *stream)
{
   return stream->fn.fn_flush(stream, EOF);
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
      stream->fn.fn_setbuf(stream, buf, BUFSIZ, _IOFBF);
   else
      stream->fn.fn_setbuf(stream, buf, 0, _IONBF);
}

//
// setvbuf
//
int setvbuf(FILE *restrict stream, char *restrict buf, int mode, size_t size)
{
   return stream->fn.fn_setbuf(stream, buf, size, mode);
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
// __fopen_fn
//
FILE *__fopen_fn(__FILE_fn const *fn, size_t size, void *data,
   char const *filename, char const *mode)
{
   FILE *f;

   if(!(f = malloc(sizeof(FILE) + size)))
      return NULL;

   f->fn      = *fn;
   f->buf_get = (__FILE_buf const){NULL, NULL, NULL, _IONBF};
   f->buf_put = (__FILE_buf const){NULL, NULL, NULL, _IONBF};
   f->data    = data;
   f->flags   = 0;

   if(f->fn.fn_open && f->fn.fn_open(f, filename, mode) == EOF)
   {
      free(f);
      return NULL;
   }

   if(!f->fn.fn_close)  f->fn.fn_close  = FILE_fn_pass_close;
   if(!f->fn.fn_fetch)  f->fn.fn_fetch  = FILE_fn_fail_fetch;
   if(!f->fn.fn_flush)  f->fn.fn_flush  = FILE_fn_fail_flush;
   if(!f->fn.fn_getpos) f->fn.fn_getpos = FILE_fn_fail_getpos;
   if(!f->fn.fn_open)   f->fn.fn_open   = FILE_fn_pass_open;
   if(!f->fn.fn_reopen) f->fn.fn_reopen = FILE_fn_fail_reopen;
   if(!f->fn.fn_setbuf) f->fn.fn_setbuf = FILE_fn_fail_setbuf;
   if(!f->fn.fn_setpos) f->fn.fn_setpos = FILE_fn_fail_setpos;
   if(!f->fn.fn_unget)  f->fn.fn_unget  = FILE_fn_fail_unget;

   return f;
}

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

