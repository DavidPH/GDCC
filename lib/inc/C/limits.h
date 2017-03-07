//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2017 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// Sizes of integer types.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__limits_h__
#define __GDCC_Header__C__limits_h__

#include <bits/features.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// CHAR_BIT
//
#if __GDCC_Family__ZDACS__
#define CHAR_BIT 32
#else
#define CHAR_BIT 8
#endif

//
// CHAR_*
//
#if __GDCC_Family__ZDACS__
# define SCHAR_MIN (-0x7FFFFFFF-1)
# define SCHAR_MAX ( 0x7FFFFFFF  )
# define UCHAR_MAX ( 0xFFFFFFFFU )
#else
# define SCHAR_MIN (-0x7F-1)
# define SCHAR_MAX ( 0x7F  )
# define UCHAR_MAX ( 0xFFU )
#endif

#if __GDCC_Family__ZDACS__
# define CHAR_MIN SCHAR_MIN
# define CHAR_MAX SCHAR_MAX
#else
# define CHAR_MIN UCHAR_MIN
# define CHAR_MAX UCHAR_MAX
#endif

//
// MB_LEN_MAX
//
#define MB_LEN_MAX 6

//
// SHRT_*
//
#if __GDCC_Family__ZDACS__
# define  SHRT_MIN (-0x7FFFFFFF-1)
# define  SHRT_MAX ( 0x7FFFFFFF  )
# define USHRT_MAX ( 0xFFFFFFFFU )
#else
# define  SHRT_MIN (-0x7FFF-1)
# define  SHRT_MAX ( 0x7FFF  )
# define USHRT_MAX ( 0xFFFFU )
#endif

//
// INT_*
//
#define  INT_MIN (-0x7FFFFFFF-1)
#define  INT_MAX ( 0x7FFFFFFF  )
#define UINT_MAX ( 0xFFFFFFFFU )

//
// LONG_*
//
#define  LONG_MIN (-0x7FFFFFFFFFFFFFFFL-1)
#define  LONG_MAX ( 0x7FFFFFFFFFFFFFFFL  )
#define ULONG_MAX ( 0xFFFFFFFFFFFFFFFFUL )

//
// LLONG_*
//
#define  LLONG_MIN (-0x7FFFFFFFFFFFFFFFFFFFFFFFLL-1)
#define  LLONG_MAX ( 0x7FFFFFFFFFFFFFFFFFFFFFFFLL  )
#define ULLONG_MAX ( 0xFFFFFFFFFFFFFFFFFFFFFFFFULL )

#endif//__GDCC_Header__C__limits_h__

