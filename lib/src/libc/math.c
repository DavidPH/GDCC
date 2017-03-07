//-----------------------------------------------------------------------------
//
// Copyright(C) 2015-2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Mathematics.
//
//-----------------------------------------------------------------------------

#define __GDCC__DirectObject

#include <math.h>

#include <errno.h>
#include <float.h>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//=========================================================
// Hyperbolic functions.
//

#if !__GDCC__NoFloat
//
// acosh
//
double acosh(double x)
{
   if(x < 1) {errno = EDOM; return __nan();}

   return log(x + sqrt(x * x - 1));
}

//
// acoshf
//
float acoshf(float x)
{
   if(x < 1) {errno = EDOM; return __nanf();}

   return logf(x + sqrtf(x * x - 1));
}

//
// acoshl
//
long double acoshl(long double x)
{
   if(x < 1) {errno = EDOM; return __nanl();}

   return logl(x + sqrtl(x * x - 1));
}

//
// asinh
//
double asinh(double x)
{
   return log(x + sqrt(x * x + 1));
}

//
// asinhf
//
float asinhf(float x)
{
   return logf(x + sqrtf(x * x + 1));
}

//
// asinhl
//
long double asinhl(long double x)
{
   return logl(x + sqrtl(x * x + 1));
}

//
// atanh
//
double atanh(double x)
{
   if(fabs(x) >= 1) {errno = EDOM; return __nan();}

   return log((1 + x) / (1 - x)) >> 1;
}

//
// atanhf
//
float atanhf(float x)
{
   if(fabsf(x) >= 1) {errno = EDOM; return __nanf();}

   return logf((1 + x) / (1 - x)) >> 1;
}

//
// atanhl
//
long double atanhl(long double x)
{
   if(fabsl(x) >= 1) {errno = EDOM; return __nanl();}

   return logl((1 + x) / (1 - x)) >> 1;
}

//
// cosh
//
double cosh(double x)
{
   return (exp(x) + exp(-x)) >> 1;
}

//
// coshf
//
float coshf(float x)
{
   return (expf(x) + expf(-x)) >> 1;
}

//
// coshl
//
long double coshl(long double x)
{
   return (expl(x) + expl(-x)) >> 1;
}

//
// sinh
//
double sinh(double x)
{
   return (exp(x) - exp(-x)) >> 1;
}

//
// sinhf
//
float sinhf(float x)
{
   return (expf(x) - expf(-x)) >> 1;
}

//
// sinhl
//
long double sinhl(long double x)
{
   return (expl(x) - expl(-x)) >> 1;
}

//
// tanh
//
double tanh(double x)
{
   return (exp(x << 1) - 1) / (exp(x << 1) + 1);
}

//
// tanhf
//
float tanhf(float x)
{
   return (expf(x << 1) - 1) / (expf(x << 1) + 1);
}

//
// tanhl
//
long double tanhl(long double x)
{
   return (expl(x << 1) - 1) / (expl(x << 1) + 1);
}
#endif // !__GDCC__NoFloat

//=========================================================
// Power and absolute-value functions.
//

#if !__GDCC__NoFloat
//
// cbrt
//
double cbrt(double x)
{
   if(x == 0) return 0;

   double y = __approx_cbrt(x);

   for(int j = 0; j != 5; ++j)
      y = ((y << 1) + x / (y * y)) * (1.0/3.0);

   return y;
}

//
// cbrtf
//
float cbrtf(float x)
{
   if(x == 0) return 0;

   float y = __approx_cbrtf(x);

   for(int j = 0; j != 4; ++j)
      y = ((y << 1) + x / (y * y)) * (1.0/3.0);

   return y;
}

//
// cbrtl
//
long double cbrtl(long double x)
{
   if(x == 0) return 0;

   long double y = __approx_cbrt(x);

   for(int j = 0; j != 5; ++j)
      y = ((y << 1) + x / (y * y)) * (1.0/3.0);

   return y;
}

//
// fabs
//
double fabs(double x)
{
   return signbit(x) ? -x : x;
}

//
// fabsf
//
float fabsf(float x)
{
   return signbit(x) ? -x : x;
}

//
// fabsl
//
long double fabsl(long double x)
{
   return signbit(x) ? -x : x;
}

//
// hypot
//
double hypot(double x, double y)
{
   return sqrt(x * x + y * y);
}

//
// hypotf
//
float hypotf(float x, float y)
{
   return sqrtf(x * x + y * y);
}

//
// hypotl
//
long double hypotl(long double x, long double y)
{
   return sqrtl(x * x + y * y);
}

