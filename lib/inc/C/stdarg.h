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
// Variable arguments.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__stdarg_h__
#define __GDCC_Header__C__stdarg_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// va_arg
//
#define va_arg(ap, type) (__va_arg((ap), type))

//
// va_copy
//
#define va_copy(dest, src) ((void)((dest) = (src)))

//
// va_end
//
#define va_end(ap) ((void)(ap))

//
// va_start
//
#define va_start(ap, parmN) ((void)((ap) = __va_start))


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// va_list
//
typedef __va_list va_list;

#endif//__GDCC_Header__C__stdarg_h__

