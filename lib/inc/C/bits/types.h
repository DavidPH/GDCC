//-----------------------------------------------------------------------------
//
// Copyright(C) 2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// This is an internal header and should not be included directly.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__bits__types_h__
#define __GDCC_Header__C__bits__types_h__

#include <bits/features.h>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

typedef unsigned int __clock_t;
typedef long long __intmax_t;
typedef signed int __intptr_t;
typedef int __max_align_t;
typedef long int __off_t;
typedef int __ptrdiff_t;
typedef unsigned int __size_t;
typedef int __ssize_t;
typedef long int __time_t;
typedef unsigned long long __uintmax_t;
typedef unsigned int __uintptr_t;
typedef unsigned int __uptrdiff_t;
typedef unsigned int __wchar_t;
typedef int __wint_t;


typedef __ssize_t __cookie_read_function_t(void *, char *, __size_t);
typedef __ssize_t __cookie_write_function_t(void *, char const *, __size_t);
typedef int __cookie_seek_function_t(void *, __off_t *, int);
typedef int __cookie_close_function_t(void *);

struct __FILE;
struct __cookie_io_functions_t;
struct __mbstate_t;

#endif//__GDCC_Header__C__bits__types_h__

