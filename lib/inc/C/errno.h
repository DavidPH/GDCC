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
// Errors.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__errno_h__
#define __GDCC_Header__C__errno_h__

#include <bits/features.h>


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// errno
//
#ifdef __GDCC__DirectObject
#define errno __errno
#else
#define errno (*__get_errno())
#endif

//
// E*
//
#define EBADF  1
#define EDOM   2
#define EILSEQ 3
#define EINVAL 4
#define ERANGE 5


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

//
// __errno
//
extern int __errno;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

int *__get_errno(void);

#ifdef __cplusplus
}
#endif

#endif//__GDCC_Header__C__errno_h__

