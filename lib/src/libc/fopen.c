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
#define _GNU_SOURCE

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
// Types                                                                      |
//

//
// __cookie_mem
//
typedef struct __cookie_mem
{
   char  *mem;
   size_t len;
   size_t pos;
} __cookie_mem;

//
// __cookie_mem_rea
//
typedef struct __cookie_mem_rea
{
   char  **mem_p;
   size_t *pos_p;

   char  *mem;
   size_t len;
   size_t pos;
} __cookie_mem_rea;

//
// __cookie_mem_str
//
typedef struct __cookie_mem_str
{
   char __str_ars *mem;
   size_t          len;
   size_t          pos;
} __cookie_mem_str;


//----------------------------------------------------------------------------|
// Static Prototypes                                                          |
//

static ssize_t FILE_fn_mem_read(void *cookie, char *buf, size_t size);
static ssize_t FILE_fn_mem_read_rea(void *cookie, char *buf, size_t size);
static ssize_t FILE_fn_mem_read_str(void *cookie, char *buf, size_t size);
static ssize_t FILE_fn_mem_write(void *cookie, char const *buf, size_t size);
static ssize_t FILE_fn_mem_write_rea(void *cookie, char const *buf, size_t size);
static ssize_t FILE_fn_mem_write_sta(void *cookie, char const *buf, size_t size);
static int     FILE_fn_mem_seek(void *cookie, off_t *offset, int whence);
static int     FILE_fn_mem_seek_rea(void *cookie, off_t *offset, int whence);
static int     FILE_fn_mem_seek_str(void *cookie, off_t *offset, int whence);

static ssize_t FILE_fn_stdout_write(void *cookie, char const *buf, size_t size);


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

[[no_init]]
static char Buffer_memfile[BUFSIZ];

[[no_init]]
static char Buffer_stderr[BUFSIZ];

[[no_init]]
static char Buffer_stdout[BUFSIZ];

[[no_init]]
static __cookie_mem Cookie_mem;

[[no_init]]
static __cookie_mem_str Cookie_mem_str;

static FILE __memfile;


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

//
// __stderr
//
FILE __stderr =
{
   {
      .read  = NULL,
      .write = FILE_fn_stdout_write,
      .seek  = NULL,
      .close = NULL,
   },

   {Buffer_stderr, Buffer_stderr, Buffer_stderr, 0},
   {Buffer_stderr, Buffer_stderr, Buffer_stderr + sizeof(Buffer_stderr), sizeof(Buffer_stderr)},

   NULL, _FILEFLAG_LBF
};

//
// __stdin
//
FILE __stdin =
{
   {
      .read  = NULL,
      .write = NULL,
      .seek  = NULL,
      .close = NULL,
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
      .read  = NULL,
      .write = FILE_fn_stdout_write,
      .seek  = NULL,
      .close = NULL,
   },

   {Buffer_stdout, Buffer_stdout, Buffer_stdout, 0},
   {Buffer_stdout, Buffer_stdout, Buffer_stdout + sizeof(Buffer_stdout), sizeof(Buffer_stdout)},

   NULL, _FILEFLAG_LBF
};


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//=========================================================
// memfile functions.
//

//
// FILE_fn_mem_read
//
static ssize_t FILE_fn_mem_read(void *cookie_, char *buf, size_t size)
{
   __cookie_mem *cookie = cookie_;
   size_t        avail  = cookie->len - cookie->pos;

   if(size > avail)
      size = avail;

   memcpy(buf, cookie->mem + cookie->pos, size);
   cookie->pos += size;
   return size;
}

//
// FILE_fn_mem_read_rea
//
static ssize_t FILE_fn_mem_read_rea(void *cookie_, char *buf, size_t size)
{
   __cookie_mem_rea *cookie = cookie_;
   size_t            avail  = cookie->len - cookie->pos;

   if(size > avail)
      size = avail;

   memcpy(buf, cookie->mem + cookie->pos, size);
   cookie->pos += size;
   return size;
}

