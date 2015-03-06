//-----------------------------------------------------------------------------
//
// Copyright(C) 2015 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Mathematics.
//
// Nearest integer functions.
// Remainder functions.
//
//-----------------------------------------------------------------------------

#include <math.h>

#include <limits.h>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//=========================================================
// Nearest integer functions.
//

//
// ceil
//
double ceil(double x)
{
   return signbit(x) ? trunc(x) : __rinf(x);
}

//
// ceilf
//
float ceilf(float x)
{
   return signbit(x) ? truncf(x) : __rinff(x);
}

//
// ceill
//
long double ceill(long double x)
{
   return signbit(x) ? truncl(x) : __rinfl(x);
}

//
// floor
//
double floor(double x)
{
   return signbit(x) ? __rinf(x) : trunc(x);
}

//
// floorf
//
float floorf(float x)
{
   return signbit(x) ? __rinff(x) : truncf(x);
}

//
// floorl
//
long double floorl(long double x)
{
   return signbit(x) ? __rinfl(x) : truncl(x);
}

//
// nearbyint
//
double nearbyint(double x)
{
   return trunc(x);
}

//
// nearbyintf
//
float nearbyintf(float x)
{
   return truncf(x);
}

//
// nearbyintl
//
long double nearbyintl(long double x)
{
   return truncl(x);
}

//
// rint
//
double rint(double x)
{
   return trunc(x);
}

//
// rintf
//
float rintf(float x)
{
   return truncf(x);
}

//
// rintl
//
long double rintl(long double x)
{
   return truncl(x);
}

//
// lrint
//
long int lrint(double x)
{
   return x;
}

//
// lrintf
//
long int lrintf(float x)
{
   return x;
}

//
// lrintl
//
long int lrintl(long double x)
{
   return x;
}

//
// llrint
//
long long int llrint(double x)
{
   return x;
}

//
// llrintf
//
long long int llrintf(float x)
{
   return x;
}

//
// llrintl
//
long long int llrintl(long double x)
{
   return x;
}

//
// round
//
double round(double x)
{
   union {double f; long i; struct {int i0, i1;};} u = {x};

   int exp = ((u.i1 >> 20) & 0x7FF) - 1023;

   if(exp >= 53) return x;
   if(exp <  -1) return u.i &= 0x8000000000000000, u.f;
   if(exp == -1) return u.i1 & 0x80000000 ? -1.0 : 1.0;

   if(u.i & (0x0008000000000000 >> exp))
      u.i += 0x0010000000000000 >> exp;

   return u.i &= ~(0x000FFFFFFFFFFFFF >> exp), u.f;
}

//
// roundf
//
float roundf(float x)
{
   union {float f; int i; struct {int i1;};} u = {x};

   int exp = ((u.i1 >> 23) & 0xFF) - 127;

   if(exp >= 24) return x;
   if(exp <  -1) return u.i &= 0x80000000, u.f;
   if(exp == -1) return u.i1 & 0x80000000 ? -1.0f : 1.0f;

   if(u.i & (0x00400000 >> exp))
      u.i += 0x00800000 >> exp;

   return u.i &= ~(0x000FFFFF >> exp), u.f;
}

//
// roundl
//
long double roundl(long double x)
{
   union {long double f; long i; struct {int i0, i1;};} u = {x};

   int exp = ((u.i1 >> 20) & 0x7FF) - 1023;

   if(exp >= 53) return x;
   if(exp <  -1) return u.i &= 0x8000000000000000, u.f;
   if(exp == -1) return u.i1 & 0x80000000 ? -1.0l : 1.0l;

   if(u.i & (0x0008000000000000 >> exp))
      u.i += 0x0010000000000000 >> exp;

   return u.i &= ~(0x000FFFFFFFFFFFFF >> exp), u.f;
}

//
// lround
//
long int lround(double x)
{
   return round(x);
}

//
// lroundf
//
long int lroundf(float x)
{
   return roundf(x);
}

//
// lroundl
//
long int lroundl(long double x)
{
   return roundl(x);
}

//
// llround
//
long long int llround(double x)
{
   return round(x);
}

//
// llroundf
//
long long int llroundf(float x)
{
   return roundf(x);
}

//
// llroundl
//
long long int llroundl(long double x)
{
   return roundl(x);
}

//
// trunc
//
double trunc(double x)
{
   union {double f; long i; struct {int i0, i1;};} u = {x};

   int exp = ((u.i1 >> 20) & 0x7FF) - 1023;

   if(exp >= 53) return x;
   if(exp <   0) return u.i &= 0x8000000000000000, u.f;

   return u.i &= ~(0x000FFFFFFFFFFFFF >> exp), u.f;
}

//
// truncf
//
float truncf(float x)
{
   union {float f; int i; struct {int i1;};} u = {x};

   int exp = ((u.i1 >> 23) & 0xFF) - 127;

   if(exp >= 24) return x;
   if(exp <   0) return u.i &= 0x80000000, u.f;

   return u.i &= ~(0x000FFFFF >> exp), u.f;
}

//
// truncl
//
long double truncl(long double x)
{
   union {long double f; long i; struct {int i0, i1;};} u = {x};

   int exp = ((u.i1 >> 20) & 0x7FF) - 1023;

   if(exp >= 53) return x;
   if(exp <   0) return u.i &= 0x8000000000000000, u.f;

   return u.i &= ~(0x000FFFFFFFFFFFFF >> exp), u.f;
}

