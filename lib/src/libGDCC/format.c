//-----------------------------------------------------------------------------
//
// Copyright(C) 2014 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Number to string formatting functions.
//
//-----------------------------------------------------------------------------

#include <GDCC.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define BufferLen 8192

//
// FormatFixCoreD
//
#define FormatFixCoreD(T, op) \
   /* Perform core conversion. */ \
   FormatFixCoreDI(T); \
   FormatFixCoreDF(op); \
   \
   ret.len = ret.end - ret.begin;

//
// FormatFixCoreDF
//
#define FormatFixCoreDF(op) \
   /* Fractional conversion. */ \
   if(format.prec) \
   { \
      *ret.end++ = '.'; \
      \
      unsigned precDigs = format.prec; \
      \
      FormatFixCoreDFLoop_##op(); \
      \
      for(; precDigs; --precDigs) \
         *ret.end++ = '0'; \
   } \
   else if(format.flags & __GDCC__FormatFlag_AltF) \
      *ret.end++ = '.';

//
// FormatFixCoreDFLoop_k
//
#if __GDCC_Target__ZDoom__
#define FormatFixCoreDFLoop_k() \
   for(union {_Accum k; int i;} u = {data}; u.i && precDigs; --precDigs) \
   { \
      u.i &= 0xFFFF; \
      u.i *= 10; \
      *ret.end++ = (u.i >> 16) + '0'; \
   }
#else
#define FormatFixCoreDFLoop_k() \
   for(_Accum k = data; k && precDigs; --precDigs) \
   { \
      k -= (int)k; \
      k *= 10; \
      *ret.end++ = (int)k + '0'; \
   }
#endif

//
// FormatFixCoreDFLoop_kl
//
#if __GDCC_Target__ZDoom__
#define FormatFixCoreDFLoop_kl() \
   for(union {long _Accum k; int i;} u = {data}; u.i && precDigs; --precDigs) \
   { \
      *ret.end++ = ((int)(((unsigned)u.i >> 24) * 10) >> 8) + '0'; \
      u.i *= 10; \
   }
#else
#define FormatFixCoreDFLoop_kl() \
   for(long _Accum k = data; k && precDigs; --precDigs) \
   { \
      k -= (long int)k; \
      k *= 10; \
      *ret.end++ = (int)k + '0'; \
   }
#endif

//
// FormatFixCoreDI
//
#define FormatFixCoreDI(T) \
   /* Integral conversion. */ \
   for(T int i = data; i;) \
   { \
      T __div_t d = __div(i, 10); \
      *--ret.begin = d.rem + '0'; \
      i = d.quot; \
   } \
   \
   if(ret.begin == ret.end && \
      (format.prec || !(format.flags & __GDCC__FormatFlag_AltF))) \
      *--ret.begin = '0';

//
// FormatFixPreS
//
#define FormatFixPreS() \
   __GDCC__FormatRet ret; \
   ret.begin = ret.end = Buffer + BufferLen / 2; \
   \
   if(format.prec == -1) format.prec = 6; \
   \
   char sign; \
   if(data < 0) \
   { \
      sign = '-'; \
      data = -data; \
   } \
   else if(format.flags & __GDCC__FormatFlag_Sign) \
      sign = '+'; \
   else if(format.flags & __GDCC__FormatFlag_PadS) \
      sign = ' '; \
   else \
      sign = '\0';

//
// FormatFixPreU
//
#define FormatFixPreU() \
   __GDCC__FormatRet ret; \
   ret.begin = ret.end = Buffer + BufferLen / 2; \
   \
   if(format.prec == -1) format.prec = 6; \
   \
   char const sign = '\0';

//
// FormatFixWidth
//
#define FormatFixWidth() \
   /* Pad to width. */ \
   if(format.flags & __GDCC__FormatFlag_Pad0 && \
      !(format.flags & __GDCC__FormatFlag_Left)) \
   { \
      if(sign && format.width) --format.width; \
      \
      while(ret.len < format.width) \
         *--ret.begin = '0', ++ret.len; \
   }

