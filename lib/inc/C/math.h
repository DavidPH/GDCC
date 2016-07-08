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
// Mathematics.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__math_h__
#define __GDCC_Header__C__math_h__

#include <bits/features.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// HUGE_VAL
//
#define HUGE_VAL  (__inf())
#define HUGE_VALF (__inff())
#define HUGE_VALL (__infl())

//
// INFINITY
//
#define INFINITY (__inf())

//
// NAN
//
#define NAN (__nanf())

//
// FP_*
//
#define FP_INFINITE  2
#define FP_NAN       1
#define FP_NORMAL    3
#define FP_SUBNORMAL 4
#define FP_ZERO      5

//
// FP_FAST_FMA
//
// These are conditionally defined.
//
//#define FP_FAST_FMA  1
//#define FP_FAST_FMAF 1
//#define FP_FAST_FMAL 1

//
// FP_ILOGB*
//
#define FP_ILOGB0   INT_MIN
#define FP_ILOGBNAN INT_MAX

//
// MATH_ERR*
//
#define MATH_ERRNO     1
#define MATH_ERREXCEPT 2

//
// math_errhandling
//
#define math_errhandling 0

//
// Classification macros.
//

//
// fpclassify
//
#define fpclassify(x) (__GDCC__TGMathR((x), __fpclassify)((x)))

//
// infinite
//
#define isfinite(x) (__GDCC__TGMathR((x), __isfinite)((x)))

//
// isinf
//
#define isinf(x) (__GDCC__TGMathR((x), __isinf)((x)))

//
// isnan
//
#define isnan(x) (__GDCC__TGMathR((x), __isnan)((x)))

//
// isnormal
//
#define isnormal(x) (__GDCC__TGMathR((x), __isnormal)((x)))

//
// signbit
//
#define signbit(x) (__GDCC__TGMathR((x), __signbit)((x)))

//
// Comparison macros.
//

//
// isgreater
//
#define isgreater(x, y) (__GDCC__TGMathR((x) + (y), __isgreater)((x), (y)))

//
// isgreaterequal
//
#define isgreaterequal(x, y) (__GDCC__TGMathR((x) + (y), __isgreaterequal)((x), (y)))

//
// isless
//
#define isless(x, y) (__GDCC__TGMathR((x) + (y), __isless)((x), (y)))

//
// islessequal
//
#define islessequal(x, y) (__GDCC__TGMathR((x) + (y), __islessequal)((x), (y)))

//
// islessgreater
//
#define islessgreater(x, y) (__GDCC__TGMathR((x) + (y), __islessgreater)((x), (y)))

//
// isunordered
//
#define isunordered(x, y) (__GDCC__TGMathR((x) + (y), __isunordered)((x), (y)))

//
// __GDCC__TGMathR
//
#define __GDCC__TGMathR(x, func) \
   (_Generic((x), \
      float:       func##f, \
      double:      func, \
      long double: func##l, \
      default: func))


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// float_t
//
typedef float float_t;

//
// double_t
//
typedef double double_t;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

