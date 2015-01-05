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
// Trigonometric functions.
//
//-----------------------------------------------------------------------------

#include <math.h>

#include <errno.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define M_PI  3.141592653589793238462643383279502884
#define M_PIf 3.141592653589793238462643383279502884F
#define M_PIl 3.141592653589793238462643383279502884L

#define M_PI_2  1.570796326794896619231321691639751442
#define M_PI_2f 1.570796326794896619231321691639751442F
#define M_PI_2l 1.570796326794896619231321691639751442L

//
// CordicCount
//
#define CordicCount  53
#define CordicCountf 24
#define CordicCountl 53

//
// SinCosScale
//
#define SinCosScale  0.60725293500888
#define SinCosScalef 0.60725293500888F
#define SinCosScalel 0.60725293500888L

//
// Cordic_ArcTan
//
#define Cordic_ArcTan(suffix) \
   for(int j = 0; j != CordicCount##suffix; ++j) \
   { \
      x0 = x; \
      \
      if(y < 0) \
      { \
         x -= y  >> j; \
         y += x0 >> j; \
         z -= CordicTable##suffix[j]; \
      } \
      else \
      { \
         x += y  >> j; \
         y -= x0 >> j; \
         z += CordicTable##suffix[j]; \
      } \
   }

//
// Cordic_SinCos
//
#define Cordic_SinCos(suffix) \
   for(int j = 0; j != CordicCount##suffix; ++j) \
   { \
      x0 = x; \
      \
      if(z < 0) \
      { \
         x += y  >> j; \
         y -= x0 >> j; \
         z += CordicTable##suffix[j]; \
      } \
      else \
      { \
         x -= y  >> j; \
         y += x0 >> j; \
         z -= CordicTable##suffix[j]; \
      } \
   }

//
// Domain_ArcTan
//
#define Domain_ArcTan(suffix) \
   if(isnan(y)) return y;

//
// Domain_ArcTan2
//
#define Domain_ArcTan2(suffix) \
   if(isnan(x)) return x; \
   if(isnan(y)) return y; \
   \
   if(signbit(x)) \
   { \
      if(signbit(y)) \
         return atan2##suffix(-y, -x) - M_PI##suffix; \
      else \
         return atan2##suffix(-y, -x) + M_PI##suffix; \
   } \
   else if(!x) \
   { \
      if(y < 0) return -M_PI_2##suffix; \
      if(y > 0) return +M_PI_2##suffix; \
      return y; /* Return 0 with the same sign as y. Since y is zero... */ \
   }

//
// Domain_Cos
//
#define Domain_Cos(suffix) \
   if(isnan(z)) return z; \
   \
   if(isinf(z)) \
   { \
      errno = EDOM; \
      return __nan##suffix(); \
   } \
   \
   /* Force input into range. */ \
   if(z < -M_PI_2##suffix) return -cos##suffix(z + M_PI##suffix); \
   if(z >  M_PI_2##suffix) return -cos##suffix(z - M_PI##suffix);

//
// Domain_Sin
//
#define Domain_Sin(suffix) \
   if(isnan(z)) return z; \
   \
   if(isinf(z)) \
   { \
      errno = EDOM; \
      return __nan##suffix(); \
   } \
   \
   /* Force input into range. */ \
   if(z < -M_PI_2##suffix) return -sin##suffix(z + M_PI##suffix); \
   if(z >  M_PI_2##suffix) return -sin##suffix(z - M_PI##suffix);

//
// Domain_SinCos
//
#define Domain_SinCos(suffix) \
   if(isnan(z)) \
   { \
      *sin = z; \
      *cos = z; \
      return; \
   } \
   \
   if(isinf(z)) \
   { \
      *sin = __nan##suffix(); \
      *cos = __nan##suffix(); \
      errno = EDOM; \
      return; \
   } \
   \
   /* Force input into range. */ \
   if(z < -M_PI_2##suffix) \
   { \
      __sincos##suffix(z + M_PI##suffix, sin, cos); \
      *sin = -*sin; \
      *cos = -*cos; \
      return; \
   } \
   \
   if(z >  M_PI_2##suffix) \
   { \
      __sincos##suffix(z - M_PI##suffix, sin, cos); \
      *sin = -*sin; \
      *cos = -*cos; \
      return; \
   }

//
// Domain_Tan
//
#define Domain_Tan(suffix) \
   if(isnan(z)) return z; \
   \
   if(isinf(z)) \
   { \
      errno = EDOM; \
      return __nan##suffix(); \
   } \
   \
   /* Force input into range. */ \
   while(z < -M_PI_2##suffix) z += M_PI##suffix; \
   while(z >  M_PI_2##suffix) z -= M_PI##suffix;


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

//
// CordicTable
//
static double const CordicTable[] =
{
   0x1.921FB54442D18p-01, 0x1.DAC670561BB4Fp-02, 0x1.F5B75F92C80DDp-03,
   0x1.FD5BA9AAC2F6Ep-04, 0x1.FF55BB72CFDEAp-05, 0x1.FFD55BBA97625p-06,
   0x1.FFF555BBB729Bp-07, 0x1.FFFD555BBBA97p-08, 0x1.FFFF5555BBBB7p-09,
   0x1.FFFFD5555BBBCp-10, 0x1.FFFFF55555BBCp-11, 0x1.FFFFFD55555BCp-12,
   0x1.FFFFFF555555Cp-13, 0x1.FFFFFFD555556p-14, 0x1.FFFFFFF555555p-15,
   0x1.FFFFFFFD55555p-16, 0x1.FFFFFFFF55555p-17, 0x1.FFFFFFFFD5555p-18,
   0x1.FFFFFFFFF5555p-19, 0x1.FFFFFFFFFD555p-20, 0x1.FFFFFFFFFF555p-21,
   0x1.FFFFFFFFFFD55p-22, 0x1.FFFFFFFFFFF55p-23, 0x1.FFFFFFFFFFFD5p-24,
   0x1.FFFFFFFFFFFF5p-25, 0x1.FFFFFFFFFFFFDp-26, 0x1.FFFFFFFFFFFFFp-27,
   0x1p-27, 0x1p-28, 0x1p-29, 0x1p-30, 0x1p-31, 0x1p-32, 0x1p-33, 0x1p-34,
   0x1p-35, 0x1p-36, 0x1p-37, 0x1p-38, 0x1p-39, 0x1p-40, 0x1p-41, 0x1p-42,
   0x1p-43, 0x1p-44, 0x1p-45, 0x1p-46, 0x1p-47, 0x1p-48, 0x1p-49, 0x1p-50,
   0x1p-51, 0x1p-52, 0x1p-53, 0x1p-54, 0x1p-55, 0x1p-56, 0x1p-57, 0x1p-58,
   0x1p-59, 0x1p-60, 0x1p-61, 0x1p-62, 0x1p-63,
};

//
// CordicTablef
//
static float const CordicTablef[] =
{
   0x1.921FB6p-01F, 0x1.DAC670p-02F, 0x1.F5B760p-03F, 0x1.FD5BAAp-04F,
   0x1.FF55BCp-05F, 0x1.FFD55Cp-06F, 0x1.FFF556p-07F, 0x1.FFFD56p-08F,
   0x1.FFFF56p-09F, 0x1.FFFFD6p-10F, 0x1.FFFFF6p-11F, 0x1.FFFFFEp-12F,
   0x1p-12F, 0x1p-13F, 0x1p-14F, 0x1p-15F, 0x1p-16F, 0x1p-17F, 0x1p-18F,
   0x1p-19F, 0x1p-20F, 0x1p-21F, 0x1p-22F, 0x1p-23F, 0x1p-24F, 0x1p-25F,
   0x1p-26F, 0x1p-27F, 0x1p-28F, 0x1p-29F, 0x1p-30F, 0x1p-31F,
};

//
// CordicTablel
//
static long double const CordicTablel[] =
{
   0x1.921FB54442D18p-01L, 0x1.DAC670561BB4Fp-02L, 0x1.F5B75F92C80DDp-03L,
   0x1.FD5BA9AAC2F6Ep-04L, 0x1.FF55BB72CFDEAp-05L, 0x1.FFD55BBA97625p-06L,
   0x1.FFF555BBB729Bp-07L, 0x1.FFFD555BBBA97p-08L, 0x1.FFFF5555BBBB7p-09L,
   0x1.FFFFD5555BBBCp-10L, 0x1.FFFFF55555BBCp-11L, 0x1.FFFFFD55555BCp-12L,
   0x1.FFFFFF555555Cp-13L, 0x1.FFFFFFD555556p-14L, 0x1.FFFFFFF555555p-15L,
   0x1.FFFFFFFD55555p-16L, 0x1.FFFFFFFF55555p-17L, 0x1.FFFFFFFFD5555p-18L,
   0x1.FFFFFFFFF5555p-19L, 0x1.FFFFFFFFFD555p-20L, 0x1.FFFFFFFFFF555p-21L,
   0x1.FFFFFFFFFFD55p-22L, 0x1.FFFFFFFFFFF55p-23L, 0x1.FFFFFFFFFFFD5p-24L,
   0x1.FFFFFFFFFFFF5p-25L, 0x1.FFFFFFFFFFFFDp-26L, 0x1.FFFFFFFFFFFFFp-27L,
   0x1p-27L, 0x1p-28L, 0x1p-29L, 0x1p-30L, 0x1p-31L, 0x1p-32L, 0x1p-33L,
   0x1p-34L, 0x1p-35L, 0x1p-36L, 0x1p-37L, 0x1p-38L, 0x1p-39L, 0x1p-40L,
   0x1p-41L, 0x1p-42L, 0x1p-43L, 0x1p-44L, 0x1p-45L, 0x1p-46L, 0x1p-47L,
   0x1p-48L, 0x1p-49L, 0x1p-50L, 0x1p-51L, 0x1p-52L, 0x1p-53L, 0x1p-54L,
   0x1p-55L, 0x1p-56L, 0x1p-57L, 0x1p-58L, 0x1p-59L, 0x1p-60L, 0x1p-61L,
   0x1p-62L, 0x1p-63L,
};


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//=========================================================
// Trigonometric functions.
//

//
// acos
//
double acos(double x)
{
   return atan2(sqrt((1 + x) * (1 - x)), x);
}

//
// acosf
//
float acosf(float x)
{
   return atan2f(sqrtf((1 + x) * (1 - x)), x);
}

//
// acosl
//
long double acosl(long double x)
{
   return atan2l(sqrtl((1 + x) * (1 - x)), x);
}

//
// asin
//
double asin(double x)
{
   return atan2(x, sqrt((1 + x) * (1 - x)));
}

//
// asinf
//
float asinf(float x)
{
   return atan2f(x, sqrtf((1 + x) * (1 - x)));
}

//
// asinl
//
long double asinl(long double x)
{
   return atan2l(x, sqrtl((1 + x) * (1 - x)));
}

//
// atan
//
double atan(double y)
{
   Domain_ArcTan();

   double x = 1, z = 0, x0;
   Cordic_ArcTan();

   return z;
}

//
// atanf
//
float atanf(float y)
{
   Domain_ArcTan(f);

   float x = 1, z = 0, x0;
   Cordic_ArcTan(f);

   return z;
}

//
// atanl
//
long double atanl(long double y)
{
   Domain_ArcTan(l);

   long double x = 1, z = 0, x0;
   Cordic_ArcTan(l);

   return z;
}

//
// atan2
//
double atan2(double y, double x)
{
   Domain_ArcTan2();

   double z = 0, x0;
   Cordic_ArcTan();

   return z;
}

//
// atan2f
//
float atan2f(float y, float x)
{
   Domain_ArcTan2(f);

   float z = 0, x0;
   Cordic_ArcTan(f);

   return z;
}

//
// atan2l
//
long double atan2l(long double y, long double x)
{
   Domain_ArcTan2(l);

   long double z = 0, x0;
   Cordic_ArcTan(l);

   return z;
}

//
// cos
//
double cos(double z)
{
   Domain_Cos();

   double x = 1, y = 0, x0;
   Cordic_SinCos();

   return x * SinCosScale;
}

//
// cosf
//
float cosf(float z)
{
   Domain_Cos(f);

   float x = 1, y = 0, x0;
   Cordic_SinCos(f);

   return x * SinCosScalef;
}

//
// cosl
//
long double cosl(long double z)
{
   Domain_Cos(l);

   long double x = 1, y = 0, x0;
   Cordic_SinCos(l);

   return x * SinCosScalel;
}

//
// sin
//
double sin(double z)
{
   Domain_Sin();

   double x = 1, y = 0, x0;
   Cordic_SinCos();

   return y * SinCosScale;
}

//
// sinf
//
float sinf(float z)
{
   Domain_Sin(f);

   float x = 1, y = 0, x0;
   Cordic_SinCos(f);

   return y * SinCosScalef;
}

//
// sinl
//
long double sinl(long double z)
{
   Domain_Sin(l);

   long double x = 1, y = 0, x0;
   Cordic_SinCos(l);

   return y * SinCosScalel;
}

//
// tan
//
double tan(double z)
{
   Domain_Tan();

   double x = 1, y = 0, x0;
   Cordic_SinCos();

   return y / x;
}

//
// tanf
//
float tanf(float z)
{
   Domain_Tan(f);

   float x = 1, y = 0, x0;
   Cordic_SinCos(f);

   return y / x;
}

//
// tanl
//
long double tanl(long double z)
{
   Domain_Tan(l);

   long double x = 1, y = 0, x0;
   Cordic_SinCos(l);

   return y / x;
}

//=========================================================
// Implementation extensions.
//

//
// __sincos
//
void __sincos(double z, double *sin, double *cos)
{
   Domain_SinCos();

   double x = 1, y = 0, x0;
   Cordic_SinCos();

   *sin = y * SinCosScale;
   *cos = x * SinCosScale;
}

//
// __sincosf
//
void __sincosf(float z, float *sin, float *cos)
{
   Domain_SinCos(f);

   float x = 1, y = 0, x0;
   Cordic_SinCos(f);

   *sin = y * SinCosScalef;
   *cos = x * SinCosScalef;
}

//
// __sincosl
//
void __sincosl(long double z, long double *sin, long double *cos)
{
   Domain_SinCos(l);

   long double x = 1, y = 0, x0;
   Cordic_SinCos(l);

   *sin = y * SinCosScalel;
   *cos = x * SinCosScalel;
}

// EOF

