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
// Integer types.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__stdint_h__
#define __GDCC_Header__C__stdint_h__

#include <bits/types.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#if !__GDCC_Family__ZDACS__
#define  INT8_MIN (-0x7F-1)
#define  INT8_MAX ( 0x7F  )
#define UINT8_MAX ( 0xFFU )
#endif

#if !__GDCC_Family__ZDACS__
#define  INT16_MIN (-0x7FFF-1)
#define  INT16_MAX ( 0x7FFF  )
#define UINT16_MAX ( 0xFFFFU )
#endif

#define  INT32_MIN (-0x7FFFFFFF-1)
#define  INT32_MAX ( 0x7FFFFFFF  )
#define UINT32_MAX ( 0xFFFFFFFFU )

#define  INT64_MIN (-0x7FFFFFFFFFFFFFFF-1)
#define  INT64_MAX ( 0x7FFFFFFFFFFFFFFF  )
#define UINT64_MAX ( 0xFFFFFFFFFFFFFFFFU )

#define  INT96_MIN (-0x7FFFFFFFFFFFFFFFFFFFFFFF-1)
#define  INT96_MAX ( 0x7FFFFFFFFFFFFFFFFFFFFFFF  )
#define UINT96_MAX ( 0xFFFFFFFFFFFFFFFFFFFFFFFFU )

#if __GDCC_Family__ZDACS__
#define  INT_LEAST8_MIN (-0x7FFFFFFF-1)
#define  INT_LEAST8_MAX ( 0x7FFFFFFF  )
#define UINT_LEAST8_MAX ( 0xFFFFFFFFU )
#else
#define  INT_LEAST8_MIN (-0x7F-1)
#define  INT_LEAST8_MAX ( 0x7F  )
#define UINT_LEAST8_MAX ( 0xFFU )
#endif

#if __GDCC_Family__ZDACS__
#define  INT_LEAST16_MIN (-0x7FFFFFFF-1)
#define  INT_LEAST16_MAX ( 0x7FFFFFFF  )
#define UINT_LEAST16_MAX ( 0xFFFFFFFFU )
#else
#define  INT_LEAST16_MIN (-0x7FFF-1)
#define  INT_LEAST16_MAX ( 0x7FFF  )
#define UINT_LEAST16_MAX ( 0xFFFFU )
#endif

#define  INT_LEAST32_MIN (-0x7FFFFFFF-1)
#define  INT_LEAST32_MAX ( 0x7FFFFFFF  )
#define UINT_LEAST32_MAX ( 0xFFFFFFFFU )

#define  INT_LEAST64_MIN (-0x7FFFFFFFFFFFFFFF-1)
#define  INT_LEAST64_MAX ( 0x7FFFFFFFFFFFFFFF  )
#define UINT_LEAST64_MAX ( 0xFFFFFFFFFFFFFFFFU )

#define  INT_LEAST96_MIN (-0x7FFFFFFFFFFFFFFFFFFFFFFF-1)
#define  INT_LEAST96_MAX ( 0x7FFFFFFFFFFFFFFFFFFFFFFF  )
#define UINT_LEAST96_MAX ( 0xFFFFFFFFFFFFFFFFFFFFFFFFU )

#define  INT_FAST8_MIN (-0x7FFFFFFF-1)
#define  INT_FAST8_MAX ( 0x7FFFFFFF  )
#define UINT_FAST8_MAX ( 0xFFFFFFFFU )

#define  INT_FAST16_MIN (-0x7FFFFFFF-1)
#define  INT_FAST16_MAX ( 0x7FFFFFFF  )
#define UINT_FAST16_MAX ( 0xFFFFFFFFU )

#define  INT_FAST32_MIN (-0x7FFFFFFF-1)
#define  INT_FAST32_MAX ( 0x7FFFFFFF  )
#define UINT_FAST32_MAX ( 0xFFFFFFFFU )

#define  INT_FAST64_MIN (-0x7FFFFFFFFFFFFFFF-1)
#define  INT_FAST64_MAX ( 0x7FFFFFFFFFFFFFFF  )
#define UINT_FAST64_MAX ( 0xFFFFFFFFFFFFFFFFU )

#define  INT_FAST96_MIN (-0x7FFFFFFFFFFFFFFFFFFFFFFF-1)
#define  INT_FAST96_MAX ( 0x7FFFFFFFFFFFFFFFFFFFFFFF  )
#define UINT_FAST96_MAX ( 0xFFFFFFFFFFFFFFFFFFFFFFFFU )

