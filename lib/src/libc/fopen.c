//-----------------------------------------------------------------------------
//
// Copyright(C) 2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Input/output.
//
// File access functions.
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
// Static Objects                                                             |
//

[[no_init]]
static char Buffer_stderr[BUFSIZ];

[[no_init]]
static char Buffer_stdout[BUFSIZ];


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
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
   #if __GDCC_Family__ZDACS__
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
      setvbuf(stream, buf, _IOFBF, BUFSIZ);
   else
      setvbuf(stream, buf, _IONBF, 0);
}

//
// setvbuf
//
int setvbuf(FILE *restrict stream, char *restrict buf, int mode, size_t size)
{
   return stream->fn.fn_setbuf(stream, buf, size, mode);
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
// __get_stderr
//
FILE *__get_stderr(void)
{
   return &__stderr;
}

//
// __get_stdin
//
FILE *__get_stdin(void)
{
   return &__stdin;
}

//
// __get_stdout
//
FILE *__get_stdout(void)
{
   return &__stdout;
}

// EOF

