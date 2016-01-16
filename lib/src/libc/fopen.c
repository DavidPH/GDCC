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
static int FILE_fn_pass_flush(FILE *stream, int c);
static int FILE_fn_pass_open(FILE *stream, char const *filename, char const *mode);

static int FILE_fn_stdout_flush(FILE *stream, int c);
static int FILE_fn_stdout_setbuf(FILE *stream, char *buf, size_t size, int mode);

static int FILE_fn_strr_getpos(FILE *stream, fpos_t *pos);
static int FILE_fn_strr_setpos(FILE *stream, fpos_t const *pos);

static int FILE_fn_strw_flush(FILE *stream, int c);
static int FILE_fn_strw_getpos(FILE *stream, fpos_t *pos);
static int FILE_fn_strw_setpos(FILE *stream, fpos_t const *pos);

static int FILE_fn_strentr_fetch(FILE *stream);
static int FILE_fn_strentr_getpos(FILE *stream, fpos_t *pos);
static int FILE_fn_strentr_setpos(FILE *stream, fpos_t const *pos);
static int FILE_fn_strentr_unget(FILE *stream, int c);


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

//
// __strfilr
//
FILE __strfilr =
{
   {
      .fn_close  = FILE_fn_fail_close,
      .fn_fetch  = FILE_fn_fail_fetch,
      .fn_flush  = FILE_fn_fail_flush,
      .fn_getpos = FILE_fn_strr_getpos,
      .fn_open   = FILE_fn_fail_open,
      .fn_reopen = FILE_fn_fail_reopen,
      .fn_setbuf = FILE_fn_fail_setbuf,
      .fn_setpos = FILE_fn_strr_setpos,
      .fn_unget  = FILE_fn_fail_unget,
   },

   {NULL, NULL, NULL, 0},
   {NULL, NULL, NULL, _IOFBF},

   NULL, 0
};

//
// __strfiler_str
//
FILE __strfilr_str =
{
   {
      {
         .fn_close  = FILE_fn_fail_close,
         .fn_fetch  = FILE_fn_strentr_fetch,
         .fn_flush  = FILE_fn_fail_flush,
         .fn_getpos = FILE_fn_strentr_getpos,
         .fn_open   = FILE_fn_fail_open,
         .fn_reopen = FILE_fn_fail_reopen,
         .fn_setbuf = FILE_fn_fail_setbuf,
         .fn_setpos = FILE_fn_strentr_setpos,
         .fn_unget  = FILE_fn_strentr_unget,
      },

      {NULL, NULL, NULL, 0},
      {NULL, NULL, NULL, 0},

      NULL, 0
   },

   {NULL, NULL, NULL, _IOFBF}
};

//
// __strfilew
//
FILE __strfilew =
{
   {
      .fn_close  = FILE_fn_fail_close,
      .fn_fetch  = FILE_fn_fail_fetch,
      .fn_flush  = FILE_fn_pass_flush,
      .fn_getpos = FILE_fn_strw_getpos,
      .fn_open   = FILE_fn_fail_open,
      .fn_reopen = FILE_fn_fail_reopen,
      .fn_setbuf = FILE_fn_fail_setbuf,
      .fn_setpos = FILE_fn_strw_setpos,
      .fn_unget  = FILE_fn_fail_unget,
   },

   {NULL, NULL, NULL, 0},
   {NULL, NULL, NULL, _IOFBF},

   NULL, 0
};


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//=========================================================
// Automatic fail functions.
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

//=========================================================
// Automatic success functions.
//

//
// FILE_fn_pass_close
//
static int FILE_fn_pass_close(FILE *stream)
{
   return 0;
}

//
// FILE_fn_pass_flush
//
static int FILE_fn_pass_flush(FILE *stream, int c)
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

//=========================================================
// stdout functions.
//

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

//=========================================================
// __stropenr functions.
//

//
// FILE_fn_strr_getpos
//
static int FILE_fn_strr_getpos(FILE *stream, fpos_t *pos)
{
   __ltofpos(pos, stream->buf_get.buf_ptr - stream->buf_get.buf_beg);
   return 0;
}

//
// FILE_fn_strr_setpos
//
static int FILE_fn_strr_setpos(FILE *stream, fpos_t const *pos)
{
   stream->buf_get.buf_ptr = stream->buf_get.buf_beg + __fpostol(pos);
   return 0;
}

//=========================================================
// __stropenr_str functions.
//

//
// FILE_fn_strentr_fetch
//
static int FILE_fn_strentr_fetch(FILE *stream_)
{
   __FILE_str *stream = (__FILE_str *)stream_;

   if(stream->buf_get.buf_ptr == stream->buf_get.buf_end)
   {
      stream->f.flags |= _FILEFLAG_EOF;
      return EOF;
   }

   return *stream->buf_get.buf_ptr++;
}

//
// FILE_fn_strentr_getpos
//
static int FILE_fn_strentr_getpos(FILE *stream_, fpos_t *pos)
{
   __FILE_str *stream = (__FILE_str *)stream_;

   __ltofpos(pos, stream->buf_get.buf_ptr - stream->buf_get.buf_beg);
   return 0;
}

