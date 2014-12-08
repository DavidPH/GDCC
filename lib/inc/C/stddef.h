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
// Common definitions.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__stddef_h__
#define __GDCC_Header__C__stddef_h__


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
// offsetof
//
#ifndef offsetof
#define offsetof(T, M) ((size_t)__offsetof(T, M))
#endif


//----------------------------------------------------------------------------|
// Types                                                                      |
//

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
// max_align_t
//
#ifndef __GDCC_Have__max_align_t__
#define __GDCC_Have__max_align_t__
typedef int max_align_t;
#endif

//
// wchar_t
//
#ifndef __GDCC_Have__wchar_t__
#define __GDCC_Have__wchar_t__
typedef unsigned wchar_t;
#endif


#endif//__GDCC_Header__C__stddef_h__

