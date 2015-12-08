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
// Date and time.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__time_h__
#define __GDCC_Header__C__time_h__


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


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// size_t
//
#ifndef __GDCC_Have__size_t__
#define __GDCC_Have__size_t__
typedef unsigned int size_t;
#endif

//
// clock_t
//
#ifndef __GDCC_Have__clock_t__
#define __GDCC_Have__clock_t__
typedef unsigned int clock_t;
#endif

//
// time_t
//
#ifndef __GDCC_Have__time_t__
#define __GDCC_Have__time_t__
typedef long int time_t;
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

double difftime(time_t time1, time_t time0);

time_t mktime(struct tm *timeptr);

time_t time(time_t *timer);

//
// Time conversion functions.
//

char *asctime(struct tm const *timeptr);

char *ctime(time_t const *timer);

struct tm *gmtime(time_t const *timer);

struct tm *localtime(time_t const *timer);

size_t strftime(char *restrict s, size_t maxsize, char const *restrict format,
   struct tm const *restrict timeptr);

#ifdef __cplusplus
}
#endif

#endif//__GDCC_Header__C__time_h__

