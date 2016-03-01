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

//
// __fpostol
//
#define __fpostol(pos) ((long int)*(pos))

//
// __ltofpos
//
#define __ltofpos(pos, l) ((void)(*(pos) = (fpos_t)(long int)(l)))


//----------------------------------------------------------------------------|
// Types                                                                      |
//

struct __FILE;

//
// size_t
//
#ifndef __GDCC_Have__size_t__
#define __GDCC_Have__size_t__
typedef unsigned size_t;
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
typedef unsigned long int fpos_t;

//
// __FILE_buf
//
typedef struct __FILE_buf
{
   char *buf_beg;
   char *buf_ptr;
   char *buf_end;
   int   buf_mode;
} __FILE_buf;

//
// __FILE_fn
//
typedef struct __FILE_fn
{
   int (*fn_close)(FILE *_stream);
   int (*fn_fetch)(FILE *_stream);
   int (*fn_flush)(FILE *_stream, int _c);
   int (*fn_getpos)(FILE *_stream, fpos_t *_pos);
   int (*fn_open)(FILE *_stream, char const *_filename, char const *_mode);
   int (*fn_reopen)(FILE *_stream, char const *_filename, char const *_mode);
   int (*fn_setbuf)(FILE *_stream, char *_buf, size_t _size, int _mode);
   int (*fn_setpos)(FILE *_stream, fpos_t const *_pos);
   int (*fn_unget)(FILE *_stream, int _c);
} __FILE_fn;

//
// __FILE
//
typedef struct __FILE
{
   __FILE_fn fn;

   __FILE_buf buf_get;
   __FILE_buf buf_put;

   void    *data;
   unsigned flags;
} __FILE;

//
// __FILE_buf_str
//
typedef struct __FILE_buf_str
{
   char __str_ars *buf_beg;
   char __str_ars *buf_ptr;
   char __str_ars *buf_end;
   int             buf_mode;
} __FILE_buf_str;

//
// __FILE_str
//
typedef struct __FILE_str
{
   FILE f;

   __FILE_buf_str buf_get;
} __FILE_str;


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

//
// __stderr, __stdin, __stdout
//
extern FILE __stderr, __stdin, __stdout;

//
// __strfiler, __strfiler_str, __strfilew
//
// Used internally for sprintf/sscanf.
//
extern FILE       __strfiler;
extern __FILE_str __strfiler_str;
extern FILE       __strfilew;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// Operations on files.
//

int remove(char const *_filename);
int rename(char const *oldname, char const *_newname);
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
// Implementation extensions.
//

FILE *__fopen_fn(__FILE_fn const *_fn, size_t _size, void *_data,
   char const *_filename, char const *_mode);
FILE *__stropenr(char const *_str, size_t _size);
FILE *__stropenr_sta(char const *_str, size_t _size);
FILE *__stropenr_str(char __str_ars const *_str, size_t _size);
FILE *__stropenr_str_sta(char __str_ars const *_str, size_t _size);
FILE *__stropenw(char *_str, size_t _size);
FILE *__stropenw_sta(char *_str, size_t _size);

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