//
// FILE_fn_mem_read_str
//
static ssize_t FILE_fn_mem_read_str(void *cookie_, char *buf, size_t size)
{
   __cookie_mem_str *cookie = cookie_;
   size_t            avail  = cookie->len - cookie->pos;

   if(size > avail)
      size = avail;

   ACS_StrArsCpyToGlobalCharRange((int)buf, __GDCC__Sta, 0, size, cookie->mem + cookie->pos);
   cookie->pos += size;
   return size;
}

//
// FILE_fn_mem_write
//
static ssize_t FILE_fn_mem_write(void *cookie_, char const *buf, size_t size)
{
   __cookie_mem *cookie = cookie_;
   size_t        avail  = cookie->len - cookie->pos;

   if(size >= avail)
   {
      memcpy(cookie->mem + cookie->pos, buf, avail);
      cookie->pos = cookie->len;
      return avail;
   }
   else
   {
      memcpy(cookie->mem + cookie->pos, buf, size);
      cookie->mem[cookie->pos += size] = '\0';
      return size;
   }
}

//
// FILE_fn_mem_write_rea
//
static ssize_t FILE_fn_mem_write(void *cookie_, char const *buf, size_t size)
{
   __cookie_mem_rea *cookie = cookie_;
   size_t            avail  = cookie->len - cookie->pos;

   if(size >= avail)
   {
      // TODO: Check for overflow.
      size_t len = cookie->len + cookie->len / 2 + size + 1;
      char *mem = realloc(cookie->mem, len);

      if(!mem)
         return 0;

      cookie->len = len;
      cookie->mem = mem;
   }

   memcpy(cookie->mem + cookie->pos, buf, size);
   cookie->mem[cookie->pos += size] = '\0';
   *cookie->mem_p = cookie->mem;
   *cookie->pos_p = cookie->pos;
   return size;
}

//
// FILE_fn_mem_write_sta
//
static ssize_t FILE_fn_mem_write_sta(void *cookie_, char const *buf, size_t size)
{
   __cookie_mem *cookie = cookie_;
   size_t        avail  = cookie->len - cookie->pos;

   if(size >= avail)
   {
      memcpy(cookie->mem + cookie->pos, buf, avail);
      cookie->pos = cookie->len;
      return size; // Silently ignore unwritten data.
   }
   else
   {
      memcpy(cookie->mem + cookie->pos, buf, size);
      cookie->mem[cookie->pos += size] = '\0';
      return size;
   }
}

//
// FILE_fn_mem_seek
//
static int FILE_fn_mem_seek(void *cookie_, off_t *offset, int whence)
{
   __cookie_mem *cookie = cookie_;
   size_t        pos;

   switch(whence)
   {
   case SEEK_SET: pos = *offset; break;
   case SEEK_CUR: pos = *offset + cookie->pos; break;
   case SEEK_END: pos = *offset + cookie->len; break;
   default: return EOF;
   }

   if(pos > cookie->len)
      return EOF;

   *offset = cookie->pos = pos;

   return 0;
}

//
// FILE_fn_mem_seek_rea
//
static int FILE_fn_mem_seek_rea(void *cookie_, off_t *offset, int whence)
{
   __cookie_mem_rea *cookie = cookie_;
   size_t            pos;

   switch(whence)
   {
   case SEEK_SET: pos = *offset; break;
   case SEEK_CUR: pos = *offset + cookie->pos; break;
   case SEEK_END: pos = *offset + cookie->len; break;
   default: return EOF;
   }

   if(pos > cookie->len)
      return EOF;

   *offset = cookie->pos = pos;

   return 0;
}

//
// FILE_fn_mem_seek_str
//
static int FILE_fn_mem_seek_str(void *cookie_, off_t *offset, int whence)
{
   __cookie_mem_str *cookie = cookie_;
   size_t            pos;

   switch(whence)
   {
   case SEEK_SET: pos = *offset; break;
   case SEEK_CUR: pos = *offset + cookie->pos; break;
   case SEEK_END: pos = *offset + cookie->len; break;
   default: return EOF;
   }

   if(pos > cookie->len)
      return EOF;

   *offset = cookie->pos = pos;

   return 0;
}

//=========================================================
// stdout functions.
//

