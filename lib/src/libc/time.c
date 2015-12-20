//-----------------------------------------------------------------------------
//
// Copyright(C) 2015 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Date and time.
//
//-----------------------------------------------------------------------------

#include <time.h>

#if __GDCC_Family__ZDACS__
#include <ACS_ZDoom.h>
#endif


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

static time_t TimeBase;


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// TimeCount
//
#if __GDCC_Family__ZDACS__
[[address(999), call("ScriptI"), script("Open")]]
static void TimeCount(void)
{
   for(;;)
   {
      ACS_Delay(CLOCKS_PER_SEC);
      ++TimeBase;
   }
}
#endif


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//=========================================================
// Time manipulation functions.
//

//
// clock
//
clock_t clock()
{
   return ACS_Timer();
}

//
// difftime
//
double difftime(time_t time1, time_t time0)
{
   return (double)(time1 - time0);
}

time_t mktime(struct tm *timeptr);

//
// time
//
time_t time(time_t *timer)
{
   time_t t = TimeBase;

   if(timer)
      *timer = t;

   return t;
}

//=========================================================
// Time conversion functions.
//

char *asctime(struct tm const *timeptr);

char *ctime(time_t const *timer);

struct tm *gmtime(time_t const *timer);

struct tm *localtime(time_t const *timer);

size_t strftime(char *restrict s, size_t maxsize, char const *restrict format,
   struct tm const *restrict timeptr);

// EOF

