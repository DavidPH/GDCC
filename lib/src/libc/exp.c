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
// Exponential and logarithmic functions.
//
//-----------------------------------------------------------------------------

#define __GDCC__DirectObject

#include <math.h>

#include <errno.h>
#include <limits.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define M_E  2.718281828459045235360287471352662498
#define M_Ef 2.718281828459045235360287471352662498F
#define M_El 2.718281828459045235360287471352662498L

#define M_LN2  0.693147180559945309417232121458176568
#define M_LN2f 0.693147180559945309417232121458176568F
#define M_LN2l 0.693147180559945309417232121458176568L

#define M_10LN2  0x1.A934F0979A371p+1
#define M_10LN2f 0x1.A934F0979A371p+1F
#define M_10LN2l 0x1.A934F0979A371p+1L

//
// Impl_Exp
//
#define Impl_Exp(base, suffix, ebit, eoff, emin, emax, itr) \
   int exp = u.i1 >> (ebit); \
   \
   if(exp >  (eoff) + (emax)) return __inf##suffix(); \
   if(exp <= (eoff) + (emin)) return Exp##base##Table##suffix[0]; \
   \
   exp -= (eoff) + (emin); \
   \
   res = Exp##base##Table##suffix[exp]; \
   for(int i = 0; i != (itr) && --exp; ++i) \
   { \
      u.i <<= 1; \
      if(u.i1 & (1 << (ebit))) \
         res *= Exp##base##Table##suffix[exp]; \
   }