//
// pow
//
double pow(double x, double y)
{
   if(isnan(x)) return x;
   if(isnan(y)) return y;

   return exp2(y * log2(x));
}

//
// powf
//
float powf(float x, float y)
{
   if(isnan(x)) return x;
   if(isnan(y)) return y;

   return exp2f(y * log2f(x));
}

//
// powl
//
long double powl(long double x, long double y)
{
   if(isnan(x)) return x;
   if(isnan(y)) return y;

   return exp2l(y * log2l(x));
}

//
// sqrt
//
double sqrt(double x)
{
   if(x == 0) return 0;

   double y = __approx_sqrt(x);

   for(int j = 0; j != 5; ++j)
      y = (y + x / y) >> 1;

   return y;
}

//
// sqrtf
//
float sqrtf(float x)
{
   if(x == 0) return 0;

   float y = __approx_sqrtf(x);

   for(int j = 0; j != 4; ++j)
      y = (y + x / y) >> 1;

   return y;
}

//
// sqrtl
//
long double sqrtl(long double x)
{
   if(x == 0) return 0;

   long double y = __approx_sqrtl(x);

   for(int j = 0; j != 5; ++j)
      y = (y + x / y) >> 1;

   return y;
}
#endif // !__GDCC__NoFloat

//=========================================================
// Error and gamma functions.
//

double erf(double x);
float erff(float x);
long double erfl(long double x);

double erfc(double x);
float erfcf(float x);
long double erfcl(long double x);

double lgamma(double x);
float lgammaf(float x);
long double lgammal(long double x);

double tgamma(double x);
float tgammaf(float x);
long double tgammal(long double x);

//=========================================================
// Manipulation functions.
//

#if !__GDCC__NoFloat
//
// copysign
//
double copysign(double x, double y)
{
   __asm
   (
      "BAnd_W 1, LocReg(Lit(:x), 1), LocReg(Lit(:x), 1), Lit(0x7FFFFFFF)\n"
      "BAnd_W 1, Stk(), LocReg(Lit(:y), 1), Lit(0x80000000)\n"
      "BOrI_W 1, LocReg(Lit(:x), 1), LocReg(Lit(:x), 1), Stk()\n"
   );

   return x;
}

//
// copysignf
//
float copysignf(float x, float y)
{
   __asm
   (
      "BAnd_W 1, LocReg(Lit(:x), 0), LocReg(Lit(:x), 0), Lit(0x7FFFFFFF)\n"
      "BAnd_W 1, Stk(), LocReg(Lit(:y), 0), Lit(0x80000000)\n"
      "BOrI_W 1, LocReg(Lit(:x), 0), LocReg(Lit(:x), 0), Stk()\n"
   );

   return x;
}

//
// copysignl
//
long double copysignl(long double x, long double y)
{
   __asm
   (
      "BAnd_W 1, LocReg(Lit(:x), 1), LocReg(Lit(:x), 1), Lit(0x7FFFFFFF)\n"
      "BAnd_W 1, Stk(), LocReg(Lit(:y), 1), Lit(0x80000000)\n"
      "BOrI_W 1, LocReg(Lit(:x), 1), LocReg(Lit(:x), 1), Stk()\n"
   );

   return x;
}

//
// nan
//
double nan(char const *tagp)
{
   return __nan();
}

//
// nanf
//
float nanf(char const *tagp)
{
   return __nanf();
}

//
// nanl
//
long double nanl(char const *tagp)
{
   return __nanl();
}

//
// nextafter
//
double nextafter(double x, double y)
{
   if(x < y)
   {
      if(x == -DBL_MIN) return 0;
      if(x == 0) return +DBL_MIN;

      __asm("AddU_W 2, LocReg(Lit(:x)), LocReg(Lit(:x)), Lit(1)\n");
   }
   else if(x > y)
   {
      if(x == +DBL_MIN) return 0;
      if(x == 0) return -DBL_MIN;

      __asm("SubU_W 2, LocReg(Lit(:x)), LocReg(Lit(:x)), Lit(1)\n");
   }

   return x;
}

//
// nextafterf
//
float nextafterf(float x, float y)
{
   if(x < y)
   {
      if(x == -FLT_MIN) return 0;
      if(x == 0) return +FLT_MIN;

      __asm("AddU_W 1, LocReg(Lit(:x)), LocReg(Lit(:x)), Lit(1)\n");
   }
   else if(x > y)
   {
      if(x == +FLT_MIN) return 0;
      if(x == 0) return -FLT_MIN;

      __asm("SubU_W 1, LocReg(Lit(:x)), LocReg(Lit(:x)), Lit(1)\n");
   }

   return x;
}