//
// FILE_fn_stdout_write
//
static ssize_t FILE_fn_stdout_write(void *cookie, char const *buf, size_t size)
{
   #if __GDCC_Family__ZDACS__
   ACS_BeginPrint();
   if(buf[size - 1] == '\n')
      ACS_PrintGlobalCharRange((int)buf, __GDCC__Sta, 0, size - 1);
   else
      ACS_PrintGlobalCharRange((int)buf, __GDCC__Sta, 0, size);
   ACS_EndLog();

   return size;
   #else
   return 0;
   #endif
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

   if(fflush(stream) == EOF)
      res = EOF;

   if(stream->_fn.close && stream->_fn.close(stream->_cookie) == EOF)
      res = EOF;

   if(stream->_flag & _FILEFLAG_FRB)
      free(stream->_get._buf);

   if(stream->_flag & _FILEFLAG_FRC)
      free(stream->_cookie);

   if(stream->_flag & _FILEFLAG_FRF)
      free(stream);

   return res;
}

//
// fflush
//
int fflush(FILE *stream)
{
   size_t len;

   if(stream->_flag & _FILEFLAG_ERR)
      return EOF;

   // Flush get buffer by ungetting read buffer.
   len = stream->_get._end - stream->_get._ptr;
   if(len && stream->_fn.read)
   {
      if(!stream->_fn.seek)
         return stream->_flag |= _FILEFLAG_ERR, EOF;

      off_t   off = -(off_t)len;
      ssize_t res = stream->_fn.seek(stream->_cookie, &off, SEEK_CUR);
      if(res == EOF) return stream->_flag |= _FILEFLAG_ERR, EOF;
      stream->_get._end = stream->_get._ptr;
   }

   // Flush put buffer by writing it.
   len = stream->_put._ptr - stream->_put._buf;
   if(len && stream->_fn.write)
   {
      ssize_t res = stream->_fn.write(stream->_cookie, stream->_put._buf, len);
      if(res != len) return stream->_flag |= _FILEFLAG_ERR, EOF;
      stream->_put._ptr = stream->_put._buf;
   }

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
   if(mode == _IOFBF || mode == _IOLBF)
   {
      // TODO: Allocate buffer.
      if(!buf)
         return EOF;

      // Fuck off.
      if(size < 4)
         return EOF;
   }
   else if(mode == _IONBF)
   {
      // Discard passed buffer data for unbuffered mode.
      buf  = NULL;
      size = 0;
   }
   else
   {
      // Invalid mode.
      errno = EINVAL;
      return EOF;
   }

   size_t getUsed = stream->_get._end - stream->_get._ptr;
   size_t putUsed = stream->_put._ptr - stream->_put._buf;

   // Divide buffer as needed.
   size_t getSize, putSize;
   if(stream->_fn.write)
   {
      if(stream->_fn.read)
      {
         getSize = size / 2;
         putSize = size - getSize;
      }
      else
      {
         getSize = 0;
         putSize = size;
      }
   }
   else
   {
      if(stream->_fn.read)
      {
         getSize = size;
         putSize = 0;
      }
      else
      {
         return EOF;
      }
   }

   // Line-buffered writes need a reserved byte.
   if(mode == _IOLBF && stream->_fn.write)
      --putSize;

   // Check that new buffer large enough for existing contents.
   if(getUsed > getSize || putUsed > putSize)
      return EOF;

   // TODO: Allocate new buffer, if needed.

   // Transfer buffer contents.
   memcpy(buf,           stream->_get._ptr, getUsed);
   memcpy(buf + getSize, stream->_put._buf, putUsed);

   // TODO: Free old buffer, if needed.

   // Set buffers.
   stream->_get._buf = buf;
   stream->_get._ptr = buf;
   stream->_get._end = buf + getUsed;
   stream->_get._len = getSize;
   stream->_put._buf = buf + getSize;
   stream->_put._ptr = buf + getSize + putUsed;
   stream->_put._end = buf + getSize + putSize;
   stream->_put._len = putSize;

   // Set linebuffer flag.
   if(mode == _IOLBF)
      stream->_flag |= _FILEFLAG_LBF;
   else
      stream->_flag &= ~_FILEFLAG_LBF;

   return 0;
}

//=========================================================
// POSIX extensions.
//