//
// Impl_Log2
//
#define Impl_Log2(type, suffix, N) \
   if(isnan(x)) return x; \
   if(signbit(x)) {errno = EDOM; return __nan##suffix();} \
   if(isinf(x)) return x; \
   \
   if(x < 1) return -log2##suffix(1/x); \
   \
   if(x < 2) \
   { \
      type res = 0; \
      \
      x *= x; \
      for(int n = 1;;) \
      { \
         /* Find the next n for which x**2**n >= 2. */ \
         for(; x < 2; x *= x) \
             if(++n == N) return res; \
         \
         /* Set result bit. */ \
         res += 1.0##suffix >> n; \
         \
         /* Start over with x**2**n / 2. */ \
         x >>= 1; \
      } \
   } \
   else \
   { \
      int n = __approx_log2##suffix(x); \
      return n + log2##suffix(x >> n); \
   }


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// __frexp_res
//
struct __frexp_res  {     double value; int exp;};
struct __frexpf_res {     float  value; int exp;};
struct __frexpl_res {long double value; int exp;};

//
// __modf_res
//
struct __modf_res  {     double fval, ival;};
struct __modff_res {     float  fval, ival;};
struct __modfl_res {long double fval, ival;};


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

#if !__GDCC__NoFloat
//
// Exp2Table
//
static double const Exp2Table[] =
{
   0x1.0000000000000p+0,
   0x1.0000000000001p+0,
   0x1.0000000000001p+0,
   0x1.0000000000003p+0,
   0x1.0000000000006p+0,
   0x1.000000000000Bp+0,
   0x1.0000000000016p+0,
   0x1.000000000002Cp+0,
   0x1.0000000000059p+0,
   0x1.00000000000B1p+0,
   0x1.0000000000163p+0,
   0x1.00000000002C6p+0,
   0x1.000000000058Cp+0,
   0x1.0000000000B17p+0,
   0x1.000000000162Ep+0,
   0x1.0000000002C5Dp+0,
   0x1.00000000058B9p+0,
   0x1.000000000B172p+0,
   0x1.00000000162E4p+0,
   0x1.000000002C5C8p+0,
   0x1.0000000058B91p+0,
   0x1.00000000B1721p+0,
   0x1.0000000162E43p+0,
   0x1.00000002C5C86p+0,
   0x1.000000058B90Cp+0,
   0x1.0000000B17218p+0,
   0x1.000000162E430p+0,
   0x1.0000002C5C860p+0,
   0x1.00000058B90C1p+0,
   0x1.000000B172183p+0,
   0x1.00000162E430Ep+0,
   0x1.000002C5C863Bp+0,
   0x1.0000058B90CF2p+0,
   0x1.00000B1721BD0p+0,
   0x1.0000162E43F50p+0,
   0x1.00002C5C89D5Fp+0,
   0x1.000058B91B5BDp+0,
   0x1.0000B17255776p+0,
   0x1.000162E525EE0p+0,
   0x1.0002C5CC37DA9p+0,
   0x1.00058BA01FBA0p+0,
   0x1.000B175EFFDC7p+0,
   0x1.00162F3904052p+0,
   0x1.002C605E2E8CFp+0,
   0x1.0058C86DA1C0Ap+0,
   0x1.00B1AFA5ABCBFp+0,
   0x1.0163DA9FB3335p+0,
   0x1.02C9A3E778061p+0,
   0x1.059B0D3158574p+0,
   0x1.0B5586CF9890Fp+0,
   0x1.172B83C7D517Bp+0,
   0x1.306FE0A31B715p+0,
   0x1.6A09E667F3BCDp+0,
   0x1.0000000000000p+1,
   0x1.0000000000000p+2,
   0x1.0000000000000p+4,
   0x1.0000000000000p+8,
   0x1.0000000000000p+16,
   0x1.0000000000000p+32,
   0x1.0000000000000p+64,
   0x1.0000000000000p+128,
   0x1.0000000000000p+256,
   0x1.0000000000000p+512,
};

//
// Exp2Tablef
//
static float const Exp2Tablef[] =
{
   0x1.000000p+0F,
   0x1.000002p+0F,
   0x1.000002p+0F,
   0x1.000006p+0F,
   0x1.00000Cp+0F,
   0x1.000016p+0F,
   0x1.00002Cp+0F,
   0x1.000058p+0F,
   0x1.0000B2p+0F,
   0x1.000162p+0F,
   0x1.0002C6p+0F,
   0x1.00058Cp+0F,
   0x1.000B18p+0F,
   0x1.001630p+0F,
   0x1.002C60p+0F,
   0x1.0058C8p+0F,
   0x1.00B1B0p+0F,
   0x1.0163DAp+0F,
   0x1.02C9A4p+0F,
   0x1.059B0Ep+0F,
   0x1.0B5586p+0F,
   0x1.172B84p+0F,
   0x1.306FE0p+0F,
   0x1.6A09E6p+0F,
   0x1.000000p+1F,
   0x1.000000p+2F,
   0x1.000000p+4F,
   0x1.000000p+8F,
   0x1.000000p+16F,
   0x1.000000p+32F,
   0x1.000000p+64F,
};

//
// Exp2Tablel
//
static long double const Exp2Tablel[] =
{
   0x1.0000000000000p+0L,
   0x1.0000000000001p+0L,
   0x1.0000000000001p+0L,
   0x1.0000000000003p+0L,
   0x1.0000000000006p+0L,
   0x1.000000000000Bp+0L,
   0x1.0000000000016p+0L,
   0x1.000000000002Cp+0L,
   0x1.0000000000059p+0L,
   0x1.00000000000B1p+0L,
   0x1.0000000000163p+0L,
   0x1.00000000002C6p+0L,
   0x1.000000000058Cp+0L,
   0x1.0000000000B17p+0L,
   0x1.000000000162Ep+0L,
   0x1.0000000002C5Dp+0L,
   0x1.00000000058B9p+0L,
   0x1.000000000B172p+0L,
   0x1.00000000162E4p+0L,
   0x1.000000002C5C8p+0L,
   0x1.0000000058B91p+0L,
   0x1.00000000B1721p+0L,
   0x1.0000000162E43p+0L,
   0x1.00000002C5C86p+0L,
   0x1.000000058B90Cp+0L,
   0x1.0000000B17218p+0L,
   0x1.000000162E430p+0L,
   0x1.0000002C5C860p+0L,
   0x1.00000058B90C1p+0L,
   0x1.000000B172183p+0L,
   0x1.00000162E430Ep+0L,
   0x1.000002C5C863Bp+0L,
   0x1.0000058B90CF2p+0L,
   0x1.00000B1721BD0p+0L,
   0x1.0000162E43F50p+0L,
   0x1.00002C5C89D5Fp+0L,
   0x1.000058B91B5BDp+0L,
   0x1.0000B17255776p+0L,
   0x1.000162E525EE0p+0L,
   0x1.0002C5CC37DA9p+0L,
   0x1.00058BA01FBA0p+0L,
   0x1.000B175EFFDC7p+0L,
   0x1.00162F3904052p+0L,
   0x1.002C605E2E8CFp+0L,
   0x1.0058C86DA1C0Ap+0L,
   0x1.00B1AFA5ABCBFp+0L,
   0x1.0163DA9FB3335p+0L,
   0x1.02C9A3E778061p+0L,
   0x1.059B0D3158574p+0L,
   0x1.0B5586CF9890Fp+0L,
   0x1.172B83C7D517Bp+0L,
   0x1.306FE0A31B715p+0L,
   0x1.6A09E667F3BCDp+0L,
   0x1.0000000000000p+1L,
   0x1.0000000000000p+2L,
   0x1.0000000000000p+4L,
   0x1.0000000000000p+8L,
   0x1.0000000000000p+16L,
   0x1.0000000000000p+32L,
   0x1.0000000000000p+64L,
   0x1.0000000000000p+128L,
   0x1.0000000000000p+256L,
   0x1.0000000000000p+512L,
};

//
// ExpETable
//
static double const ExpETable[] =
{
   0x1.0000000000000p+0,
   0x1.0000000000001p+0,
   0x1.0000000000001p+0,
   0x1.0000000000002p+0,
   0x1.0000000000004p+0,
   0x1.0000000000008p+0,
   0x1.0000000000010p+0,
   0x1.0000000000020p+0,
   0x1.0000000000040p+0,
   0x1.0000000000080p+0,
   0x1.0000000000100p+0,
   0x1.0000000000200p+0,
   0x1.0000000000400p+0,
   0x1.0000000000800p+0,
   0x1.0000000001000p+0,
   0x1.0000000002000p+0,
   0x1.0000000004000p+0,
   0x1.0000000008000p+0,
   0x1.0000000010000p+0,
   0x1.0000000020000p+0,
   0x1.0000000040000p+0,
   0x1.0000000080000p+0,
   0x1.0000000100000p+0,
   0x1.0000000200000p+0,
   0x1.0000000400000p+0,
   0x1.0000000800000p+0,
   0x1.0000001000000p+0,
   0x1.0000002000000p+0,
   0x1.0000004000001p+0,
   0x1.0000008000002p+0,
   0x1.0000010000008p+0,
   0x1.0000020000020p+0,
   0x1.0000040000080p+0,
   0x1.0000080000200p+0,
   0x1.0000100000800p+0,
   0x1.0000200002000p+0,
   0x1.0000400008000p+0,
   0x1.0000800020000p+0,
   0x1.0001000080003p+0,
   0x1.0002000200015p+0,
   0x1.00040008000ABp+0,
   0x1.0008002000555p+0,
   0x1.0010008002AABp+0,
   0x1.0020020015560p+0,
   0x1.00400800AAB55p+0,
   0x1.0080200556001p+0,
   0x1.0100802AB5577p+0,
   0x1.0202015600446p+0,
   0x1.04080AB55DE39p+0,
   0x1.08205601127EDp+0,
   0x1.1082B577D34EDp+0,
   0x1.2216045B6F5CDp+0,
   0x1.48B5E3C3E8186p+0,
   0x1.A61298E1E069Cp+0,
   0x1.5BF0A8B145769p+1,
   0x1.D8E64B8D4DDAEp+2,
   0x1.B4C902E273A58p+5,
   0x1.749EA7D470C6Ep+11,
   0x1.0F2EBD0A80020p+23,
   0x1.1F43FCC4B662Cp+46,
   0x1.425982CF597CDp+92,
   0x1.95E54C5DD4217p+184,
   0x1.41C7A8814BEBAp+369,
   0x1.9476504BA852Ep+738,
};

//
// ExpETablef
//
static float const ExpETablef[] =
{
   0x1.000000p+0F,
   0x1.000002p+0F,
   0x1.000002p+0F,
   0x1.000004p+0F,
   0x1.000008p+0F,
   0x1.000010p+0F,
   0x1.000020p+0F,
   0x1.000040p+0F,
   0x1.000080p+0F,
   0x1.000100p+0F,
   0x1.000200p+0F,
   0x1.000400p+0F,
   0x1.000800p+0F,
   0x1.001000p+0F,
   0x1.002002p+0F,
   0x1.004008p+0F,
   0x1.008020p+0F,
   0x1.010080p+0F,
   0x1.020202p+0F,
   0x1.04080Ap+0F,
   0x1.082056p+0F,
   0x1.1082B6p+0F,
   0x1.221604p+0F,
   0x1.48B5E4p+0F,
   0x1.A61298p+0F,
   0x1.5BF0A8p+1F,
   0x1.D8E64Cp+2F,
   0x1.B4C902p+5F,
   0x1.749EA8p+11F,
   0x1.0F2EBEp+23F,
   0x1.1F43FCp+46F,
   0x1.425982p+92F,
};

//
// ExpETablel
//
static long double const ExpETablel[] =
{
   0x1.0000000000000p+0L,
   0x1.0000000000001p+0L,
   0x1.0000000000001p+0L,
   0x1.0000000000002p+0L,
   0x1.0000000000004p+0L,
   0x1.0000000000008p+0L,
   0x1.0000000000010p+0L,
   0x1.0000000000020p+0L,
   0x1.0000000000040p+0L,
   0x1.0000000000080p+0L,
   0x1.0000000000100p+0L,
   0x1.0000000000200p+0L,
   0x1.0000000000400p+0L,
   0x1.0000000000800p+0L,
   0x1.0000000001000p+0L,
   0x1.0000000002000p+0L,
   0x1.0000000004000p+0L,
   0x1.0000000008000p+0L,
   0x1.0000000010000p+0L,
   0x1.0000000020000p+0L,
   0x1.0000000040000p+0L,
   0x1.0000000080000p+0L,
   0x1.0000000100000p+0L,
   0x1.0000000200000p+0L,
   0x1.0000000400000p+0L,
   0x1.0000000800000p+0L,
   0x1.0000001000000p+0L,
   0x1.0000002000000p+0L,
   0x1.0000004000001p+0L,
   0x1.0000008000002p+0L,
   0x1.0000010000008p+0L,
   0x1.0000020000020p+0L,
   0x1.0000040000080p+0L,
   0x1.0000080000200p+0L,
   0x1.0000100000800p+0L,
   0x1.0000200002000p+0L,
   0x1.0000400008000p+0L,
   0x1.0000800020000p+0L,
   0x1.0001000080003p+0L,
   0x1.0002000200015p+0L,
   0x1.00040008000ABp+0L,
   0x1.0008002000555p+0L,
   0x1.0010008002AABp+0L,
   0x1.0020020015560p+0L,
   0x1.00400800AAB55p+0L,
   0x1.0080200556001p+0L,
   0x1.0100802AB5577p+0L,
   0x1.0202015600446p+0L,
   0x1.04080AB55DE39p+0L,
   0x1.08205601127EDp+0L,
   0x1.1082B577D34EDp+0L,
   0x1.2216045B6F5CDp+0L,
   0x1.48B5E3C3E8186p+0L,
   0x1.A61298E1E069Cp+0L,
   0x1.5BF0A8B145769p+1L,
   0x1.D8E64B8D4DDAEp+2L,
   0x1.B4C902E273A58p+5L,
   0x1.749EA7D470C6Ep+11L,
   0x1.0F2EBD0A80020p+23L,
   0x1.1F43FCC4B662Cp+46L,
   0x1.425982CF597CDp+92L,
   0x1.95E54C5DD4217p+184L,
   0x1.41C7A8814BEBAp+369L,
   0x1.9476504BA852Ep+738L,
};
#endif // !__GDCC__NoFloat


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

[[call("StkCall")]] struct __frexp_res  __approx_frexp(double value);
[[call("StkCall")]] struct __frexpf_res __approx_frexpf(float value);
[[call("StkCall")]] struct __frexpl_res __approx_frexpl(long double value);

[[call("StkCall")]] struct __modf_res  __approx_modf(double value);
[[call("StkCall")]] struct __modff_res __approx_modff(float value);
[[call("StkCall")]] struct __modfl_res __approx_modfl(long double value);

//=========================================================
// Exponential and logarithmic functions.
//

#if !__GDCC__NoFloat
//
// exp
//
double exp(double x)
{
   if(isnan(x))   return x;
   if(signbit(x)) return 1 / exp(-x);

   union {double f; long i; struct {int i0, i1;};} u = {x};
   double res;

   Impl_Exp(E, , 20, 1023, -54, 9, 52);

   return res;
}

//
// expf
//
float expf(float x)
{
   if(isnan(x))   return x;
   if(signbit(x)) return 1 / exp(-x);

   union {float f; int i; struct {int i1;};} u = {x};
   float res;

   Impl_Exp(E, f, 23, 127, -25, 7, 23);

   return res;
}

//
// expl
//
long double expl(long double x)
{
   if(isnan(x))   return x;
   if(signbit(x)) return 1 / expl(-x);

   union {long double f; long i; struct {int i0, i1;};} u = {x};
   long double res;

   Impl_Exp(E, l, 20, 1023, -54, 9, 52);

   return res;
}

//
// exp2
//
double exp2(double x)
{
   if(isnan(x))   return x;
   if(signbit(x)) return 1 / exp2(-x);

   union {double f; long i; struct {int i0, i1;};} u = {x};
   double res;

   Impl_Exp(2, , 20, 1023, -53, 9, 52);

   return res;
}

//
// exp2f
//
float exp2f(float x)
{
   if(isnan(x))   return x;
   if(signbit(x)) return 1 / exp2f(-x);

   union {float f; long i; struct {int i1;};} u = {x};
   float res;

   Impl_Exp(2, f, 23, 127, -24, 7, 23);

   return res;
}

//
// exp2l
//
long double exp2l(long double x)
{
   if(isnan(x))   return x;
   if(signbit(x)) return 1 / exp2l(-x);

   union {long double f; long i; struct {int i0, i1;};} u = {x};
   long double res;

   Impl_Exp(2, l, 20, 1023, -53, 9, 52);

   return res;
}

//
// expm1
//
double expm1(double x)
{
   return exp(x) - 1;
}

//
// expm1
//
float expm1f(float x)
{
   return expf(x) - 1;
}

//
// expm1
//
long double expm1l(long double x)
{
   return expl(x) - 1;
}

//
// frexp
//
double frexp(double value, int *exp)
{
   if(isnan(value)) return *exp = FP_ILOGBNAN, value;
   if(isinf(value)) return *exp = INT_MAX, value;
   if(!value) return *exp = 0, value;

   struct __frexp_res res = __approx_frexp(value);
   return *exp = res.exp, res.value;
}

//
// frexpf
//
float frexpf(float value, int *exp)
{
   if(isnan(value)) return *exp = FP_ILOGBNAN, value;
   if(isinf(value)) return *exp = INT_MAX, value;
   if(!value) return *exp = 0, value;

   struct __frexpf_res res = __approx_frexpf(value);
   return *exp = res.exp, res.value;
}

//
// frexpl
//
long double frexpl(long double value, int *exp)
{
   if(isnan(value)) return *exp = FP_ILOGBNAN, value;
   if(isinf(value)) return *exp = INT_MAX, value;
   if(!value) return *exp = 0, value;

   struct __frexpl_res res = __approx_frexpl(value);
   return *exp = res.exp, res.value;
}

//
// ilogb
//
int ilogb(double x)
{
   if(isnan(x)) return FP_ILOGBNAN;
   if(isinf(x)) return INT_MAX;
   if(!x) return FP_ILOGB0;

   return __approx_log2(x);
}

//
// ilogbf
//
int ilogbf(float x)
{
   if(isnan(x)) return FP_ILOGBNAN;
   if(isinf(x)) return INT_MAX;
   if(!x) return FP_ILOGB0;

   return __approx_log2f(x);
}

//
// ilogbl
//
int ilogbl(long double x)
{
   if(isnan(x)) return FP_ILOGBNAN;
   if(isinf(x)) return INT_MAX;
   if(!x) return FP_ILOGB0;

   return __approx_log2l(x);
}

//
// ldexp
//
double ldexp(double x, int exp)
{
   return exp < 0 ? x >> -exp : x << exp;
}

//
// ldexpf
//
float ldexpf(float x, int exp)
{
   return exp < 0 ? x >> -exp : x << exp;
}

//
// ldexpl
//
long double ldexpl(long double x, int exp)
{
   return exp < 0 ? x >> -exp : x << exp;
}

//
// log
//
double log(double x)
{
   return log2(x) * M_LN2;
}

//
// logf
//
float logf(float x)
{
   return log2f(x) * M_LN2f;
}

//
// logl
//
long double logl(long double x)
{
   return log2l(x) * M_LN2l;
}

//
// log10
//
double log10(double x)
{
   return log2(x) * (1 / M_10LN2);
}

//
// log10f
//
float log10f(float x)
{
   return log2(x) * (1 / M_10LN2f);
}

//
// log10l
//
long double log10l(long double x)
{
   return log2(x) * (1 / M_10LN2l);
}

//
// log1p
//
double log1p(double x)
{
   return log(1 + x);
}

//
// log1pf
//
float log1pf(float x)
{
   return log(1 + x);
}

//
// log1pl
//
long double log1pl(long double x)
{
   return log(1 + x);
}

//
// log2
//
double log2(double x)
{
   Impl_Log2(double, , 53);
}

//
// log2f
//
float log2f(float x)
{
   Impl_Log2(float, f, 24);
}

//
// log2l
//
long double log2l(long double x)
{
   Impl_Log2(long double, l, 53);
}

//
// logb
//
double logb(double x)
{
   if(isnan(x)) return x;
   if(isinf(x)) return x;
   if(!x) {errno = ERANGE; return 0;}

   return __approx_log2(x);
}

//
// logbf
//
float logbf(float x)
{
   if(isnan(x)) return x;
   if(isinf(x)) return x;
   if(!x) {errno = ERANGE; return 0;}

   return __approx_log2f(x);
}

//
// logbl
//
long double logbl(long double x)
{
   if(isnan(x)) return x;
   if(isinf(x)) return x;
   if(!x) {errno = ERANGE; return 0;}

   return __approx_log2l(x);
}

//
// modf
//
double modf(double value, double *iptr)
{
   if(isnan(value) || !value) return *iptr = value;

   struct __modf_res res = __approx_modf(value);
   return *iptr = res.ival, res.fval;
}

//
// modff
//
float modff(float value, float *iptr)
{
   if(isnan(value) || !value) return *iptr = value;

   struct __modff_res res = __approx_modff(value);
   return *iptr = res.ival, res.fval;
}

//
// modfl
//
long double modfl(long double value, long double *iptr)
{
   if(isnan(value) || !value) return *iptr = value;

   struct __modfl_res res = __approx_modfl(value);
   return *iptr = res.ival, res.fval;
}

//
// scalbn
//
double scalbn(double x, int n)
{
   return n < 0 ? x >> -n : x << n;
}

//
// scalbnf
//
float scalbnf(float x, int n)
{
   return n < 0 ? x >> -n : x << n;
}

//
// scalbnl
//
long double scalbnl(long double x, int n)
{
   return n < 0 ? x >> -n : x << n;
}

//
// scalbln
//
double scalbln(double x, long int n)
{
   return n < 0 ? x >> -n : x << n;
}

//
// scalblnf
//
float scalblnf(float x, long int n)
{
   return n < 0 ? x >> -n : x << n;
}

//
// scalblnl
//
long double scalblnl(long double x, long int n)
{
   return n < 0 ? x >> -n : x << n;
}

#endif // !__GDCC__NoFloat

// EOF

