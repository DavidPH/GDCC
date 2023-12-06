//-----------------------------------------------------------------------------
//
// Copyright(C) 2016-2023 David Hill
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

#if __GDCC_Engine__Doominati__
#include <Doominati.h>
#endif


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// __cookie_file
//
typedef struct __cookie_file
{
   #if __GDCC_Family__ZDACS__
   unsigned int fd;
   unsigned int pos;
   #elif __GDCC_Engine__Doominati__
   unsigned int  fd;
   unsigned long pos;
   #else
   int pos;
   #endif
} __cookie_file;


//----------------------------------------------------------------------------|
// Static Prototypes                                                          |
//

static int     FILE_fn_file_close(void *cookie);
static ssize_t FILE_fn_file_read(void *cookie, char *buf, size_t size);
static ssize_t FILE_fn_file_write(void *cookie, char const *buf, size_t size);
static int     FILE_fn_file_seek(void *cookie, off_t *offset, int whence);

static ssize_t FILE_fn_stdin_read(void *cookie, char *buf, size_t size);

static ssize_t FILE_fn_stdout_write(void *cookie, char const *buf, size_t size);


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

[[no_init]]
static char Buffer_stderr[BUFSIZ];

[[no_init]]
static char Buffer_stdin[BUFSIZ];

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
      .read  = FILE_fn_stdin_read,
      .write = NULL,
      .seek  = NULL,
      .close = NULL,
   },

   {Buffer_stdin, Buffer_stdin, Buffer_stdin, 0},
   {Buffer_stdin, Buffer_stdin, Buffer_stdin + sizeof(Buffer_stdin), sizeof(Buffer_stdin)},

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
// file functions
//

//
// FILE_fn_file_close
//
static int FILE_fn_file_close(void *cookie_)
{
   __cookie_file *cookie = cookie_;

   #if __GDCC_Family__ZDACS__
   ACS_LumpClose(cookie->fd);
   #elif __GDCC_Engine__Doominati__
   DGE_File_Close(cookie->fd);
   #endif

   return 0;
}

//
// FILE_fn_file_read
//
static ssize_t FILE_fn_file_read(void *cookie_, char *buf, size_t size)
{
   __cookie_file *cookie = cookie_;
   int            read;

   #if __GDCC_Family__ZDACS__
   read = ACS_LumpReadGlobalCharArray(cookie->fd, cookie->pos, __GDCC__Sta, (int)buf, size);
   #elif __GDCC_Engine__Doominati__
   read = DGE_File_Read(cookie->fd, cookie->pos, buf, size);
   #else
   read = -1;
   #endif

   if(read < 0)
      return EOF;

   cookie->pos += read;
   return read;
}

//
// FILE_fn_file_write
//
static ssize_t FILE_fn_file_write(void *cookie_, char const *buf, size_t size)
{
   __cookie_file *cookie = cookie_;

   #if __GDCC_Engine__Doominati__
   int write = DGE_File_Write(cookie->fd, cookie->pos, buf, size);

   if(write < 0)
      return 0;

   cookie->pos += write;
   return write;
   #else
   return 0;
   #endif
}

//
// FILE_fn_file_seek
//
static int FILE_fn_file_seek(void *cookie_, off_t *offset, int whence)
{
   __cookie_file *cookie = cookie_;

   switch(whence)
   {
   case SEEK_END:
      #if __GDCC_Family__ZDACS__
      *offset = cookie->pos = ACS_LumpGetInfo(cookie->fd, LUMP_INFO_SIZE) - *offset;
      #elif __GDCC_Engine__Doominati__
      *offset = cookie->pos = DGE_File_Size(cookie->fd) - *offset;
      #endif
      break;

   case SEEK_CUR:
      *offset = cookie->pos += *offset;
      break;

   case SEEK_SET:
      cookie->pos = *offset;
      break;
   }

   return 0;
}

//=========================================================
// stdin functions.
//

//
// FILE_fn_stdin_read
//
static ssize_t FILE_fn_stdin_read(void *cookie, char *buf, size_t size)
{
   #if __GDCC_Engine__Doominati__
   int res;
   while((res = DGE_Shell_ReadStd(0, buf, size)) == 0)
      DGE_Task_Sleep(0, 1);
   return res;
   #else
   return 0;
   #endif
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
   #elif __GDCC_Engine__Doominati__
   return DGE_Shell_WriteStd(0, buf, size);
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
   cookie_io_functions_t io_funcs =
   {
      .read  = FILE_fn_file_read,
      .write = FILE_fn_file_write,
      .seek  = FILE_fn_file_seek,
      .close = FILE_fn_file_close,
   };

   int fd;

   #if __GDCC_Family__ZDACS__
   #define FD_CLOSE_FUNC ACS_LumpClose

   ACS_BeginStrParam();
   ACS_PrintGlobalCharArray((int)filename, __GDCC__Sta);
   __str filename_str = ACS_EndStrParam();

   if(*mode == 'r')
   {
      if((fd = ACS_LumpOpen(filename_str, -1, LUMP_OPEN_FULLPATH)) < 0)
         return NULL;
   }
   else
      return NULL;

   #elif __GDCC_Engine__Doominati__
   #define FD_CLOSE_FUNC DGE_File_Close

   if(*mode == 'r')
   {
      if((fd = DGE_File_Open(filename)) == -1)
         return NULL;
   }
   else if(*mode == 'w')
   {
      if((fd = DGE_File_Create(filename)) == -1)
         return NULL;

      if(DGE_File_Trunc(fd, 0) == -1)
         return DGE_File_Close(fd), NULL;
   }
   else if(*mode == 'a')
   {
      if((fd = DGE_File_Create(filename)) == -1)
         return NULL;
   }
   else
      return NULL;

   #else
   #define FD_CLOSE_FUNC

   return NULL;
   #endif

   FILE *stream = malloc(sizeof(FILE) + sizeof(__cookie_file) + BUFSIZ);
   if(!stream) return FD_CLOSE_FUNC(fd), NULL;

   __cookie_file *cookie = (__cookie_file *)(stream + 1);
   char          *buffer = (char *)(cookie + 1);

   cookie->fd  = fd;
   cookie->pos = 0;

   if(!__fopencookie_ctor(stream, cookie, mode, io_funcs))
      return free(stream), FD_CLOSE_FUNC(fd), NULL;

   stream->_flag |= _FILEFLAG_FRF;

   setvbuf(stream, buffer, _IOFBF, BUFSIZ);

   return stream;

   #undef FD_CLOSE_FUNC
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

