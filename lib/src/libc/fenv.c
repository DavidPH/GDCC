//-----------------------------------------------------------------------------
//
// Copyright(C) 2018 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Floating-point environment.
//
//-----------------------------------------------------------------------------

#define __GDCC_DirectObject

#include <fenv.h>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//=========================================================
// Floating-point exceptions.
//

//
// feclearexcept
//
int feclearexcept(int excepts)
{
   return 0;
}

//
// fegetexceptflag
//
int fegetexceptflag(fexcept_t *flagp, int excepts)
{
   return -1;
}

//
// feraiseexcept
//
int feraiseexcept(int excepts)
{
   return excepts == 0 ? 0 : -1;
}

//
// fesetexceptflag
//
int fesetexceptflag(fexcept_t const *flagp, int excepts)
{
   return excepts == 0 ? 0 : -1;
}

//
// fetestexcept
//
int fetestexcept(int excepts)
{
   return 0;
}

//=========================================================
// Rounding.
//

//
// fegetround
//
int fegetround(void)
{
   return -1;
}

//
// fesetround
//
int fesetround(int round)
{
   return -1;
}

//=========================================================
// Environment.
//

//
// fegetenv
//
int fegetenv(fenv_t *envp)
{
   return -1;
}

//
// feholdexcept
//
int feholdexcept(fenv_t *envp)
{
   return -1;
}

//
// fesetenv
//
int fesetenv(const fenv_t *envp)
{
   return -1;
}

//
// feupdateenv
//
int feupdateenv(const fenv_t *envp)
{
   return -1;
}

// EOF

