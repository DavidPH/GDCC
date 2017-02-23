//-----------------------------------------------------------------------------
//
// Copyright(C) 2016-2017 David Hill
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
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <GDCC.h>

#if __GDCC_Family__ZDACS__
#include <ACS_ZDoom.h>
#endif

#if __GDCC_Target__Doominati__
#include <Doominati.h>
#endif


//----------------------------------------------------------------------------|
// Static Prototypes                                                          |
//

static ssize_t FILE_fn_stdout_write(void *cookie, char const *buf, size_t size);


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
   #elif __GDCC_Target__Doominati__
   return DGE_SysWrite(buf, size);
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
   bool alloc = false;

   if(mode == _IOFBF || mode == _IOLBF)
   {
      // Allocate buffer.
      if(!buf)
         alloc = true;

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

   // Allocate new buffer, if needed.
   if(alloc)
   {
      if(!(buf = malloc(size)))
         return EOF;
   }

   // Transfer buffer contents.
   memcpy(buf,           stream->_get._ptr, getUsed);
   memcpy(buf + getSize, stream->_put._buf, putUsed);

   // Free old buffer, if needed.
   if(stream->_flag & _FILEFLAG_FRB)
   {
      free(stream->_get._buf);
      stream->_flag &= ~_FILEFLAG_FRB;
   }

   // Set buffers.
   stream->_get._buf = buf;
   stream->_get._ptr = buf;
   stream->_get._end = buf + getUsed;
   stream->_get._len = getSize;
   stream->_put._buf = buf + getSize;
   stream->_put._ptr = buf + getSize + putUsed;
   stream->_put._end = buf + getSize + putSize;
   stream->_put._len = putSize;

   // Set free buffer flag.
   if(alloc)
      stream->_flag |= _FILEFLAG_FRB;

   // Set linebuffer flag.
   if(mode == _IOLBF)
      stream->_flag |= _FILEFLAG_LBF;
   else
      stream->_flag &= ~_FILEFLAG_LBF;

   return 0;
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

   // Check + mode flag.
   _Bool modePlus = *mode == '+';
   if(modePlus)
      ++mode;

   // Must be at end of mode string.
   if(*mode)
      return errno = EINVAL, NULL;

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

      // Seek to stream end.

      if(!stream->_fn.seek)
         return errno = EINVAL, NULL;

      if(stream->_fn.seek(stream->_cookie, &(off_t){0}, SEEK_END) == -1)
         return NULL;

      break;

   default:
      return NULL;
   }

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

