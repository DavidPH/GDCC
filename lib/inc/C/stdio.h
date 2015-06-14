//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2015 David Hill
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
#define stderr (&__stderr)
#define stdin  (&__stdin)
#define stdout (&__stdout)

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
   int (*fn_close)(FILE *stream);
   int (*fn_fetch)(FILE *stream);
   int (*fn_flush)(FILE *stream, int c);
   int (*fn_getpos)(FILE *stream, fpos_t *pos);
   int (*fn_open)(FILE *stream, char const *filename, char const *mode);
   int (*fn_reopen)(FILE *stream, char const *filename, char const *mode);
   int (*fn_setbuf)(FILE *stream, char *buf, size_t size, int mode);
   int (*fn_setpos)(FILE *stream, fpos_t const *pos);
   int (*fn_unget)(FILE *stream, int c);
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


//----------------------------------------------------------------------------|
// Extern Variables                                                           |
//

//
// __stderr, __stdin, __stdout
//
#ifdef __cplusplus
extern "C" FILE __stderr, __stdin, __stdout;
#else
extern FILE __stderr, __stdin, __stdout;
#endif


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

#ifdef __cplusplus
extern "C" {
#endif

//
// Operations on files.
//

int remove(char const *filename);
int rename(char const *oldname, char const *newname);
FILE *tmpfile(void);
char *tmpnam(char *s);

//
// File access functions.
//

int fclose(FILE *stream);
int fflush(FILE *stream);
FILE *fopen(char const *restrict filename, char const *restrict mode);
FILE *freopen(char const *restrict filename, char const *restrict mode,
   FILE *restrict stream);
void setbuf(FILE *restrict stream, char *restrict buf);
int setvbuf(FILE *restrict stream, char *restrict buf, int mode, size_t size);

//
// Formatted input/output functions.
//

int fprintf(FILE *restrict stream, char const *restrict format, ...);
int fscanf(FILE *restrict stream, char const *restrict format, ...);
int printf(char const *restrict format, ...);
int scanf(char const *restrict format, ...);
int snprintf(char *restrict s, size_t n, char const *restrict format, ...);
int sprintf(char *restrict s, char const *restrict format, ...);
int sscanf(char const *restrict s, char const *restrict format, ...);
int vfprintf(FILE *restrict stream, char const *restrict format, __va_list arg);
int vfscanf(FILE *restrict stream, char const *restrict format, __va_list arg);
int vprintf(char const *restrict format, __va_list arg);
int vscanf(char const *restrict format, __va_list arg);
int vsnprintf(char *restrict s, size_t n, char const *restrict format, __va_list arg);
int vsprintf(char *restrict s, char const *restrict format, __va_list arg);
int vsscanf(char const *restrict s, char const *restrict format, __va_list arg);

//
// Character input/output functions.
//

int fgetc(FILE *stream);
char *fgets(char *restrict s, int n, FILE *restrict stream);
int fputc(int c, FILE *stream);
int fputs(char const *restrict s, FILE *restrict stream);
int getc(FILE *stream);
int getchar(void);
int putc(int c, FILE *stream);
int putchar(int c);
int puts(char const *s);
int ungetc(int c, FILE *stream);

//
// Direct input/output functions.
//

size_t fread(void *restrict ptr, size_t size, size_t nmemb,
   FILE *restrict stream);
size_t fwrite(void const *restrict ptr, size_t size, size_t nmemb,
   FILE *restrict stream);

//
// File positioning functions.
//

int fgetpos(FILE *restrict stream, fpos_t *restrict pos);
int fseek(FILE *stream, long int offset, int whence);
int fsetpos(FILE *stream, fpos_t const *pos);
long int ftell(FILE *stream);
void rewind(FILE *stream);

//
// Error-handling functions.
//

void clearerr(FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);
void perror(char const *s);

//
// Implementation extensions.
//

FILE *__fopen_fn(__FILE_fn const *fn, size_t size, void *data,
   char const *filename, char const *mode);
int __fprintf_str(FILE *restrict stream,
   char const __str_ars *restrict format, ...);
size_t __fwrite_str(void const __str_ars *restrict ptr, size_t size,
   size_t nmemb, FILE *restrict stream);
int __nprintf(char const *restrict format, ...);
int __nprintf_str(char const __str_ars *restrict format, ...);
int __printf_str(char const __str_ars *restrict format, ...);
int __vfprintf_str(FILE *restrict stream,
   char const __str_ars *restrict format, __va_list arg);
int __vnprintf(char const *restrict format, __va_list arg);
int __vnprintf_str(char const __str_ars *restrict format, __va_list arg);
int __vprintf_str(char const __str_ars *restrict format, __va_list arg);

#ifdef __cplusplus
}
#endif

#endif//__GDCC_Header__C__stdio_h__