//
// FormatFltCore_f
//
#define FormatFltCore_f(bufINF, bufNaN) \
   union {float f; int i0;} u = {data}; \
   \
   FormatFltCoreSign(u.i0); \
   \
   int exp = (u.i0 >> 23) - 127; \
   u.i0 &= 0x007FFFFF; \
   \
   FormatFltCoreSpec(128, u.i0, bufINF, bufNaN);

//
// FormatFltCore_fl
//
#define FormatFltCore_fl(bufINF, bufNaN) \
   union {double f; struct {int i0, i1;};} u = {data}; \
   \
   FormatFltCoreSign(u.i1); \
   \
   int exp = (u.i1 >> 20) - 1023; \
   u.i1 &= 0x000FFFFF; \
   \
   FormatFltCoreSpec(1024, u.i0 | u.i1, bufINF, bufNaN);

//
// FormatFltCoreSign
//
#define FormatFltCoreSign(signWord) \
   char sign; \
   if((signWord) & 0x80000000) \
   { \
      sign = '-'; \
      (signWord) &= 0x7FFFFFFF; \
   } \
   else if(format.flags & __GDCC__FormatFlag_Sign) \
      sign = '+'; \
   else if(format.flags & __GDCC__FormatFlag_PadS) \
      sign = ' '; \
   else \
      sign = '\0';

//
// FormatFltCoreSpec
//
#define FormatFltCoreSpec(expMax, nanWord, bufINF, bufNaN) \
   /* Check for NaN or infinity. */ \
   if(exp == expMax) \
   { \
      /* NaN */ \
      if(nanWord) \
         ret.begin = bufNaN + 1, ret.end = bufNaN + 4, ret.len = 3; \
      \
      /* infinity */ \
      else \
         ret.begin = bufINF + 1, ret.end = bufINF + 9, ret.len = 8; \
      \
      if(sign) *--ret.begin = sign, ++ret.len; \
      \
      return ret; \
   }

//
// FormatFltCoreX
//
#define FormatFltCoreX(op, table, c, bufINF, bufNaN) \
   FormatFltCore_##op(bufINF, bufNaN); \
   \
   /* Perform core conversion. */ \
   \
   FormatFltCoreXI_##op(); \
   FormatFltCoreXF_##op(table); \
   FormatFltCoreXE_##op(c); \
   \
   ret.len = ret.end - ret.begin; \
   \
   if(format.flags & __GDCC__FormatFlag_Pad0 && \
      !(format.flags & __GDCC__FormatFlag_Left)) \
   { \
      ret.len += 2; \
   }

//
// FormatFltCoreXE
//
#define FormatFltCoreXE(c) \
   *ret.end++ = (c); \
   if(exp < 0) \
   { \
      exp = -exp; \
      *ret.end++ = '-'; \
   } \
   else \
      *ret.end++ = '+'; \

//
// FormatFltCoreXE_f
//
#define FormatFltCoreXE_f(c) \
   /* Perform core exponent conversion. */ \
   if(exp == -127) \
      *ret.end++ = (c), *ret.end++ = '+', *ret.end++ = '0'; \
   else \
   { \
      FormatFltCoreXE(c); \
      if(exp >= 100) {*ret.end++ = '1';} \
      if(exp >=  10) {*ret.end++ = exp / 10 % 10 + '0';} \
                     {*ret.end++ = exp      % 10 + '0';} \
   }

//
// FormatFltCoreXE_fl
//
#define FormatFltCoreXE_fl(c) \
   /* Perform core exponent conversion. */ \
   if(exp == -1023) \
      *ret.end++ = (c), *ret.end++ = '+', *ret.end++ = '0'; \
   else \
   { \
      FormatFltCoreXE(c); \
      if(exp >= 1000) {*ret.end++ = '1';} \
      if(exp >=  100) {*ret.end++ = exp / 100 % 10 + '0';} \
      if(exp >=   10) {*ret.end++ = exp /  10 % 10 + '0';} \
                      {*ret.end++ = exp       % 10 + '0';} \
   }

