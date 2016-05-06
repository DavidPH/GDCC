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
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__sys__types_h__
#define __GDCC_Header__C__sys__types_h__

#include <bits/types.h>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// clock_t
//
#ifndef __GDCC_Have__clock_t__
#define __GDCC_Have__clock_t__
typedef __clock_t clock_t;
#endif

//
// off_t
//
#ifndef __GDCC_Have__off_t__
#define __GDCC_Have__off_t__
typedef __off_t off_t;
#endif

//
// size_t
//
#ifndef __GDCC_Have__size_t__
#define __GDCC_Have__size_t__
typedef __size_t size_t;
#endif

//
// ssize_t
//
#ifndef __GDCC_Have__ssize_t__
#define __GDCC_Have__ssize_t__
typedef __ssize_t ssize_t;
#endif

//
// time_t
//
#ifndef __GDCC_Have__time_t__
#define __GDCC_Have__time_t__
typedef __time_t time_t;
#endif

#endif//__GDCC_Header__C__sys__types_h__

