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
// Mathematics.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__math_h__
#define __GDCC_Header__C__math_h__


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

double acos(double x);
float acosf(float x);
long double acosl(long double x);

double asin(double x);
float asinf(float x);
long double asinl(long double x);

double atan(double x);
float atanf(float x);
long double atanl(long double x);

double atan2(double y, double x);
float atan2f(float y, float x);
long double atan2l(long double y, long double x);

double cos(double x);
float cosf(float x);
long double cosl(long double x);

double sin(double x);
float sinf(float x);
long double sinl(long double x);

double tan(double x);
float tanf(float x);
long double tanl(long double x);

//
// Hyperbolic functions.
//

double acosh(double x);
float acoshf(float x);
long double acoshl(long double x);

double asinh(double x);
float asinhf(float x);
long double asinhl(long double x);

double atanh(double x);
float atanhf(float x);
long double atanhl(long double x);

double cosh(double x);
float coshf(float x);
long double coshl(long double x);

double sinh(double x);
float sinhf(float x);
long double sinhl(long double x);

double tanh(double x);
float tanhf(float x);
long double tanhl(long double x);

//
// Exponential and logarithmic functions.
//

double exp(double x);
float expf(float x);
long double expl(long double x);

double exp2(double x);
float exp2f(float x);
long double exp2l(long double x);

double expm1(double x);
float expm1f(float x);
long double expm1l(long double x);

double frexp(double value, int *exp);
float frexpf(float value, int *exp);
long double frexpl(long double value, int *exp);

int ilogb(double x);
int ilogbf(float x);
int ilogbl(long double x);

double ldexp(double x, int exp);
float ldexpf(float x, int exp);
long double ldexpl(long double x, int exp);

double log(double x);
float logf(float x);
long double logl(long double x);

double log10(double x);
float log10f(float x);
long double log10l(long double x);

double log1p(double x);
float log1pf(float x);
long double log1pl(long double x);

double log2(double x);
float log2f(float x);
long double log2l(long double x);

double logb(double x);
float logbf(float x);
long double logbl(long double x);

double modf(double value, double *iptr);
float modff(float value, float *iptr);
long double modfl(long double value, long double *iptr);

double scalbn(double x, int n);
float scalbnf(float x, int n);
long double scalbnl(long double x, int n);
double scalbln(double x, long int n);
float scalblnf(float x, long int n);
long double scalblnl(long double x, long int n);

//
// Power and absolute-value functions.
//

double cbrt(double x);
float cbrtf(float x);
long double cbrtl(long double x);

double fabs(double x);
float fabsf(float x);
long double fabsl(long double x);

double hypot(double x, double y);
float hypotf(float x, float y);
long double hypotl(long double x, long double y);

double pow(double x, double y);
float powf(float x, float y);
long double powl(long double x, long double y);

double sqrt(double x);
float sqrtf(float x);
long double sqrtl(long double x);

//
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

//
// Nearest integer functions.
//

double ceil(double x);
float ceilf(float x);
long double ceill(long double x);

double floor(double x);
float floorf(float x);
long double floorl(long double x);

double nearbyint(double x);
float nearbyintf(float x);
long double nearbyintl(long double x);

double rint(double x);
float rintf(float x);
long double rintl(long double x);

long int lrint(double x);
long int lrintf(float x);
long int lrintl(long double x);
long long int llrint(double x);
long long int llrintf(float x);
long long int llrintl(long double x);

double round(double x);
float roundf(float x);
long double roundl(long double x);

long int lround(double x);
long int lroundf(float x);
long int lroundl(long double x);
long long int llround(double x);
long long int llroundf(float x);
long long int llroundl(long double x);

double trunc(double x);
float truncf(float x);
long double truncl(long double x);

//
// Remainder functions.
//

double fmod(double x, double y);
float fmodf(float x, float y);
long double fmodl(long double x, long double y);

double remainder(double x, double y);
float remainderf(float x, float y);
long double remainderl(long double x, long double y);

double remquo(double x, double y, int *quo);
float remquof(float x, float y, int *quo);
long double remquol(long double x, long double y, int *quo);

//
// Manipulation functions.
//

double copysign(double x, double y);
float copysignf(float x, float y);
long double copysignl(long double x, long double y);

double nan(char const *tagp);
float nanf(char const *tagp);
long double nanl(char const *tagp);

