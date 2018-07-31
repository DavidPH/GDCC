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
// Floating-point environment.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__fenv_h__
#define __GDCC_Header__C__fenv_h__

#include <bits/types.h>


#ifdef __cplusplus
extern "C" {
#endif


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// FE_* exceptions
//

//
// FE_ALL_EXCEPT
//
#define FE_ALL_EXCEPT 0

//
// FE_* rounding
//

//
// FL_DFL_ENV
//
#define FL_DFL_ENV ((fenv_t const *)0)


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// fenv_t
//
typedef struct __fenv_t fenv_t;

//
// fexcept_t
//
typedef struct __fexcept_t fexcept_t;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// Floating-point exceptions.
//

int feclearexcept(int _excepts);
int fegetexceptflag(fexcept_t *_flagp, int _excepts);
int feraiseexcept(int _excepts);
int fesetexceptflag(fexcept_t const *_flagp, int _excepts);
int fetestexcept(int _excepts);

//
// Rounding.
//

int fegetround(void);
int fesetround(int _round);

//
// Environment.
//

int fegetenv(fenv_t *_envp);
int feholdexcept(fenv_t *_envp);
int fesetenv(const fenv_t *_envp);
int feupdateenv(const fenv_t *_envp);

#ifdef __cplusplus
}
#endif

#endif//__GDCC_Header__C__fenv_h__