//
// FormatFltCoreXF_f
//
#define FormatFltCoreXF_f(table) \
   /* Perform core fractional conversion. */ \
   if(format.prec) \
   { \
      *ret.end++ = '.'; \
      for(; format.prec; --format.prec) \
      { \
         *ret.end++ = table[(u.i0 >> 19) & 0xF]; \
         u.i0 <<= 4; \
      } \
   } \
   else if(format.flags & __GDCC__FormatFlag_AltF) \
      *ret.end++ = '.';

//
// FormatFltCoreXF_fl
//
#define FormatFltCoreXF_fl(table) \
   /* Perform core fractional conversion. */ \
   if(format.prec) \
   { \
      *ret.end++ = '.'; \
      \
      for(int i = 5; i && format.prec; --i, --format.prec) \
      { \
         *ret.end++ = table[(u.i1 >> 16) & 0xF]; \
         u.i1 <<= 4; \
      } \
      \
      for(; format.prec; --format.prec) \
      { \
         *ret.end++ = table[(u.i0 >> 28) & 0xF]; \
         u.i0 <<= 4; \
      } \
   } \
   else if(format.flags & __GDCC__FormatFlag_AltF) \
      *ret.end++ = '.';

//
// FormatFltCoreXI_f
//
#define FormatFltCoreXI_f() \
   /* Perform core integral conversion. */ \
   *--ret.begin = '0' + (exp != -127);

//
// FormatFltCoreXI_fl
//
#define FormatFltCoreXI_fl() \
   /* Perform core integral conversion. */ \
   *--ret.begin = '0' + (exp != -1023);

//
// FormatFltPreS
//
#define FormatFltPreS(defPrec) \
   __GDCC__FormatRet ret; \
   ret.begin = ret.end = Buffer + BufferLen / 2; \
   \
   if(format.prec == -1) format.prec = defPrec;

//
// FormatFltPrefixX
//
#define FormatFltPrefixX(c) \
   *--ret.begin = (c); \
   *--ret.begin = '0'; \
   \
   if(!(format.flags & __GDCC__FormatFlag_Pad0) || \
      format.flags & __GDCC__FormatFlag_Left) \
   { \
      ret.len += 2; \
   }

//
// FormatIntAltB
//
#define FormatIntAltB(c) \
   if(format.flags & __GDCC__FormatFlag_AltF) \
   { \
      *--ret.begin = (c); \
      *--ret.begin = '0'; \
      \
      if(!(format.flags & __GDCC__FormatFlag_Pad0) || \
         format.flags & __GDCC__FormatFlag_Left) \
      { \
         ret.len += 2; \
      } \
   }

//
// FormatIntAltX
//
#define FormatIntAltX(c) \
   if(format.flags & __GDCC__FormatFlag_AltF) \
   { \
      *--ret.begin = (c); \
      *--ret.begin = '0'; \
      \
      if(!(format.flags & __GDCC__FormatFlag_Pad0) || \
         format.flags & __GDCC__FormatFlag_Left) \
      { \
         ret.len += 2; \
      } \
   }

//
// FormatIntCoreB
//
#define FormatIntCoreB() \
   /* Perform core conversion. */ \
   for(; data; data >>= 1) \
      *--ret.begin = ((int)data & 1) + '0'; \
   \
   ret.len = ret.end - ret.begin; \
   \
   if(format.flags & __GDCC__FormatFlag_AltF && \
      format.flags & __GDCC__FormatFlag_Pad0 && \
      !(format.flags & __GDCC__FormatFlag_Left)) \
   { \
      ret.len += 2; \
   }

