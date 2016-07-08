//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// Input/output.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__stdio_h__
#define __GDCC_Header__C__stdio_h__

#include <bits/types.h>


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// NULL
//
#ifndef NULL
#define NULL 0
#endif

//
// _IO*
//
#define _IOFBF 1
#define _IOLBF 2
#define _IONBF 3

//
// BUFSIZ
//
#define BUFSIZ 256

//
// EOF
//
#define EOF (-1)

//
// FOPEN_MAX
//
#define FOPEN_MAX 8

//
// FILENAME_MAX
//
#define FILENAME_MAX 13

//
// L_tmpnam
//
#define L_tmpnam 13

//
// SEEK_*
//
#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 3

//
// TMP_MAX
//
#define TMP_MAX 25

//
// stderr, stdin, stdout
//
#ifdef __GDCC__DirectObject
#define stderr (&__stderr)
#define stdin  (&__stdin)
#define stdout (&__stdout)
#else
#define stderr (__get_stderr())
#define stdin  (__get_stdin())
#define stdout (__get_stdout())
#endif

//
// _FILEFLAG_*
//
#define _FILEFLAG_EOF 0x00000001
#define _FILEFLAG_ERR 0x00000002
#define _FILEFLAG_FRB 0x00000004
#define _FILEFLAG_FRC 0x00000008
#define _FILEFLAG_FRF 0x00000010
#define _FILEFLAG_LBF 0x00000020


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// size_t
//
#ifndef __GDCC_Have__size_t__
#define __GDCC_Have__size_t__
typedef __size_t size_t;
#endif

//
// FILE
//
#ifndef __GDCC_Have__FILE__
#define __GDCC_Have__FILE__
typedef struct __FILE FILE;
#endif

//
// fpos_t
//
typedef __off_t fpos_t;

//
// POSIX extensions.
//

#if _POSIX_C_SOURCE >= 200809L

//
// off_t
//
#ifndef __GDCC_Have__off_t__
#define __GDCC_Have__off_t__
typedef __off_t off_t;
#endif

//
// ssize_t
//
#ifndef __GDCC_Have__ssize_t__
#define __GDCC_Have__ssize_t__
typedef __ssize_t ssize_t;
#endif

#endif

//
// GNU extensions.
//

#if defined(_GNU_SOURCE)

typedef __cookie_read_function_t cookie_read_function_t;
typedef __cookie_write_function_t cookie_write_function_t;
typedef __cookie_seek_function_t cookie_seek_function_t;
typedef __cookie_close_function_t cookie_close_function_t;

typedef struct __cookie_io_functions_t cookie_io_functions_t;

#endif

//
// Implementation extensions.
//

//
// __cookie_io_functions_t
//
struct __cookie_io_functions_t
{
   __cookie_read_function_t *read;
   __cookie_write_function_t *write;
   __cookie_seek_function_t *seek;
   __cookie_close_function_t *close;
};

//
// __FILE_buf
//
typedef struct __FILE_buf
{
   char  *_buf;
   char  *_ptr;
   char  *_end;
   size_t _len;
} __FILE_buf;

//
// __FILE
//
typedef struct __FILE
{
   struct __cookie_io_functions_t _fn;

   __FILE_buf _get;
   __FILE_buf _put;

   void    *_cookie;
   unsigned _flag;
} __FILE;


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

//
// __stderr, __stdin, __stdout
//
extern FILE __stderr, __stdin, __stdout;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// Operations on files.
//

int remove(char const *_filename);
int rename(char const *_oldname, char const *_newname);
FILE *tmpfile(void);
char *tmpnam(char *_s);

//
// File access functions.
//

int fclose(FILE *_stream);
int fflush(FILE *_stream);
FILE *fopen(char const *restrict _filename, char const *restrict _mode);
FILE *freopen(char const *restrict _filename, char const *restrict _mode,
   FILE *restrict _stream);
void setbuf(FILE *restrict _stream, char *restrict _buf);
int setvbuf(FILE *restrict _stream, char *restrict _buf, int _mode, size_t _size);

//
// Formatted input/output functions.
//

int fprintf(FILE *restrict _stream, char const *restrict _format, ...);
int fscanf(FILE *restrict _stream, char const *restrict _format, ...);
int printf(char const *restrict _format, ...);
int scanf(char const *restrict _format, ...);
int snprintf(char *restrict _s, size_t _n, char const *restrict _format, ...);
int sprintf(char *restrict _s, char const *restrict _format, ...);
int sscanf(char const *restrict _s, char const *restrict _format, ...);
int vfprintf(FILE *restrict _stream, char const *restrict _format, __va_list _arg);
int vfscanf(FILE *restrict _stream, char const *restrict _format, __va_list _arg);
int vprintf(char const *restrict _format, __va_list _arg);
int vscanf(char const *restrict _format, __va_list _arg);
int vsnprintf(char *restrict _s, size_t _n, char const *restrict _format, __va_list _arg);
int vsprintf(char *restrict _s, char const *restrict _format, __va_list _arg);
int vsscanf(char const *restrict _s, char const *restrict _format, __va_list _arg);

