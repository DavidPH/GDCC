//-----------------------------------------------------------------------------
//
// Copyright(C) 2018 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Signal handling.
//
//-----------------------------------------------------------------------------

#define __GDCC_DirectObject

#include <errno.h>
#include <signal.h>


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

static void (*FuncSIGABRT)(int) = SIG_IGN;
static void (*FuncSIGFPE )(int) = SIG_IGN;
static void (*FuncSIGILL )(int) = SIG_IGN;
static void (*FuncSIGINT )(int) = SIG_IGN;
static void (*FuncSIGSEGV)(int) = SIG_IGN;
static void (*FuncSIGTERM)(int) = SIG_IGN;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//=========================================================
// Specify signal handling.
//

//
// signal
//
void (*signal(int sig, void (*func)(int)))(int)
{
   void (**funp)(int);
   switch(sig)
   {
   case SIGABRT: funp = &FuncSIGABRT; break;
   case SIGFPE:  funp = &FuncSIGFPE;  break;
   case SIGILL:  funp = &FuncSIGILL;  break;
   case SIGINT:  funp = &FuncSIGINT;  break;
   case SIGSEGV: funp = &FuncSIGSEGV; break;
   case SIGTERM: funp = &FuncSIGTERM; break;

   default:
      errno = EINVAL;
      return SIG_ERR;
   }

   void (*oldf)(int) = *funp;
   *funp = func == SIG_DFL ? SIG_IGN : func;
   return oldf;
}

//=========================================================
// Send signal.
//

//
// raise
//
int raise(int sig)
{
   void (*func)(int);
   switch(sig)
   {
   case SIGABRT: func = FuncSIGABRT; break;
   case SIGFPE:  func = FuncSIGFPE;  break;
   case SIGILL:  func = FuncSIGILL;  break;
   case SIGINT:  func = FuncSIGINT;  break;
   case SIGSEGV: func = FuncSIGSEGV; break;
   case SIGTERM: func = FuncSIGTERM; break;

   default:
      errno = EINVAL;
      return EINVAL;
   }

   if(func != SIG_IGN)
      func(sig);

   return 0;
}

// EOF