//
// nextafterl
//
long double nextafterl(long double x, long double y)
{
   if(x < y)
   {
      if(x == -LDBL_MIN) return 0;
      if(x == 0) return +LDBL_MIN;

      __asm("AddU_W 2, LocReg(Lit(:x)), LocReg(Lit(:x)), Lit(1)\n");
   }
   else if(x > y)
   {
      if(x == +LDBL_MIN) return 0;
      if(x == 0) return -LDBL_MIN;

      __asm("SubU_W 2, LocReg(Lit(:x)), LocReg(Lit(:x)), Lit(1)\n");
   }

   return x;
}

//
// nexttoward
//
double nexttoward(double x, long double y)
{
   if(x < y)
   {
      if(x == -DBL_MIN) return 0;
      if(x == 0) return +DBL_MIN;

      __asm("AddU_W 2, LocReg(Lit(:x)), LocReg(Lit(:x)), Lit(1)\n");
   }
   else if(x > y)
   {
      if(x == +DBL_MIN) return 0;
      if(x == 0) return -DBL_MIN;

      __asm("SubU_W 2, LocReg(Lit(:x)), LocReg(Lit(:x)), Lit(1)\n");
   }

   return x;
}

//
// nexttowardf
//
float nexttowardf(float x, long double y)
{
   if(x < y)
   {
      if(x == -FLT_MIN) return 0;
      if(x == 0) return +FLT_MIN;

      __asm("AddU_W 1, LocReg(Lit(:x)), LocReg(Lit(:x)), Lit(1)\n");
   }
   else if(x > y)
   {
      if(x == +FLT_MIN) return 0;
      if(x == 0) return -FLT_MIN;

      __asm("SubU_W 1, LocReg(Lit(:x)), LocReg(Lit(:x)), Lit(1)\n");
   }

   return x;
}

//
// nexttowardl
//
long double nexttowardl(long double x, long double y)
{
   if(x < y)
   {
      if(x == -LDBL_MIN) return 0;
      if(x == 0) return +LDBL_MIN;

      __asm("AddU_W 2, LocReg(Lit(:x)), LocReg(Lit(:x)), Lit(1)\n");
   }
   else if(x > y)
   {
      if(x == +LDBL_MIN) return 0;
      if(x == 0) return -LDBL_MIN;

      __asm("SubU_W 2, LocReg(Lit(:x)), LocReg(Lit(:x)), Lit(1)\n");
   }

   return x;
}
#endif // !__GDCC__NoFloat

//=========================================================
// Maximum, minimum, and positive difference functions.
//

#if !__GDCC__NoFloat
//
// fdim
//
double fdim(double x, double y)
{
   if(isnan(x)) return x;
   if(isnan(y)) return y;

   return x > y ? x - y : 0;
}

//
// fdimf
//
float fdimf(float x, float y)
{
   if(isnan(x)) return x;
   if(isnan(y)) return y;

   return x > y ? x - y : 0;
}

//
// fdiml
//
long double fdiml(long double x, long double y)
{
   if(isnan(x)) return x;
   if(isnan(y)) return y;

   return x > y ? x - y : 0;
}

//
// fmax
//
double fmax(double x, double y)
{
   if(isnan(x)) return y;
   if(isnan(y)) return x;

   return x < y ? y : x;
}

//
// fmaxf
//
float fmaxf(float x, float y)
{
   if(isnan(x)) return y;
   if(isnan(y)) return x;

   return x < y ? y : x;
}

//
// fmaxl
//
long double fmaxl(long double x, long double y)
{
   if(isnan(x)) return y;
   if(isnan(y)) return x;

   return x < y ? y : x;
}

//
// fmin
//
double fmin(double x, double y)
{
   if(isnan(x)) return y;
   if(isnan(y)) return x;

   return x < y ? x : y;
}

//
// fminf
//
float fminf(float x, float y)
{
   if(isnan(x)) return y;
   if(isnan(y)) return x;

   return x < y ? x : y;
}

//
// fminl
//
long double fminl(long double x, long double y)
{
   if(isnan(x)) return y;
   if(isnan(y)) return x;

   return x < y ? x : y;
}
#endif // !__GDCC__NoFloat

//=========================================================
// Floating multiply-add.
//

#if !__GDCC__NoFloat
//
// fma
//
double fma(double x, double y, double z)
{
   return x * y + z;
}

//
// fmaf
//
float fmaf(float x, float y, float z)
{
   return x * y + z;
}

//
// fmal
//
long double fmal(long double x, long double y, long double z)
{
   return x * y + z;
}
#endif // !__GDCC__NoFloat

// EOF

