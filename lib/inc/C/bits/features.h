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

#ifndef __GDCC_Header__C__bits__features_h__
#define __GDCC_Header__C__bits__features_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

// If _GNU_SOURCE defined, enable everything.
#if defined(_GNU_SOURCE)
#  undef __STRICT_ANSI__
#  undef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200809L
#endif

// If no feature test macros are defined, set defaults.
#if !defined(__STRICT_ANSI__) && !defined(_POSIX_C_SOURCE) && !defined(_GNU_SOURCE)
#  define _POSIX_C_SOURCE 200809L
#endif

#endif//__GDCC_Header__C__bits__features_h__

