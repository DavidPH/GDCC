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
// Characteristics of floating types.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__float_h__
#define __GDCC_Header__C__float_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// FLT_ROUNDS
//
// -1 - indeterminate
//  0 - toward zero
//  1 - to nearest
//  2 - toward positive infinity
//  3 - toward negative infinity
//
// fesetround is unsupported, so this macro expands to a constant.
//
#define FLT_ROUNDS 0

//
// FLT_EVAL_METHOD
//
// -1 - indeterminate
//  0 - all operations as type
//  1 - float as double, others as type
//  2 - all operations as long double
//
// This would be 0, except that constants evaluate with unknown precision.
//
#define FLT_EVAL_METHOD -1

//
// FLT_HAS_SUBNORM
//
// -1 - indeterminate
//  0 - absent
//  1 - present
//
#define  FLT_HAS_SUBNORM -1
#define  DBL_HAS_SUBNORM -1
#define LDBL_HAS_SUBNORM -1

//
// FLT_RADIX
//
#define FLT_RADIX 2

//
// FLT_MANT_DIG
//
#define  FLT_MANT_DIG 24
#define  DBL_MANT_DIG 53
#define LDBL_MANT_DIG 53

//
// FLT_DECIMAL_DIG
//
#define  FLT_DECIMAL_DIG  9
#define  DBL_DECIMAL_DIG 17
#define LDBL_DECIMAL_DIG 17

//
// DECIMAL_DIG
//
#define DECIMAL_DIG LDBL_DECIMAL_DIG

//
// FLT_DIG
//
#define  FLT_DIG  6
#define  DBL_DIG 15
#define LDBL_DIG 15

//
// FLT_MIN_EXP
//
#define  FLT_MIN_EXP ( -125)
#define  DBL_MIN_EXP (-1021)
#define LDBL_MIN_EXP (-1021)

//
// FLT_MIN_10_EXP
//
#define  FLT_MIN_10_EXP ( -37)
#define  DBL_MIN_10_EXP (-307)
#define LDBL_MIN_10_EXP (-307)

//
// FLT_MAX_EXP
//
#define  FLT_MAX_EXP  128
#define  DBL_MAX_EXP 1024
#define LDBL_MAX_EXP 1024

//
// FLT_MAX_10_EXP
//
#define  FLT_MAX_10_EXP  38
#define  DBL_MAX_10_EXP 308
#define LDBL_MAX_10_EXP 308

//
// FLT_MAX
//
#define  FLT_MAX 0x1.FFFFFEp+127F
#define  DBL_MAX 0x1.FFFFFFFFFFFFFp+1023
#define LDBL_MAX 0x1.FFFFFFFFFFFFFp+1023L

//
// FLT_EPSILON
//
#define  FLT_EPSILON 0x1.0p-23F
#define  DBL_EPSILON 0x1.0p-52
#define LDBL_EPSILON 0x1.0p-52L

//
// FLT_MIN
//
#define  FLT_MIN 0x1.0p-126F
#define  DBL_MIN 0x1.0p-1022
#define LDBL_MIN 0x1.0p-1022L

//
// FLT_TRUE_MIN
//
#define  FLT_TRUE_MIN 0x1.0p-126F
#define  DBL_TRUE_MIN 0x1.0p-1022
#define LDBL_TRUE_MIN 0x1.0p-1022L

#endif//__GDCC_Header__C__float_h__