//=========================================================
// Remainder functions.
//

//
// fmod
//
double fmod(double x, double y)
{
   return x - trunc(x / y) * y;
}

//
// fmodf
//
float fmodf(float x, float y)
{
   return x - truncf(x / y) * y;
}

//
// fmodl
//
long double fmodl(long double x, long double y)
{
   return x - truncl(x / y) * y;
}

//
// remainder
//
double remainder(double x, double y)
{
   double q = x / y;
   double n = trunc(q);
   if(fabs(n - q) >= 0.5)
      ++n;

   return x - n * y;
}

//
// remainderf
//
float remainderf(float x, float y)
{
   float q = x / y;
   float n = truncf(q);
   if(fabsf(n - q) >= 0.5F)
      ++n;

   return x - n * y;
}

//
// remainderl
//
long double remainderl(long double x, long double y)
{
   long double q = x / y;
   long double n = truncl(q);
   if(fabsl(n - q) >= 0.5L)
      ++n;

   return x - n * y;
}

//
// remquo
//
double remquo(double x, double y, int *quo)
{
   double q = x / y;
   double n = trunc(q);
   if(fabs(n - q) >= 0.5)
      ++n;

   *quo = __irmod(round(q));
   return x - n * y;
}

//
// remquof
//
float remquof(float x, float y, int *quo)
{
   float q = x / y;
   float n = truncf(q);
   if(fabsf(n - q) >= 0.5F)
      ++n;

   *quo = __irmodf(roundf(q));
   return x - n * y;
}

//
// remquol
//
long double remquol(long double x, long double y, int *quo)
{
   long double q = x / y;
   long double n = truncl(q);
   if(fabsl(n - q) >= 0.5L)
      ++n;

   *quo = __irmodl(roundl(q));
   return x - n * y;
}


//=========================================================
// Implementation extensions.
//

//
// __rinf
//
double __rinf(double x)
{
   union {double f; long i; struct {int i0, i1;};} u = {x};

   int exp = ((u.i1 >> 20) & 0x7FF) - 1023;

   if(exp >= 53 || exp == -1023) return x;
   if(exp < 0) return u.i1 & 0x80000000 ? -1.0 : 1.0;

   long mask = 0x000FFFFFFFFFFFFF >> exp;

   if(u.i & mask)
   {
      u.i &= ~mask;
      u.i += 1L << exp;
   }

   return u.f;
}

//
// __rinff
//
float __rinff(float x)
{
   union {float f; int i; struct {int i1;};} u = {x};

   int exp = ((u.i1 >> 23) & 0xFF) - 127;

   if(exp >= 24 || exp == -127) return x;
   if(exp < 0) return u.i1 & 0x80000000 ? -1.0f : 1.0f;

   int mask = 0x007FFFFF >> exp;

   if(u.i & mask)
   {
      u.i &= ~mask;
      u.i += 1 << exp;
   }

   return u.f;
}

//
// __rinfl
//
long double __rinfl(long double x)
{
   union {long double f; long i; struct {int i0, i1;};} u = {x};

   int exp = ((u.i1 >> 20) & 0x7FF) - 1023;

   if(exp >= 53 || exp == -1023) return x;
   if(exp < 0) return u.i1 & 0x80000000 ? -1.0l : 1.0l;

   long mask = 0x000FFFFFFFFFFFFF >> exp;

   if(u.i & mask)
   {
      u.i &= ~mask;
      u.i += 1L << exp;
   }

   return u.f;
}

//
// __irmod
//
// Returns x % INT_MAX as an int.
//
int __irmod(double x)
{
   union {double f; long i; struct {int i0, i1;};} u = {x};

   int sig = u.i1 & 0x80000000;
   int exp = ((u.i1 >> 20) & 0x7FF) - 1023;
   int res;

   if(exp >= 83 || exp < 0) return 0;

   if(exp >= 52)
      res = (u.i0 << (exp - 52)) & INT_MAX;
   else
   {
      u.i1 &= 0x000FFFFF;
      u.i1 |= 0x00100000;
      res = (u.i >> (52 - exp)) & INT_MAX;
   }

   return sig ? -res : res;
}

//
// __irmodf
//
int __irmodf(float x)
{
   union {float f; int i, i0, i1;} u = {x};

   int sig = u.i1 & 0x80000000;
   int exp = ((u.i1 >> 23) & 0xFF) - 127;
   int res;

   if(exp >= 54 || exp < 0) return 0;

   if(exp >= 23)
      res = (u.i0 << (exp - 23)) & INT_MAX;
   else
   {
      u.i1 &= 0x007FFFFF;
      u.i1 |= 0x00800000;
      res = (u.i >> (23 - exp)) & INT_MAX;
   }

   return sig ? -res : res;
}

//
// __irmodl
//
int __irmodl(long double x)
{
   union {long double f; long i; struct {int i0, i1;};} u = {x};

   int sig = u.i1 & 0x80000000;
   int exp = ((u.i1 >> 20) & 0x7FF) - 1023;
   int res;

   if(exp >= 83 || exp < 0) return 0;

   if(exp >= 52)
      res = (u.i0 << (exp - 52)) & INT_MAX;
   else
   {
      u.i1 &= 0x000FFFFF;
      u.i1 |= 0x00100000;
      res = (u.i >> (52 - exp)) & INT_MAX;
   }

   return sig ? -res : res;
}

// EOF

