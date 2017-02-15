//-----------------------------------------------------------------------------
//
// Copyright(C) 2015-2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Date and time.
//
//-----------------------------------------------------------------------------

#include <time.h>

#include <stdio.h>

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
[[call("ScriptS"), script("Open")]]
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
   #if __GDCC_Family__ZDACS__
   return ACS_Timer();
   #elif __GDCC_Target__Doominati__
   // TODO
   return 0;
   #else
   return 0;
   #endif
}

#if !__GDCC__NoFloat
//
// difftime
//
double difftime(time_t time1, time_t time0)
{
   return (double)(time1 - time0);
}
#endif

//
// mktime
//
time_t mktime(struct tm *timeptr)
{
   // TODO
   return -1;
}

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

//
// timespec_get
//
int timespec_get(struct timespec *ts, int base)
{
   // TODO
   return 0;
}

//=========================================================
// Time conversion functions.
//

//
// asctime
//
char *asctime(struct tm const *timeptr)
{
   static const char wday_name[7][3] =
   {
      "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
   };

   static const char mon_name[12][3] =
   {
      "Jan", "Feb", "Mar", "Apr", "May", "Jun",
      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
   };

   static char result[26];

   sprintf(result, "%.3s %.3s %2d %.2d:%.2d:%.2d %d\n",
      wday_name[timeptr->tm_wday],
      mon_name[timeptr->tm_mon],
      timeptr->tm_mday, timeptr->tm_hour,
      timeptr->tm_min, timeptr->tm_sec,
      1900 + timeptr->tm_year);

   return result;
}

//
// ctime
//
char *ctime(time_t const *timer)
{
   return asctime(localtime(timer));
}

//
// gmtime
//
struct tm *gmtime(time_t const *timer)
{
   // TODO
   return NULL;
}

//
// localtime
//
struct tm *localtime(time_t const *timer)
{
   // TODO
   return NULL;
}

//
// strftime
//
size_t strftime(char *restrict s, size_t maxsize, char const *restrict format,
   struct tm const *restrict timeptr)
{
   // TODO
   return 0;
}

// EOF