#define  INTPTR_MIN (-0x7FFFFFFF-1)
#define  INTPTR_MAX ( 0x7FFFFFFF  )
#define UINTPTR_MAX ( 0xFFFFFFFFU )

#define  INTMAX_MIN (-0x7FFFFFFFFFFFFFFFFFFFFFFF-1)
#define  INTMAX_MAX ( 0x7FFFFFFFFFFFFFFFFFFFFFFF  )
#define UINTMAX_MAX ( 0xFFFFFFFFFFFFFFFFFFFFFFFFU )

#ifndef PTRDIFF_MIN
#define PTRDIFF_MIN (-0x7FFFFFFF-1)
#define PTRDIFF_MAX ( 0x7FFFFFFF  )
#endif

#ifndef SIG_ATOMIC_MIN
#define SIG_ATOMIC_MIN (-0x7FFFFFFF-1)
#define SIG_ATOMIC_MAX ( 0x7FFFFFFF  )
#endif

#ifndef SIZE_MAX
#define SIZE_MAX (0xFFFFFFFFU)
#endif

#ifndef WCHAR_MIN
#define WCHAR_MIN 0
#define WCHAR_MAX 0xFFFFFFFF
#endif

#ifndef WINT_MIN
#define WINT_MIN (-0x7FFFFFFF-1)
#define WINT_MAX ( 0x7FFFFFFF  )
#endif

#define INT8_C (VALUE) (VALUE      )
#define INT16_C(VALUE) (VALUE      )
#define INT32_C(VALUE) (VALUE      )
#define INT64_C(VALUE) (VALUE ## L )
#define INT96_C(VALUE) (VALUE ## LL)

#define UINT8_C (VALUE) (VALUE ## U  )
#define UINT16_C(VALUE) (VALUE ## U  )
#define UINT32_C(VALUE) (VALUE ## U  )
#define UINT64_C(VALUE) (VALUE ## UL )
#define UINT96_C(VALUE) (VALUE ## ULL)

#define  INTMAX_C(VALUE) (VALUE ##  LL)
#define UINTMAX_C(VALUE) (VALUE ## ULL)


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// intN_t
//
#if !__GDCC_Family__ZDACS__
typedef signed          char int8_t;
typedef signed     short int int16_t;
#endif
typedef signed           int int32_t;
typedef signed      long int int64_t;
typedef signed long long int int96_t;

//
// uintN_t
//
#if !__GDCC_Family__ZDACS__
typedef unsigned          char uint8_t;
typedef unsigned     short int uint16_t;
#endif
typedef unsigned           int uint32_t;
typedef unsigned      long int uint64_t;
typedef unsigned long long int uint96_t;

//
// int_leastN_t
//
typedef signed          char int_least8_t;
typedef signed     short int int_least16_t;
typedef signed           int int_least32_t;
typedef signed      long int int_least64_t;
typedef signed long long int int_least96_t;

//
// uint_leastN_t
//
typedef unsigned          char uint_least8_t;
typedef unsigned     short int uint_least16_t;
typedef unsigned           int uint_least32_t;
typedef unsigned      long int uint_least64_t;
typedef unsigned long long int uint_least96_t;

//
// int_fastN_t
//
typedef signed           int int_fast8_t;
typedef signed           int int_fast16_t;
typedef signed           int int_fast32_t;
typedef signed      long int int_fast64_t;
typedef signed long long int int_fast96_t;

//
// uint_fastN_t
//
typedef unsigned           int uint_fast8_t;
typedef unsigned           int uint_fast16_t;
typedef unsigned           int uint_fast32_t;
typedef unsigned      long int uint_fast64_t;
typedef unsigned long long int uint_fast96_t;

//
// intptr_t
//
#ifndef __GDCC_Have__intptr_t__
#define __GDCC_Have__intptr_t__
typedef __intptr_t intptr_t;
#endif

//
// uintptr_t
//
#ifndef __GDCC_Have__uintptr_t__
#define __GDCC_Have__uintptr_t__
typedef __uintptr_t uintptr_t;
#endif

//
// intmax_t
//
#ifndef __GDCC_Have__intmax_t__
#define __GDCC_Have__intmax_t__
typedef __intmax_t intmax_t;
#endif

//
// uintmax_t
//
#ifndef __GDCC_Have__uintmax_t__
#define __GDCC_Have__uintmax_t__
typedef __uintmax_t uintmax_t;
#endif

#endif//__GDCC_Header__C__stdint_h__