//
// FormatIntCoreD
//
#define FormatIntCoreD(T) \
   /* Perform core conversion. */ \
   while(data) \
   { \
      T __div_t d = __div(data, 10); \
      *--ret.begin = (int)d.rem + '0'; \
      data = d.quot; \
   } \
   \
   ret.len = ret.end - ret.begin;

//
// FormatIntCoreO
//
#define FormatIntCoreO() \
   /* Perform core conversion. */ \
   for(; data; data >>= 3) \
      *--ret.begin = ((int)data & 7) + '0'; \
   \
   if(format.flags & __GDCC__FormatFlag_AltF) \
   { \
      if(ret.begin == ret.end || *ret.begin != '0') \
         *--ret.begin = '0'; \
   } \
   \
   ret.len = ret.end - ret.begin;

//
// FormatIntCoreX
//
#define FormatIntCoreX(table) \
   /* Perform core conversion. */ \
   for(; data; data >>= 4) \
      *--ret.begin = table[(int)data & 0xF]; \
   \
   ret.len = ret.end - ret.begin; \
   \
   if(format.flags & __GDCC__FormatFlag_AltF && \
      format.flags & __GDCC__FormatFlag_Pad0 && \
      !(format.flags & __GDCC__FormatFlag_Left)) \
   { \
      ret.len += 2; \
   }

//
// FormatIntPreS
//
#define FormatIntPreS() \
   __GDCC__FormatRet ret; \
   ret.begin = ret.end = Buffer + BufferLen / 2; \
   \
   char sign; \
   if(data < 0) \
   { \
      sign = '-'; \
      data = -data; \
   } \
   else if(format.flags & __GDCC__FormatFlag_Sign) \
      sign = '+'; \
   else if(format.flags & __GDCC__FormatFlag_PadS) \
      sign = ' '; \
   else \
      sign = '\0';

//
// FormatIntPreU
//
#define FormatIntPreU() \
   __GDCC__FormatRet ret; \
   ret.begin = ret.end = Buffer + BufferLen / 2; \
   \
   char const sign = '\0';

//
// FormatIntPrec
//
#define FormatIntPrec() \
   /* Pad to precision. */ \
   if(format.prec == -1) \
   { \
      if(!ret.len) \
         *--ret.begin = '0', ret.len = 1; \
      \
      /* When padding to width with 0, use width as precision. */ \
      if(format.flags & __GDCC__FormatFlag_Pad0 && \
         !(format.flags & __GDCC__FormatFlag_Left)) \
      { \
         if(sign && format.width) --format.width; \
         \
         while(ret.len < format.width) \
            *--ret.begin = '0', ++ret.len; \
      } \
   } \
   else \
   { \
      while(ret.len < format.prec) \
         *--ret.begin = '0', ++ret.len; \
   }

//
// FormatIntSign
//
#define FormatIntSign() \
   if(sign) *--ret.begin = sign, ++ret.len;


//----------------------------------------------------------------------------|
// Types                                                                      |
//

typedef char __sta BufferChar;
typedef BufferChar *BufferPtr;


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

[[no_init]]
static BufferChar Buffer[BufferLen];

static char Buffer_INFINITY[9] = " INFINITY";
static char Buffer_NAN[4]      = " NAN";
static char Buffer_infinity[9] = " infinity";
static char Buffer_nan[4]      = " nan";

static char const HexTableL[16] = "0123456789abcdef";
static char const HexTableU[16] = "0123456789ABCDEF";


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// __GDCC__FormatF_X
//
__GDCC__FormatDecl(F, X, float)
{
   FormatFltPreS(6);
   FormatFltCoreX(f, HexTableU, 'P', Buffer_INFINITY, Buffer_NAN);
   FormatFixWidth();
   FormatFltPrefixX('X');
   FormatIntSign();

   return ret;
}

