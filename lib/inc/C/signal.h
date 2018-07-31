//-----------------------------------------------------------------------------
//
// Copyright(C) 2018 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// Signal handling.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__signal_h__
#define __GDCC_Header__C__signal_h__

#include <bits/features.h>


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// SIG_*
//
#define SIG_DFL ((void (*)(int))-2)
#define SIG_ERR ((void (*)(int))-3)
#define SIG_IGN ((void (*)(int))-4)

//
// SIG*
//
#define SIGABRT 1
#define SIGFPE  2
#define SIGILL  3
#define SIGINT  4
#define SIGSEGV 5
#define SIGTERM 6


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// sig_atomic_t
//
#ifndef __GDCC_Have__sig_atomic_t__
#define __GDCC_Have__sig_atomic_t__
typedef int sig_atomic_t;
#endif


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// Specify signal handling.
//

void (*signal(int _sig, void (*_func)(int)))(int);

//
// Send signal.
//

int raise(int _sig);

#ifdef __cplusplus
}
#endif

#endif//__GDCC_Header__C__signal_h__

