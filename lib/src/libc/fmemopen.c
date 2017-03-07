//-----------------------------------------------------------------------------
//
// Copyright(C) 2017 David Hill
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

static int     FILE_fn_mem_close_rea(void *cookie);
static ssize_t FILE_fn_mem_read(void *cookie, char *buf, size_t size);
static ssize_t FILE_fn_mem_read_rea(void *cookie, char *buf, size_t size);
static ssize_t FILE_fn_mem_read_str(void *cookie, char *buf, size_t size);
static ssize_t FILE_fn_mem_write(void *cookie, char const *buf, size_t size);
static ssize_t FILE_fn_mem_write_rea(void *cookie, char const *buf, size_t size);
static ssize_t FILE_fn_mem_write_sta(void *cookie, char const *buf, size_t size);
static int     FILE_fn_mem_seek(void *cookie, off_t *offset, int whence);
static int     FILE_fn_mem_seek_rea(void *cookie, off_t *offset, int whence);
static int     FILE_fn_mem_seek_str(void *cookie, off_t *offset, int whence);


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

[[no_init]]
static char Buffer_memfile[BUFSIZ];

[[no_init]]
static __cookie_mem Cookie_mem;

[[no_init]]
static __cookie_mem_str Cookie_mem_str;

static FILE __memfile;


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//=========================================================
// memfile functions.
//

//
// FILE_fn_mem_close_rea
//
static int FILE_fn_mem_close_rea(void *cookie_)
{
   __cookie_mem_rea *cookie = cookie_;

   free(cookie->mem);

   return 0;
}

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

   #if __GDCC_Family__ZDACS__
   ACS_StrArsCpyToGlobalCharRange((int)buf, __GDCC__Sta, 0, size, cookie->mem + cookie->pos);
   #else
   // TODO
   #endif
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
static ssize_t FILE_fn_mem_write_rea(void *cookie_, char const *buf, size_t size)
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


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

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
      .close = FILE_fn_mem_close_rea,
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

   if(!__fopencookie_ctor(stream, cookie, "w+", io_funcs))
      return free(stream), NULL;

   stream->_flag |= _FILEFLAG_FRF;

   setvbuf(stream, buffer, _IOFBF, BUFSIZ);

   return stream;
}

//=========================================================
// Implementation extensions.
//

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

// EOF