//
// FILE_fn_strentr_setpos
//
static int FILE_fn_strentr_setpos(FILE *stream_, fpos_t const *pos)
{
   __FILE_str *stream = (__FILE_str *)stream_;

   stream->buf_get.buf_ptr = stream->buf_get.buf_beg + __fpostol(pos);
   return 0;
}

//
// FILE_fn_strentr_unget
//
static int FILE_fn_strentr_unget(FILE *stream_, int c)
{
   __FILE_str *stream = (__FILE_str *)stream_;

   if(stream->buf_get.buf_ptr == stream->buf_get.buf_beg ||
      *(stream->buf_get.buf_ptr - 1) != c)
   {
      return EOF;
   }

   --stream->buf_get.buf_ptr;

   stream->f.flags &= ~_FILEFLAG_EOF;

   return c;
}

//=========================================================
// __stropenw functions.
//

//
// FILE_fn_strw_flush
//
static int FILE_fn_strw_flush(FILE *stream, int c)
{
   // Can only handle generic flush requests...
   if(c != EOF)
      return EOF;

   // ... Which are a no-op, because the buffer is the final output.
   return 0;
}

//
// FILE_fn_strw_getpos
//
static int FILE_fn_strw_getpos(FILE *stream, fpos_t *pos)
{
   __ltofpos(pos, stream->buf_put.buf_ptr - stream->buf_put.buf_beg);
   return 0;
}

//
// FILE_fn_strw_setpos
//
static int FILE_fn_strw_setpos(FILE *stream, fpos_t const *pos)
{
   stream->buf_put.buf_ptr = stream->buf_put.buf_beg + __fpostol(pos);
   return 0;
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
// __stropenr
//
FILE *__stropenr(char const *str, size_t size)
{
   FILE *f;

   if(!(f = malloc(sizeof(FILE))))
      return NULL;

   char *buf = (char *)str;

   f->buf_get = (__FILE_buf const){buf, buf, buf + size, _IOFBF};
   f->buf_put = (__FILE_buf const){NULL, NULL, NULL, _IONBF};
   f->data    = NULL;
   f->flags   = 0;

   f->fn.fn_close  = FILE_fn_pass_close;
   f->fn.fn_fetch  = FILE_fn_fail_fetch;
   f->fn.fn_flush  = FILE_fn_fail_flush;
   f->fn.fn_getpos = FILE_fn_strr_getpos;
   f->fn.fn_open   = FILE_fn_pass_open;
   f->fn.fn_reopen = FILE_fn_fail_reopen;
   f->fn.fn_setbuf = FILE_fn_fail_setbuf;
   f->fn.fn_setpos = FILE_fn_strr_setpos;
   f->fn.fn_unget  = FILE_fn_fail_unget;

   return f;
}

//
// __stropenr_str
//
FILE *__stropenr_str(char __str_ars const *str, size_t size)
{
   __FILE_str *f;

   if(!(f = malloc(sizeof(__FILE_str))))
      return NULL;

   f->f.buf_get = (__FILE_buf const){NULL, NULL, NULL, _IONBF};
   f->f.buf_put = (__FILE_buf const){NULL, NULL, NULL, _IONBF};
   f->f.data    = NULL;
   f->f.flags   = 0;

   f->f.fn.fn_close  = FILE_fn_pass_close;
   f->f.fn.fn_fetch  = FILE_fn_strentr_fetch;
   f->f.fn.fn_flush  = FILE_fn_fail_flush;
   f->f.fn.fn_getpos = FILE_fn_strentr_getpos;
   f->f.fn.fn_open   = FILE_fn_pass_open;
   f->f.fn.fn_reopen = FILE_fn_fail_reopen;
   f->f.fn.fn_setbuf = FILE_fn_fail_setbuf;
   f->f.fn.fn_setpos = FILE_fn_strentr_setpos;
   f->f.fn.fn_unget  = FILE_fn_strentr_unget;

   char __str_ars *buf = (char __str_ars *)str;

   f->buf_get = (__FILE_buf_str const){buf, buf, buf + size, _IOFBF};

   return &f->f;
}

//
// __stropenw
//
FILE *__stropenw(char *str, size_t size)
{
   FILE *f;

   if(!(f = malloc(sizeof(FILE))))
      return NULL;

   f->buf_get = (__FILE_buf const){NULL, NULL, NULL, _IONBF};
   f->buf_put = (__FILE_buf const){str, str, str + size, _IOFBF};
   f->data    = NULL;
   f->flags   = 0;

   f->fn.fn_close  = FILE_fn_pass_close;
   f->fn.fn_fetch  = FILE_fn_fail_fetch;
   f->fn.fn_flush  = FILE_fn_strw_flush;
   f->fn.fn_getpos = FILE_fn_strw_getpos;
   f->fn.fn_open   = FILE_fn_pass_open;
   f->fn.fn_reopen = FILE_fn_fail_reopen;
   f->fn.fn_setbuf = FILE_fn_fail_setbuf;
   f->fn.fn_setpos = FILE_fn_strw_setpos;
   f->fn.fn_unget  = FILE_fn_fail_unget;

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

