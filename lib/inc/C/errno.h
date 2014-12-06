//-----------------------------------------------------------------------------
//
// Copyright(C) 2014 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// Errors.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__errno_h__
#define __GDCC_Header__C__errno_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// errno
//
#define errno __errno

//
// E*
//
#define EBADF  1
#define EDOM   2
#define EILSEQ 3
#define ERANGE 4


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

//
// __errno
//
#ifdef __cplusplus
extern "C" int __errno;
#else
extern int __errno;
#endif

#endif//__GDCC_Header__C__errno_h__