//
// fmemopen
//
FILE *fmemopen(void *buf, size_t size, char const *mode)
{
   cookie_io_functions_t io_funcs =
   {
      .read  = FILE_fn_mem_read,
      .write = FILE_fn_mem_write,
      .seek  = FILE_fn_mem_seek,
      .close = NULL,
   };

   FILE *stream = malloc(sizeof(FILE) + BUFSIZ + sizeof(__cookie_mem));
   if(!stream) return NULL;

   char         *buffer = (char *)(stream + 1);
   __cookie_mem *cookie = (__cookie_mem *)(buffer + BUFSIZ);

   cookie->mem = buf;
   cookie->len = size;
   cookie->pos = 0;

   if(!__fopencookie_ctor(stream, cookie, mode, io_funcs))
      return free(stream), NULL;

   stream->_flag |= _FILEFLAG_FRF;

   setvbuf(stream, buffer, _IOFBF, BUFSIZ);

   return stream;
}

//
// open_memstream
//
FILE *open_memstream(char **ptr, size_t *sizeloc)
{
   cookie_io_functions_t io_funcs =
   {
      .read  = FILE_fn_mem_read_rea,
      .write = FILE_fn_mem_write_rea,
      .seek  = FILE_fn_mem_seek_rea,
      .close = NULL,
   };

   FILE *stream = malloc(sizeof(FILE) + BUFSIZ + sizeof(__cookie_mem_rea));
   if(!stream) return NULL;

   char             *buffer = (char *)(stream + 1);
   __cookie_mem_rea *cookie = (__cookie_mem_rea *)(buffer + BUFSIZ);

   *(cookie->mem_p = ptr)     = NULL;
   *(cookie->pos_p = sizeloc) = 0;

   cookie->mem = NULL;
   cookie->len = 0;
   cookie->pos = 0;

   if(!__fopencookie_ctor(stream, cookie, mode, io_funcs))
      return free(stream), NULL;

   stream->_flag |= _FILEFLAG_FRF;

   setvbuf(stream, buffer, _IOFBF, BUFSIZ);

   return stream;
}

//=========================================================
// GNU extensions.
//

//
// fopencookie
//
FILE *fopencookie(void *cookie, char const *mode, cookie_io_functions_t io_funcs)
{
   FILE *stream = malloc(sizeof(FILE) + BUFSIZ);
   if(!stream) return NULL;

   if(!__fopencookie_ctor(stream, cookie, mode, io_funcs))
      return free(stream), NULL;

   stream->_flag |= _FILEFLAG_FRF;

   setvbuf(stream, (char *)(stream + 1), _IOFBF, BUFSIZ);

   return stream;
}

//=========================================================
// Implementation extensions.
//

//
// __fopencookie_ctor
//
FILE *__fopencookie_ctor(FILE *stream, void *cookie, char const *mode,
   cookie_io_functions_t io_funcs)
{
   // Init members.
   stream->_fn = io_funcs;

   stream->_get._buf = NULL;
   stream->_get._ptr = NULL;
   stream->_get._end = NULL;
   stream->_get._len = 0;

   stream->_put._buf = NULL;
   stream->_put._ptr = NULL;
   stream->_put._end = NULL;
   stream->_put._len = 0;

   stream->_cookie = cookie;

   stream->_flag = 0;

   // Parse mode string.
   char modeBase = *mode++;

   // Ignore binary mode flag.
   if(*mode == 'b') ++mode;

   _Bool modePlus = *mode == '+';
   if(modePlus)
   {
      ++mode;
      // TODO
      return NULL;
   }

   // Must be at end of mode string.
   if(*mode)
   {
      errno = EINVAL;
      return NULL;
   }

   switch(modeBase)
   {
   case 'r':
      if(!modePlus)
         stream->_fn.write = NULL;
      break;

   case 'w':
      if(!modePlus)
         stream->_fn.read = NULL;
      break;

   case 'a':
      if(!modePlus)
         stream->_fn.read = NULL;

      // TODO: Seek to stream end.
      return NULL;

   default:
      return NULL;
   }

   return stream;
}