//
// __GDCC__FormatF_Xl
//
__GDCC__FormatDecl(F, Xl, double)
{
   FormatFltPreS(13);
   FormatFltCoreX(fl, HexTableU, 'P', Buffer_INFINITY, Buffer_NAN);
   FormatFixWidth();
   FormatFltPrefixX('X');
   FormatIntSign();

   return ret;
}

//
// __GDCC__FormatF_Xll
//
__GDCC__FormatDecl(F, Xll, long double)
{
   FormatFltPreS(13);
   FormatFltCoreX(fl, HexTableU, 'P', Buffer_INFINITY, Buffer_NAN);
   FormatFixWidth();
   FormatFltPrefixX('X');
   FormatIntSign();

   return ret;
}

//
// __GDCC__FormatF_x
//
__GDCC__FormatDecl(F, x, float)
{
   FormatFltPreS(6);
   FormatFltCoreX(f, HexTableL, 'p', Buffer_infinity, Buffer_nan);
   FormatFixWidth();
   FormatFltPrefixX('x');
   FormatIntSign();

   return ret;
}

//
// __GDCC__FormatF_xl
//
__GDCC__FormatDecl(F, xl, double)
{
   FormatFltPreS(13);
   FormatFltCoreX(fl, HexTableL, 'p', Buffer_infinity, Buffer_nan);
   FormatFixWidth();
   FormatFltPrefixX('x');
   FormatIntSign();

   return ret;
}

//
// __GDCC__FormatF_xll
//
__GDCC__FormatDecl(F, xll, long double)
{
   FormatFltPreS(13);
   FormatFltCoreX(fl, HexTableL, 'p', Buffer_infinity, Buffer_nan);
   FormatFixWidth();
   FormatFltPrefixX('x');
   FormatIntSign();

   return ret;
}

//
// __GDCC__FormatI_d
//
__GDCC__FormatDecl(I, d, int)
{
   FormatIntPreS();
   FormatIntCoreD(signed);
   FormatIntPrec();
   FormatIntSign();

   return ret;
}

//
// __GDCC__FormatI_dl
//
__GDCC__FormatDecl(I, dl, long int)
{
   FormatIntPreS();
   FormatIntCoreD(long signed);
   FormatIntPrec();
   FormatIntSign();

   return ret;
}

//
// __GDCC__FormatI_dll
//
__GDCC__FormatDecl(I, dll, long long int)
{
   FormatIntPreS();
   FormatIntCoreD(long long signed);
   FormatIntPrec();
   FormatIntSign();

   return ret;
}

//
// __GDCC__FormatK_d
//
__GDCC__FormatDecl(K, d, _Accum)
{
   FormatFixPreU();
   FormatFixCoreD(unsigned, k);
   FormatFixWidth();

   return ret;
}

//
// __GDCC__FormatK_dh
//
__GDCC__FormatDecl(K, dh, short _Accum)
{
   FormatFixPreU();
   FormatFixCoreD(unsigned, k);
   FormatFixWidth();

   return ret;
}

//
// __GDCC__FormatK_dl
//
__GDCC__FormatDecl(K, dl, long _Accum)
{
   FormatFixPreU();
   FormatFixCoreD(unsigned, kl);
   FormatFixWidth();

   return ret;
}

//
// __GDCC__FormatU_X
//
__GDCC__FormatDecl(U, X, unsigned)
{
   FormatIntPreU();
   FormatIntCoreX(HexTableU);
   FormatIntPrec();
   FormatIntAltX('X');

   return ret;
}

//
// __GDCC__FormatU_Xl
//
__GDCC__FormatDecl(U, Xl, long unsigned)
{
   FormatIntPreU();
   FormatIntCoreX(HexTableU);
   FormatIntPrec();
   FormatIntAltX('X');

   return ret;
}

