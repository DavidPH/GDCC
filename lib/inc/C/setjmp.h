//-----------------------------------------------------------------------------
//
// Copyright(C) 2015 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// Non-local jumps.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__setjmp_h__
#define __GDCC_Header__C__setjmp_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// setjmp
//
#define setjmp(_env) (__setjmp(_env))


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// jmpbuf
//
typedef int jmpbuf[3];


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// Restore calling environment.
//

_Noreturn void (longjmp)(jmpbuf _env, int _val);

#endif//__GDCC_Header__C__setjmp_h__