//
// __fmemopen_sta_r
//
FILE *__fmemopen_sta_r(char const *buf, size_t size)
{
   Cookie_mem.mem = (char *)buf;
   Cookie_mem.len = size;
   Cookie_mem.pos = 0;

   __memfile._fn.read  = FILE_fn_mem_read;
   __memfile._fn.write = NULL;
   __memfile._fn.seek  = FILE_fn_mem_seek;
   __memfile._fn.close = NULL;

   __memfile._get._buf = Buffer_memfile;
   __memfile._get._ptr = Buffer_memfile;
   __memfile._get._end = Buffer_memfile;
   __memfile._get._len = sizeof(Buffer_memfile);

   __memfile._put._buf = Buffer_memfile + sizeof(Buffer_memfile);
   __memfile._put._ptr = Buffer_memfile + sizeof(Buffer_memfile);
   __memfile._put._end = Buffer_memfile + sizeof(Buffer_memfile);
   __memfile._put._len = 0;

   __memfile._cookie = &Cookie_mem;

   __memfile._flag = 0;

   return &__memfile;
}

//
// __fmemopen_sta_r_str
//
FILE *__fmemopen_sta_r_str(char __str_ars const *buf, size_t size)
{
   Cookie_mem_str.mem = (char __str_ars *)buf;
   Cookie_mem_str.len = size;
   Cookie_mem_str.pos = 0;

   __memfile._fn.read  = FILE_fn_mem_read_str;
   __memfile._fn.write = NULL;
   __memfile._fn.seek  = FILE_fn_mem_seek_str;
   __memfile._fn.close = NULL;

   __memfile._get._buf = Buffer_memfile;
   __memfile._get._ptr = Buffer_memfile;
   __memfile._get._end = Buffer_memfile;
   __memfile._get._len = sizeof(Buffer_memfile);

   __memfile._put._buf = Buffer_memfile + sizeof(Buffer_memfile);
   __memfile._put._ptr = Buffer_memfile + sizeof(Buffer_memfile);
   __memfile._put._end = Buffer_memfile + sizeof(Buffer_memfile);
   __memfile._put._len = 0;

   __memfile._cookie = &Cookie_mem_str;

   __memfile._flag = 0;

   return &__memfile;
}

//
// __fmemopen_sta_w
//
FILE *__fmemopen_sta_w(char *buf, size_t size)
{
   Cookie_mem.mem = buf;
   Cookie_mem.pos = 0;
   Cookie_mem.len = size;

   __memfile._fn.read  = NULL;
   __memfile._fn.write = FILE_fn_mem_write_sta;
   __memfile._fn.seek  = FILE_fn_mem_seek;
   __memfile._fn.close = NULL;

   __memfile._get._buf = Buffer_memfile;
   __memfile._get._ptr = Buffer_memfile;
   __memfile._get._end = Buffer_memfile;
   __memfile._get._len = 0;

   __memfile._put._buf = Buffer_memfile;
   __memfile._put._ptr = Buffer_memfile;
   __memfile._put._end = Buffer_memfile + sizeof(Buffer_memfile);
   __memfile._put._len = sizeof(Buffer_memfile);

   __memfile._cookie = &Cookie_mem;

   __memfile._flag = 0;

   return &__memfile;
}

//
// __fmemopen_str
//
FILE *__fmemopen_str(char __str_ars *buf, size_t size, char const *mode)
{
   cookie_io_functions_t io_funcs =
   {
      .read  = FILE_fn_mem_read_str,
      .write = NULL,
      .seek  = FILE_fn_mem_seek_str,
      .close = NULL,
   };

   FILE *stream = malloc(sizeof(FILE) + BUFSIZ + sizeof(__cookie_mem_str));
   if(!stream) return NULL;

   char             *buffer = (char *)(stream + 1);
   __cookie_mem_str *cookie = (__cookie_mem_str *)(buffer + BUFSIZ);

   cookie->mem = buf;
   cookie->len = size;
   cookie->pos = 0;

   if(!__fopencookie_ctor(stream, cookie, mode, io_funcs))
      return free(stream), NULL;

   stream->_flag |= _FILEFLAG_FRF;

   setvbuf(stream, buffer, _IOFBF, BUFSIZ);

   return stream;
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