double nextafter(double x, double y);
float nextafterf(float x, float y);
long double nextafterl(long double x, long double y);

double nexttoward(double x, long double y);
float nexttowardf(float x, long double y);
long double nexttowardl(long double x, long double y);

//
// Maximum, minimum, and positive difference functions.
//

double fdim(double x, double y);
float fdimf(float x, float y);
long double fdiml(long double x, long double y);

double fmax(double x, double y);
float fmaxf(float x, float y);
long double fmaxl(long double x, long double y);

double fmin(double x, double y);
float fminf(float x, float y);
long double fminl(long double x, long double y);

//
// Floating multiply-add.
//

double fma(double x, double y, double z);
float fmaf(float x, float y, float z);
long double fmal(long double x, long double y, long double z);

//
// Implementation extensions.
//

double __rinf(double x);
float __rinff(float x);
long double __rinfl(long double x);

int __irmod(double x);
int __irmodf(float x);
int __irmodl(long double x);

void __sincos(double x, double *sin, double *cos);
void __sincosf(float x, float *sin, float *cos);
void __sincosl(long double x, long double *sin, long double *cos);

[[call("StkCall")]] double __approx_cbrt(double x);
[[call("StkCall")]] float __approx_cbrtf(float x);
[[call("StkCall")]] long double __approx_cbrtl(long double x);

[[call("StkCall")]] int __approx_log2(double x);
[[call("StkCall")]] int __approx_log2f(float x);
[[call("StkCall")]] int __approx_log2l(long double x);

[[call("StkCall")]] double __approx_sqrt(double x);
[[call("StkCall")]] float __approx_sqrtf(float x);
[[call("StkCall")]] long double __approx_sqrtl(long double x);

[[call("StkCall")]] double __inf(void);
[[call("StkCall")]] float __inff(void);
[[call("StkCall")]] long double __infl(void);

[[call("StkCall")]] double __nan(void);
[[call("StkCall")]] float __nanf(void);
[[call("StkCall")]] long double __nanl(void);

[[call("StkCall")]] int __fpclassify(double x);
[[call("StkCall")]] int __fpclassifyf(float x);
[[call("StkCall")]] int __fpclassifyl(long double x);

[[call("StkCall")]] int __isfinite(double x);
[[call("StkCall")]] int __isfinitef(float x);
[[call("StkCall")]] int __isfinitel(long double x);

[[call("StkCall")]] int __isinf(double x);
[[call("StkCall")]] int __isinff(float x);
[[call("StkCall")]] int __isinfl(long double x);

[[call("StkCall")]] int __isnan(double x);
[[call("StkCall")]] int __isnanf(float x);
[[call("StkCall")]] int __isnanl(long double x);

[[call("StkCall")]] int __isnormal(double x);
[[call("StkCall")]] int __isnormalf(float x);
[[call("StkCall")]] int __isnormall(long double x);

[[call("StkCall")]] int __signbit(double x);
[[call("StkCall")]] int __signbitf(float x);
[[call("StkCall")]] int __signbitl(long double x);

[[call("StkCall")]] int __isgreater(double x, double y);
[[call("StkCall")]] int __isgreaterf(float x, float y);
[[call("StkCall")]] int __isgreaterl(long double x, long double y);

[[call("StkCall")]] int __isgreaterequal(double x, double y);
[[call("StkCall")]] int __isgreaterequalf(float x, float y);
[[call("StkCall")]] int __isgreaterequall(long double x, long double y);

[[call("StkCall")]] int __isless(double x, double y);
[[call("StkCall")]] int __islessf(float x, float y);
[[call("StkCall")]] int __islessl(long double x, long double y);

[[call("StkCall")]] int __islessequal(double x, double y);
[[call("StkCall")]] int __islessequalf(float x, float y);
[[call("StkCall")]] int __islessequall(long double x, long double y);

[[call("StkCall")]] int __islessgreater(double x, double y);
[[call("StkCall")]] int __islessgreaterf(float x, float y);
[[call("StkCall")]] int __islessgreaterl(long double x, long double y);

[[call("StkCall")]] int __isunordered(double x, double y);
[[call("StkCall")]] int __isunorderedf(float x, float y);
[[call("StkCall")]] int __isunorderedl(long double x, long double y);

#ifdef __cplusplus
}
#endif

#endif//__GDCC_Header__C__math_h__

