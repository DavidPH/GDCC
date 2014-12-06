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
// Sizes of integer types.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__limits_h__
#define __GDCC_Header__C__limits_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define CHAR_BIT 32

#define SCHAR_MIN (-0x7FFFFFFF-1)
#define SCHAR_MAX ( 0x7FFFFFFF  )
#define UCHAR_MAX ( 0xFFFFFFFFU )

#define CHAR_MIN SCHAR_MIN
#define CHAR_MAX SCHAR_MAX

#define MB_LEN_MAX 1

#define  SHRT_MIN (-0x7FFFFFFF-1)
#define  SHRT_MAX ( 0x7FFFFFFF  )
#define USHRT_MAX ( 0xFFFFFFFFU )

#define  INT_MIN (-0x7FFFFFFF-1)
#define  INT_MAX ( 0x7FFFFFFF  )
#define UINT_MAX ( 0xFFFFFFFFU )

#define  LONG_MIN (-0x7FFFFFFFFFFFFFFFL-1)
#define  LONG_MAX ( 0x7FFFFFFFFFFFFFFFL  )
#define ULONG_MAX ( 0xFFFFFFFFFFFFFFFFUL )

#define  LLONG_MIN (-0x7FFFFFFFFFFFFFFFFFFFFFFFLL-1)
#define  LLONG_MAX ( 0x7FFFFFFFFFFFFFFFFFFFFFFFLL  )
#define ULLONG_MAX ( 0xFFFFFFFFFFFFFFFFFFFFFFFFULL )

#endif//__GDCC_Header__C__limits_h__