#ifdef __cplusplus
extern "C" {
#endif

//
// Trigonometric functions.
//

double acos(double _x);
float acosf(float _x);
long double acosl(long double _x);

double asin(double _x);
float asinf(float _x);
long double asinl(long double _x);

double atan(double _x);
float atanf(float _x);
long double atanl(long double _x);

double atan2(double _y, double _x);
float atan2f(float _y, float _x);
long double atan2l(long double _y, long double _x);

double cos(double _x);
float cosf(float _x);
long double cosl(long double _x);

double sin(double _x);
float sinf(float _x);
long double sinl(long double _x);

double tan(double _x);
float tanf(float _x);
long double tanl(long double _x);

//
// Hyperbolic functions.
//

double acosh(double _x);
float acoshf(float _x);
long double acoshl(long double _x);

double asinh(double _x);
float asinhf(float _x);
long double asinhl(long double _x);

double atanh(double _x);
float atanhf(float _x);
long double atanhl(long double _x);

double cosh(double _x);
float coshf(float _x);
long double coshl(long double _x);

double sinh(double _x);
float sinhf(float _x);
long double sinhl(long double _x);

double tanh(double _x);
float tanhf(float _x);
long double tanhl(long double _x);

//
// Exponential and logarithmic functions.
//

double exp(double _x);
float expf(float _x);
long double expl(long double _x);

double exp2(double _x);
float exp2f(float _x);
long double exp2l(long double _x);

double expm1(double _x);
float expm1f(float _x);
long double expm1l(long double _x);

double frexp(double _value, int *_exp);
float frexpf(float _value, int *_exp);
long double frexpl(long double _value, int *_exp);

int ilogb(double _x);
int ilogbf(float _x);
int ilogbl(long double _x);

double ldexp(double _x, int _exp);
float ldexpf(float _x, int _exp);
long double ldexpl(long double _x, int _exp);

double log(double _x);
float logf(float _x);
long double logl(long double _x);

double log10(double _x);
float log10f(float _x);
long double log10l(long double _x);

double log1p(double _x);
float log1pf(float _x);
long double log1pl(long double _x);

double log2(double _x);
float log2f(float _x);
long double log2l(long double _x);

double logb(double _x);
float logbf(float _x);
long double logbl(long double _x);

double modf(double _value, double *_iptr);
float modff(float _value, float *_iptr);
long double modfl(long double _value, long double *_iptr);

double scalbn(double _x, int _n);
float scalbnf(float _x, int _n);
long double scalbnl(long double _x, int _n);
double scalbln(double _x, long int _n);
float scalblnf(float _x, long int _n);
long double scalblnl(long double _x, long int _n);

//
// Power and absolute-value functions.
//

double cbrt(double _x);
float cbrtf(float _x);
long double cbrtl(long double _x);

double fabs(double _x);
float fabsf(float _x);
long double fabsl(long double _x);

double hypot(double _x, double _y);
float hypotf(float _x, float _y);
long double hypotl(long double _x, long double _y);

double pow(double _x, double _y);
float powf(float _x, float _y);
long double powl(long double _x, long double _y);

double sqrt(double _x);
float sqrtf(float _x);
long double sqrtl(long double _x);

//
// Error and gamma functions.
//

double erf(double _x);
float erff(float _x);
long double erfl(long double _x);

double erfc(double _x);
float erfcf(float _x);
long double erfcl(long double _x);

double lgamma(double _x);
float lgammaf(float _x);
long double lgammal(long double _x);

double tgamma(double _x);
float tgammaf(float _x);
long double tgammal(long double _x);

//
// Nearest integer functions.
//

double ceil(double _x);
float ceilf(float _x);
long double ceill(long double _x);

double floor(double _x);
float floorf(float _x);
long double floorl(long double _x);

double nearbyint(double _x);
float nearbyintf(float _x);
long double nearbyintl(long double _x);

double rint(double _x);
float rintf(float _x);
long double rintl(long double _x);

long int lrint(double _x);
long int lrintf(float _x);
long int lrintl(long double _x);
long long int llrint(double _x);
long long int llrintf(float _x);
long long int llrintl(long double _x);

double round(double _x);
float roundf(float _x);
long double roundl(long double _x);

long int lround(double _x);
long int lroundf(float _x);
long int lroundl(long double _x);
long long int llround(double _x);
long long int llroundf(float _x);
long long int llroundl(long double _x);

double trunc(double _x);
float truncf(float _x);
long double truncl(long double _x);

//
// Remainder functions.
//

double fmod(double _x, double _y);
float fmodf(float _x, float _y);
long double fmodl(long double _x, long double _y);

double remainder(double _x, double _y);
float remainderf(float _x, float _y);
long double remainderl(long double _x, long double _y);

double remquo(double _x, double _y, int *_quo);
float remquof(float _x, float _y, int *_quo);
long double remquol(long double _x, long double _y, int *_quo);

//
// Manipulation functions.
//

double copysign(double _x, double _y);
float copysignf(float _x, float _y);
long double copysignl(long double _x, long double _y);

double nan(char const *_tagp);
float nanf(char const *_tagp);
long double nanl(char const *_tagp);

double nextafter(double _x, double _y);
float nextafterf(float _x, float _y);
long double nextafterl(long double _x, long double _y);

double nexttoward(double _x, long double _y);
float nexttowardf(float _x, long double _y);
long double nexttowardl(long double _x, long double _y);

//
// Maximum, minimum, and positive difference functions.
//

double fdim(double _x, double _y);
float fdimf(float _x, float _y);
long double fdiml(long double _x, long double _y);

double fmax(double _x, double _y);
float fmaxf(float _x, float _y);
long double fmaxl(long double _x, long double _y);

double fmin(double _x, double _y);
float fminf(float _x, float _y);
long double fminl(long double _x, long double _y);

//
// Floating multiply-add.
//

double fma(double _x, double _y, double _z);
float fmaf(float _x, float _y, float _z);
long double fmal(long double _x, long double _y, long double _z);

//
// GNU extensions.
//

#if defined(_GNU_SOURCE)
void sincos(double _x, double *_sin, double *_cos);
void sincosf(float _x, float *_sin, float *_cos);
void sincosl(long double _x, long double *_sin, long double *_cos);
#endif

//
// Implementation extensions.
//

double __rinf(double _x);
float __rinff(float _x);
long double __rinfl(long double _x);

int __irmod(double _x);
int __irmodf(float _x);
int __irmodl(long double _x);

[[call("StkCall")]] double __approx_cbrt(double _x);
[[call("StkCall")]] float __approx_cbrtf(float _x);
[[call("StkCall")]] long double __approx_cbrtl(long double _x);

[[call("StkCall")]] int __approx_log2(double _x);
[[call("StkCall")]] int __approx_log2f(float _x);
[[call("StkCall")]] int __approx_log2l(long double _x);

[[call("StkCall")]] double __approx_sqrt(double _x);
[[call("StkCall")]] float __approx_sqrtf(float _x);
[[call("StkCall")]] long double __approx_sqrtl(long double _x);

[[call("StkCall")]] double __inf(void);
[[call("StkCall")]] float __inff(void);
[[call("StkCall")]] long double __infl(void);

[[call("StkCall")]] double __nan(void);
[[call("StkCall")]] float __nanf(void);
[[call("StkCall")]] long double __nanl(void);

[[call("StkCall")]] int __fpclassify(double _x);
[[call("StkCall")]] int __fpclassifyf(float _x);
[[call("StkCall")]] int __fpclassifyl(long double _x);

[[call("StkCall")]] int __isfinite(double _x);
[[call("StkCall")]] int __isfinitef(float _x);
[[call("StkCall")]] int __isfinitel(long double _x);

[[call("StkCall")]] int __isinf(double _x);
[[call("StkCall")]] int __isinff(float _x);
[[call("StkCall")]] int __isinfl(long double _x);

[[call("StkCall")]] int __isnan(double _x);
[[call("StkCall")]] int __isnanf(float _x);
[[call("StkCall")]] int __isnanl(long double _x);

[[call("StkCall")]] int __isnormal(double _x);
[[call("StkCall")]] int __isnormalf(float _x);
[[call("StkCall")]] int __isnormall(long double _x);

[[call("StkCall")]] int __signbit(double _x);
[[call("StkCall")]] int __signbitf(float _x);
[[call("StkCall")]] int __signbitl(long double _x);

[[call("StkCall")]] int __isgreater(double _x, double _y);
[[call("StkCall")]] int __isgreaterf(float _x, float _y);
[[call("StkCall")]] int __isgreaterl(long double _x, long double _y);

[[call("StkCall")]] int __isgreaterequal(double _x, double _y);
[[call("StkCall")]] int __isgreaterequalf(float _x, float _y);
[[call("StkCall")]] int __isgreaterequall(long double _x, long double _y);

[[call("StkCall")]] int __isless(double _x, double _y);
[[call("StkCall")]] int __islessf(float _x, float _y);
[[call("StkCall")]] int __islessl(long double _x, long double _y);

[[call("StkCall")]] int __islessequal(double _x, double _y);
[[call("StkCall")]] int __islessequalf(float _x, float _y);
[[call("StkCall")]] int __islessequall(long double _x, long double _y);

[[call("StkCall")]] int __islessgreater(double _x, double _y);
[[call("StkCall")]] int __islessgreaterf(float _x, float _y);
[[call("StkCall")]] int __islessgreaterl(long double _x, long double _y);

[[call("StkCall")]] int __isunordered(double _x, double _y);
[[call("StkCall")]] int __isunorderedf(float _x, float _y);
[[call("StkCall")]] int __isunorderedl(long double _x, long double _y);

#ifdef __cplusplus
}
#endif

#endif//__GDCC_Header__C__math_h__

