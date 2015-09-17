//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2015 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libGDCC.
//
// This header declares functions, objects, and types which are specific and
// possibly internal to GDCC. Neither the contents or existence of this file
// should be relied upon by external projects.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__GDCC_h__
#define __GDCC_Header__C__GDCC_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// __GDCC__Sta
//
#define __GDCC__Sta __glyph(int, "___GDCC__Sta")


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// intmax_t
//
#ifndef __GDCC_Have__intmax_t__
#define __GDCC_Have__intmax_t__
typedef long long intmax_t;
#endif

//
// intptr_t
//
#ifndef __GDCC_Have__intptr_t__
#define __GDCC_Have__intptr_t__
typedef signed int intptr_t;
#endif

//
// ptrdiff_t
//
#ifndef __GDCC_Have__ptrdiff_t__
#define __GDCC_Have__ptrdiff_t__
typedef int ptrdiff_t;
#endif

//
// size_t
//
#ifndef __GDCC_Have__size_t__
#define __GDCC_Have__size_t__
typedef unsigned size_t;
#endif

//
// ssize_t
//
#ifndef __GDCC_Have__ssize_t__
#define __GDCC_Have__ssize_t__
typedef int ssize_t;
#endif

//
// uintmax_t
//
#ifndef __GDCC_Have__uintmax_t__
#define __GDCC_Have__uintmax_t__
typedef unsigned long long uintmax_t;
#endif

//
// uintptr_t
//
#ifndef __GDCC_Have__uintptr_t__
#define __GDCC_Have__uintptr_t__
typedef unsigned int uintptr_t;
#endif

//
// uptrdiff_t
//
#ifndef __GDCC_Have__uptrdiff_t__
#define __GDCC_Have__uptrdiff_t__
typedef unsigned uptrdiff_t;
#endif


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

#ifdef __cplusplus
extern "C" {
#endif

[[call("StkCall")]]
extern void __sta *__GDCC__alloc(void __sta *_ptr, size_t _size);

[[call("StkCall")]]
extern void __GDCC__alloc_dump(void);

#ifdef __cplusplus
}
#endif

#endif//__GDCC_Header__C__GDCC_h__