//
// Character input/output functions.
//

int fgetc(FILE *_stream);
char *fgets(char *restrict _s, int _n, FILE *restrict _stream);
int fputc(int _c, FILE *_stream);
int fputs(char const *restrict _s, FILE *restrict _stream);
int getc(FILE *_stream);
int getchar(void);
int putc(int _c, FILE *_stream);
int putchar(int _c);
int puts(char const *_s);
int ungetc(int _c, FILE *_stream);

//
// Direct input/output functions.
//

size_t fread(void *restrict _ptr, size_t _size, size_t _nmemb,
   FILE *restrict _stream);
size_t fwrite(void const *restrict _ptr, size_t _size, size_t _nmemb,
   FILE *restrict _stream);

//
// File positioning functions.
//

int fgetpos(FILE *restrict _stream, fpos_t *restrict _pos);
int fseek(FILE *_stream, long int _offset, int _whence);
int fsetpos(FILE *_stream, fpos_t const *_pos);
long int ftell(FILE *_stream);
void rewind(FILE *_stream);

//
// Error-handling functions.
//

void clearerr(FILE *_stream);
int feof(FILE *_stream);
int ferror(FILE *_stream);
void perror(char const *_s);

//
// POSIX extensions.
//

#if _POSIX_C_SOURCE >= 200809L
FILE *fmemopen(void *_buf, size_t _size, char const *_mode);
FILE *open_memstream(char **_ptr, size_t *_sizeloc);
#endif

//
// GNU extensions.
//

#if defined(_GNU_SOURCE)
FILE *fopencookie(void *_cookie, char const *_mode, cookie_io_functions_t _io_funcs);
#endif

//
// Implementation extensions.
//

FILE *__fopencookie_ctor(FILE *_stream, void *_cookie, char const *_mode,
   struct __cookie_io_functions_t _io_funcs);
FILE *__fmemopen_str(char __str_ars const *_buf, size_t _size, char const *_mode);

FILE *__fmemopen_sta_r(char const *_buf, size_t _size);
FILE *__fmemopen_sta_r_str(char __str_ars const *_buf, size_t _size);
FILE *__fmemopen_sta_w(char *_buf, size_t _size);

int __fscanf_str(FILE *restrict _stream, char __str_ars const *restrict _format, ...);
int __scanf_str(char __str_ars const *restrict _format, ...);
int __sscanf_str(char const *restrict _s, char __str_ars const *restrict _format, ...);
int __strscanf(char __str_ars const *restrict _s, char const *restrict _format, ...);
int __strscanf_str(char __str_ars const *restrict _s, char __str_ars const *restrict _format, ...);
int __vfscanf_str(FILE *restrict _stream, char __str_ars const *restrict _format,
   __va_list _arg);
int __vscanf_str(char __str_ars const *restrict _format, __va_list _arg);
int __vsscanf_str(char const *restrict _s,
   char __str_ars const *restrict _format, __va_list _arg);
int __vstrscanf(char __str_ars const *restrict _s, char const *restrict _format,
   __va_list _arg);
int __vstrscanf_str(char __str_ars const *restrict _s,
   char __str_ars const *restrict _format, __va_list _arg);

int __fprintf_str(FILE *restrict _stream,
   char __str_ars const *restrict _format, ...);
int __nprintf(char const *restrict _format, ...);
int __nprintf_str(char __str_ars const *restrict _format, ...);
int __printf_str(char __str_ars const *restrict _format, ...);
int __snprintf_str(char *restrict _s, size_t _n, char __str_ars const *restrict _format, ...);
int __sprintf_str(char *restrict _s, char __str_ars const *restrict _format, ...);
int __vfprintf_str(FILE *restrict _stream,
   char __str_ars const *restrict _format, __va_list _arg);
int __vnprintf(char const *restrict _format, __va_list _arg);
int __vnprintf_str(char __str_ars const *restrict _format, __va_list _arg);
int __vprintf_str(char __str_ars const *restrict _format, __va_list _arg);
int __vsnprintf_str(char *restrict _s, size_t _n,
   char __str_ars const *restrict _format, __va_list _arg);
int __vsprintf_str(char *restrict _s, char __str_ars const *restrict _format, __va_list _arg);

size_t __fwrite_str(void __str_ars const *restrict _ptr, size_t _size,
   size_t _nmemb, FILE *restrict _stream);

FILE *__get_stderr(void);
FILE *__get_stdin(void);
FILE *__get_stdout(void);

#ifdef __cplusplus
}
#endif

#endif//__GDCC_Header__C__stdio_h__