//
// __GDCC__FormatU_Xll
//
__GDCC__FormatDecl(U, Xll, long long unsigned)
{
   FormatIntPreU();
   FormatIntCoreX(HexTableU);
   FormatIntPrec();
   FormatIntAltX('X');

   return ret;
}

//
// __GDCC__FormatU_b
//
__GDCC__FormatDecl(U, b, unsigned)
{
   FormatIntPreU();
   FormatIntCoreB();
   FormatIntPrec();
   FormatIntAltB('b');

   return ret;
}

//
// __GDCC__FormatU_bl
//
__GDCC__FormatDecl(U, bl, long unsigned)
{
   FormatIntPreU();
   FormatIntCoreB();
   FormatIntPrec();
   FormatIntAltB('b');

   return ret;
}

//
// __GDCC__FormatU_bll
//
__GDCC__FormatDecl(U, bll, long long unsigned)
{
   FormatIntPreU();
   FormatIntCoreB();
   FormatIntPrec();
   FormatIntAltB('b');

   return ret;
}

//
// __GDCC__FormatU_d
//
__GDCC__FormatDecl(U, d, unsigned)
{
   FormatIntPreU();
   FormatIntCoreD(unsigned);
   FormatIntPrec();

   return ret;
}

//
// __GDCC__FormatU_dl
//
__GDCC__FormatDecl(U, dl, long unsigned)
{
   FormatIntPreU();
   FormatIntCoreD(long unsigned);
   FormatIntPrec();

   return ret;
}

//
// __GDCC__FormatU_dll
//
__GDCC__FormatDecl(U, dll, long long unsigned)
{
   FormatIntPreU();
   FormatIntCoreD(long long unsigned);
   FormatIntPrec();

   return ret;
}

//
// __GDCC__FormatU_o
//
__GDCC__FormatDecl(U, o, unsigned)
{
   FormatIntPreU();
   FormatIntCoreO();
   FormatIntPrec();

   return ret;
}

//
// __GDCC__FormatU_ol
//
__GDCC__FormatDecl(U, ol, long unsigned)
{
   FormatIntPreU();
   FormatIntCoreO();
   FormatIntPrec();

   return ret;
}

//
// __GDCC__FormatU_oll
//
__GDCC__FormatDecl(U, oll, long long unsigned)
{
   FormatIntPreU();
   FormatIntCoreO();
   FormatIntPrec();

   return ret;
}

//
// __GDCC__FormatU_x
//
__GDCC__FormatDecl(U, x, unsigned)
{
   FormatIntPreU();
   FormatIntCoreX(HexTableL);
   FormatIntPrec();
   FormatIntAltX('x');

   return ret;
}

//
// __GDCC__FormatU_xl
//
__GDCC__FormatDecl(U, xl, long unsigned)
{
   FormatIntPreU();
   FormatIntCoreX(HexTableL);
   FormatIntPrec();
   FormatIntAltX('x');

   return ret;
}

//
// __GDCC__FormatU_xll
//
__GDCC__FormatDecl(U, xll, long long unsigned)
{
   FormatIntPreU();
   FormatIntCoreX(HexTableL);
   FormatIntPrec();
   FormatIntAltX('x');

   return ret;
}

//
// __GDCC__FormatX_d
//
__GDCC__FormatDecl(X, d, _Accum)
{
   FormatFixPreS();
   FormatFixCoreD(signed, k);
   FormatFixWidth();
   FormatIntSign();

   return ret;
}

//
// __GDCC__FormatX_dh
//
__GDCC__FormatDecl(X, dh, short _Accum)
{
   FormatFixPreS();
   FormatFixCoreD(signed, k);
   FormatFixWidth();
   FormatIntSign();

   return ret;
}

//
// __GDCC__FormatX_dl
//
__GDCC__FormatDecl(X, dl, long _Accum)
{
   FormatFixPreS();
   FormatFixCoreD(signed, kl);
   FormatFixWidth();
   FormatIntSign();

   return ret;
}

// EOF

