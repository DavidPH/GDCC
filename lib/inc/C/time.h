//-----------------------------------------------------------------------------
//
// Copyright(C) 2015-2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// Date and time.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__time_h__
#define __GDCC_Header__C__time_h__

#include <bits/types.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// NULL
//
#ifndef NULL
#define NULL 0
#endif

//
// CLOCKS_PER_SEC
//
#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 35
#endif

//
// TIME_UTC
//
#ifndef TIME_UTC
#define TIME_UTC 1
#endif


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// size_t
//
#ifndef __GDCC_Have__size_t__
#define __GDCC_Have__size_t__
typedef __size_t size_t;
#endif

//
// clock_t
//
#ifndef __GDCC_Have__clock_t__
#define __GDCC_Have__clock_t__
typedef __clock_t clock_t;
#endif

//
// time_t
//
#ifndef __GDCC_Have__time_t__
#define __GDCC_Have__time_t__
typedef __time_t time_t;
#endif

//
// struct tm
//
struct tm
{
   int tm_sec;   // seconds after the minute - [0, 60]
   int tm_min;   // minutes after the hour - [0, 59]
   int tm_hour;  // hours since midnight - [0, 23]
   int tm_mday;  // day of the month - [1, 31]
   int tm_mon;   // months since January - [0, 11]
   int tm_year;  // years since 1900
   int tm_wday;  // days since Sunday - [0, 6]
   int tm_yday;  // days since January 1 - [0, 365]
   int tm_isdst; // Daylight Saving Time flag
};


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

#ifdef __cplusplus
extern "C" {
#endif

//
// Time manipulation functions.
//

clock_t clock();

double difftime(time_t _time1, time_t _time0);

time_t mktime(struct tm *_timeptr);

time_t time(time_t *_timer);

int timespec_get(struct timespec *_ts, int _base);

//
// Time conversion functions.
//

char *asctime(struct tm const *_timeptr);

char *ctime(time_t const *_timer);

struct tm *gmtime(time_t const *_timer);

struct tm *localtime(time_t const *_timer);

size_t strftime(char *restrict _s, size_t _maxsize, char const *restrict _format,
   struct tm const *restrict _timeptr);

#ifdef __cplusplus
}
#endif

#endif//__GDCC_Header__C__time_h__

