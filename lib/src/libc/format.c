//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Number to string formatting functions.
//
//-----------------------------------------------------------------------------

#include <GDCC_libc.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// FormatFixCoreD
//
#define FormatFixCoreD(T, op) \
   /* Perform core conversion. */ \
   FormatFixCoreDI(T, op); \
   FormatFixCoreDF(T, op); \
   \
   ret._len = ret._end - ret._begin;

//
// FormatFixCoreDF
//
#define FormatFixCoreDF(T, op) \
   /* Fractional conversion. */ \
   if(format._prec) \
   { \
      *ret._end++ = '.'; \
      \
      unsigned precDigs = format._prec; \
      \
      FormatFixCoreDFLoop_##op(T); \
      \
      for(; precDigs; --precDigs) \
         *ret._end++ = '0'; \
   } \
   else if(format._flags & __GDCC__FormatFlag_AltF) \
      *ret._end++ = '.';

//
// FormatFixCoreDFLoop_k
//
#if __GDCC_Family__ZDACS__
#define FormatFixCoreDFLoop_k(T) \
   for(union {T _Accum k; T int i;} u = {data}; u.i && precDigs; --precDigs) \
   { \
      u.i &= 0xFFFF; \
      u.i *= 10; \
      *ret._end++ = (u.i >> 16) + '0'; \
   }
#else
#define FormatFixCoreDFLoop_k(T) \
   for(T _Accum k = data; k && precDigs; --precDigs) \
   { \
      k -= (T FormatTypeInt_k)k; \
      k *= 10; \
      *ret._end++ = (int)k + '0'; \
   }
#endif

//
// FormatFixCoreDFLoop_kh
//
#if __GDCC_Family__ZDACS__
#define FormatFixCoreDFLoop_kh(T) FormatFixCoreDFLoop_k(T)
#else
#define FormatFixCoreDFLoop_kh(T) \
   for(T short _Accum k = data; k && precDigs; --precDigs) \
   { \
      k -= (T FormatTypeInt_kh)k; \
      k *= 10; \
      *ret._end++ = (int)k + '0'; \
   }
#endif

//
// FormatFixCoreDFLoop_kl
//
#if __GDCC_Family__ZDACS__
#define FormatFixCoreDFLoop_kl(T) \
   for(union {T long _Accum k; T int i;} u = {data}; u.i && precDigs; --precDigs) \
   { \
      *ret._end++ = ((int)(((unsigned)u.i >> 24) * 10) >> 8) + '0'; \
      u.i *= 10; \
   }
#else
#define FormatFixCoreDFLoop_kl(T) \
   for(T long _Accum k = data; k && precDigs; --precDigs) \
   { \
      k -= (T FormatTypeInt_kl)k; \
      k *= 10; \
      *ret._end++ = (int)k + '0'; \
   }
#endif

//
// FormatFixCoreDI
//
#define FormatFixCoreDI(T, op) \
   /* Integral conversion. */ \
   for(T FormatTypeInt_##op int i = data; i;) \
   { \
      T FormatTypeInt_##op __div_t d = __div(i, 10); \
      *--ret._begin = d.rem + '0'; \
      i = d.quot; \
   } \
   \
   if(ret._begin == ret._end && \
      (format._prec || !(format._flags & __GDCC__FormatFlag_AltF))) \
      *--ret._begin = '0';

//
// FormatFixPreS
//
#define FormatFixPreS() \
   __GDCC__FormatRet ret; \
   ret._begin = ret._end = __GDCC__FormatBufMid; \
   \
   if(format._prec == -1) format._prec = 6; \
   \
   char sign; \
   if(data < 0) \
   { \
      sign = '-'; \
      data = -data; \
   } \
   else if(format._flags & __GDCC__FormatFlag_Sign) \
      sign = '+'; \
   else if(format._flags & __GDCC__FormatFlag_PadS) \
      sign = ' '; \
   else \
      sign = '\0';

//
// FormatFixPreU
//
#define FormatFixPreU() \
   __GDCC__FormatRet ret; \
   ret._begin = ret._end = __GDCC__FormatBufMid; \
   \
   if(format._prec == -1) format._prec = 6; \
   \
   char const sign = '\0';

//
// FormatFixWidth
//
#define FormatFixWidth() \
   /* Pad to width. */ \
   if(format._flags & __GDCC__FormatFlag_Pad0 && \
      !(format._flags & __GDCC__FormatFlag_Left)) \
   { \
      if(sign && format._width) --format._width; \
      \
      while(ret._len < format._width) \
         *--ret._begin = '0', ++ret._len; \
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
   else if(format._flags & __GDCC__FormatFlag_Sign) \
      sign = '+'; \
   else if(format._flags & __GDCC__FormatFlag_PadS) \
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
         ret._begin = bufNaN + 1, ret._end = bufNaN + 4, ret._len = 3; \
      \
      /* infinity */ \
      else \
         ret._begin = bufINF + 1, ret._end = bufINF + 9, ret._len = 8; \
      \
      if(sign) *--ret._begin = sign, ++ret._len; \
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
   ret._len = ret._end - ret._begin; \
   \
   if(format._flags & __GDCC__FormatFlag_Pad0 && \
      !(format._flags & __GDCC__FormatFlag_Left)) \
   { \
      ret._len += 2; \
   }

//
// FormatFltCoreXE
//
#define FormatFltCoreXE(c) \
   *ret._end++ = (c); \
   if(exp < 0) \
   { \
      exp = -exp; \
      *ret._end++ = '-'; \
   } \
   else \
      *ret._end++ = '+'; \

//
// FormatFltCoreXE_f
//
#define FormatFltCoreXE_f(c) \
   /* Perform core exponent conversion. */ \
   if(exp == -127) \
      *ret._end++ = (c), *ret._end++ = '+', *ret._end++ = '0'; \
   else \
   { \
      FormatFltCoreXE(c); \
      if(exp >= 100) {*ret._end++ = '1';} \
      if(exp >=  10) {*ret._end++ = exp / 10 % 10 + '0';} \
                     {*ret._end++ = exp      % 10 + '0';} \
   }

//
// FormatFltCoreXE_fl
//
#define FormatFltCoreXE_fl(c) \
   /* Perform core exponent conversion. */ \
   if(exp == -1023) \
      *ret._end++ = (c), *ret._end++ = '+', *ret._end++ = '0'; \
   else \
   { \
      FormatFltCoreXE(c); \
      if(exp >= 1000) {*ret._end++ = '1';} \
      if(exp >=  100) {*ret._end++ = exp / 100 % 10 + '0';} \
      if(exp >=   10) {*ret._end++ = exp /  10 % 10 + '0';} \
                      {*ret._end++ = exp       % 10 + '0';} \
   }

//
// FormatFltCoreXF_f
//
#define FormatFltCoreXF_f(table) \
   /* Perform core fractional conversion. */ \
   if(format._prec) \
   { \
      *ret._end++ = '.'; \
      for(; format._prec; --format._prec) \
      { \
         *ret._end++ = table[(u.i0 >> 19) & 0xF]; \
         u.i0 <<= 4; \
      } \
   } \
   else if(format._flags & __GDCC__FormatFlag_AltF) \
      *ret._end++ = '.';

//
// FormatFltCoreXF_fl
//
#define FormatFltCoreXF_fl(table) \
   /* Perform core fractional conversion. */ \
   if(format._prec) \
   { \
      *ret._end++ = '.'; \
      \
      for(int i = 5; i && format._prec; --i, --format._prec) \
      { \
         *ret._end++ = table[(u.i1 >> 16) & 0xF]; \
         u.i1 <<= 4; \
      } \
      \
      for(; format._prec; --format._prec) \
      { \
         *ret._end++ = table[(u.i0 >> 28) & 0xF]; \
         u.i0 <<= 4; \
      } \
   } \
   else if(format._flags & __GDCC__FormatFlag_AltF) \
      *ret._end++ = '.';

//
// FormatFltCoreXI_f
//
#define FormatFltCoreXI_f() \
   /* Perform core integral conversion. */ \
   *--ret._begin = '0' + (exp != -127);

//
// FormatFltCoreXI_fl
//
#define FormatFltCoreXI_fl() \
   /* Perform core integral conversion. */ \
   *--ret._begin = '0' + (exp != -1023);

//
// FormatFltPreS
//
#define FormatFltPreS(defPrec) \
   __GDCC__FormatRet ret; \
   ret._begin = ret._end = __GDCC__FormatBufMid; \
   \
   if(format._prec == -1) format._prec = defPrec;

//
// FormatFltPrefixX
//
#define FormatFltPrefixX(c) \
   *--ret._begin = (c); \
   *--ret._begin = '0'; \
   \
   if(!(format._flags & __GDCC__FormatFlag_Pad0) || \
      format._flags & __GDCC__FormatFlag_Left) \
   { \
      ret._len += 2; \
   }

//
// FormatIntAltB
//
#define FormatIntAltB(c) \
   if(format._flags & __GDCC__FormatFlag_AltF) \
   { \
      *--ret._begin = (c); \
      *--ret._begin = '0'; \
      \
      if(!(format._flags & __GDCC__FormatFlag_Pad0) || \
         format._flags & __GDCC__FormatFlag_Left) \
      { \
         ret._len += 2; \
      } \
   }

//
// FormatIntAltX
//
#define FormatIntAltX(c) \
   if(format._flags & __GDCC__FormatFlag_AltF) \
   { \
      *--ret._begin = (c); \
      *--ret._begin = '0'; \
      \
      if(!(format._flags & __GDCC__FormatFlag_Pad0) || \
         format._flags & __GDCC__FormatFlag_Left) \
      { \
         ret._len += 2; \
      } \
   }

//
// FormatIntCoreB
//
#define FormatIntCoreB() \
   /* Perform core conversion. */ \
   for(; data; data >>= 1) \
      *--ret._begin = ((int)data & 1) + '0'; \
   \
   ret._len = ret._end - ret._begin; \
   \
   if(format._flags & __GDCC__FormatFlag_AltF && \
      format._flags & __GDCC__FormatFlag_Pad0 && \
      !(format._flags & __GDCC__FormatFlag_Left)) \
   { \
      ret._len += 2; \
   }

//
// FormatIntCoreD
//
#define FormatIntCoreD(T) \
   /* Perform core conversion. */ \
   while(data) \
   { \
      T __div_t d = __div(data, 10); \
      *--ret._begin = (int)d.rem + '0'; \
      data = d.quot; \
   } \
   \
   ret._len = ret._end - ret._begin;

//
// FormatIntCoreO
//
#define FormatIntCoreO() \
   /* Perform core conversion. */ \
   for(; data; data >>= 3) \
      *--ret._begin = ((int)data & 7) + '0'; \
   \
   if(format._flags & __GDCC__FormatFlag_AltF) \
   { \
      if(ret._begin == ret._end || *ret._begin != '0') \
         *--ret._begin = '0'; \
   } \
   \
   ret._len = ret._end - ret._begin;

//
// FormatIntCoreX
//
#define FormatIntCoreX(table) \
   /* Perform core conversion. */ \
   for(; data; data >>= 4) \
      *--ret._begin = table[(int)data & 0xF]; \
   \
   ret._len = ret._end - ret._begin; \
   \
   if(format._flags & __GDCC__FormatFlag_AltF && \
      format._flags & __GDCC__FormatFlag_Pad0 && \
      !(format._flags & __GDCC__FormatFlag_Left)) \
   { \
      ret._len += 2; \
   }

//
// FormatIntPreS
//
#define FormatIntPreS() \
   __GDCC__FormatRet ret; \
   ret._begin = ret._end = __GDCC__FormatBufMid; \
   \
   char sign; \
   if(data < 0) \
   { \
      sign = '-'; \
      data = -data; \
   } \
   else if(format._flags & __GDCC__FormatFlag_Sign) \
      sign = '+'; \
   else if(format._flags & __GDCC__FormatFlag_PadS) \
      sign = ' '; \
   else \
      sign = '\0';

//
// FormatIntPreU
//
#define FormatIntPreU() \
   __GDCC__FormatRet ret; \
   ret._begin = ret._end = __GDCC__FormatBufMid; \
   \
   char const sign = '\0';

//
// FormatIntPrec
//
#define FormatIntPrec() \
   /* Pad to precision. */ \
   if(format._prec == -1) \
   { \
      if(!ret._len) \
         *--ret._begin = '0', ret._len = 1; \
      \
      /* When padding to width with 0, use width as precision. */ \
      if(format._flags & __GDCC__FormatFlag_Pad0 && \
         !(format._flags & __GDCC__FormatFlag_Left)) \
      { \
         if(sign && format._width) --format._width; \
         \
         while(ret._len < format._width) \
            *--ret._begin = '0', ++ret._len; \
      } \
   } \
   else \
   { \
      while(ret._len < format._prec) \
         *--ret._begin = '0', ++ret._len; \
   }

//
// FormatIntSign
//
#define FormatIntSign() \
   if(sign) *--ret._begin = sign, ++ret._len;

//
// FormatTypeInt_k
//
#if __GDCC_Target__Doominati__
#define FormatTypeInt_k long
#else
#define FormatTypeInt_k
#endif

//
// FormatTypeInt_kh
//
#define FormatTypeInt_kh

//
// FormatTypeInt_kl
//
#if __GDCC_Target__Doominati__
#define FormatTypeInt_kl long
#else
#define FormatTypeInt_kl long
#endif


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

static char Buffer_INFINITY[9] = " INFINITY";
static char Buffer_NAN[4]      = " NAN";
static char Buffer_infinity[9] = " infinity";
static char Buffer_nan[4]      = " nan";

static char const HexTableL[16] = "0123456789abcdef";
static char const HexTableU[16] = "0123456789ABCDEF";


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

[[no_init]]
char __GDCC__FormatBuf[__GDCC__FormatBufLen];


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

#if !__GDCC__NoFloat
//
// __GDCC__FormatF_X
//
__GDCC__FormatDefn(F, X, float)
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
__GDCC__FormatDefn(F, Xl, double)
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
__GDCC__FormatDefn(F, Xll, long double)
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
__GDCC__FormatDefn(F, x, float)
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
__GDCC__FormatDefn(F, xl, double)
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
__GDCC__FormatDefn(F, xll, long double)
{
   FormatFltPreS(13);
   FormatFltCoreX(fl, HexTableL, 'p', Buffer_infinity, Buffer_nan);
   FormatFixWidth();
   FormatFltPrefixX('x');
   FormatIntSign();

   return ret;
}
#endif // !__GDCC__NoFloat

//
// __GDCC__FormatI_d
//
__GDCC__FormatDefn(I, d, int)
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
__GDCC__FormatDefn(I, dl, long int)
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
__GDCC__FormatDefn(I, dll, long long int)
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
__GDCC__FormatDefn(K, d, _Accum)
{
   FormatFixPreU();
   FormatFixCoreD(unsigned, k);
   FormatFixWidth();

   return ret;
}

//
// __GDCC__FormatK_dh
//
__GDCC__FormatDefn(K, dh, short _Accum)
{
   FormatFixPreU();
   FormatFixCoreD(unsigned, kh);
   FormatFixWidth();

   return ret;
}

//
// __GDCC__FormatK_dl
//
__GDCC__FormatDefn(K, dl, long _Accum)
{
   FormatFixPreU();
   FormatFixCoreD(unsigned, kl);
   FormatFixWidth();

   return ret;
}

//
// __GDCC__FormatU_X
//
__GDCC__FormatDefn(U, X, unsigned)
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
__GDCC__FormatDefn(U, Xl, long unsigned)
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
__GDCC__FormatDefn(U, Xll, long long unsigned)
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
__GDCC__FormatDefn(U, b, unsigned)
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
__GDCC__FormatDefn(U, bl, long unsigned)
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
__GDCC__FormatDefn(U, bll, long long unsigned)
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
__GDCC__FormatDefn(U, d, unsigned)
{
   FormatIntPreU();
   FormatIntCoreD(unsigned);
   FormatIntPrec();

   return ret;
}

//
// __GDCC__FormatU_dl
//
__GDCC__FormatDefn(U, dl, long unsigned)
{
   FormatIntPreU();
   FormatIntCoreD(long unsigned);
   FormatIntPrec();

   return ret;
}

//
// __GDCC__FormatU_dll
//
__GDCC__FormatDefn(U, dll, long long unsigned)
{
   FormatIntPreU();
   FormatIntCoreD(long long unsigned);
   FormatIntPrec();

   return ret;
}

//
// __GDCC__FormatU_o
//
__GDCC__FormatDefn(U, o, unsigned)
{
   FormatIntPreU();
   FormatIntCoreO();
   FormatIntPrec();

   return ret;
}

//
// __GDCC__FormatU_ol
//
__GDCC__FormatDefn(U, ol, long unsigned)
{
   FormatIntPreU();
   FormatIntCoreO();
   FormatIntPrec();

   return ret;
}

//
// __GDCC__FormatU_oll
//
__GDCC__FormatDefn(U, oll, long long unsigned)
{
   FormatIntPreU();
   FormatIntCoreO();
   FormatIntPrec();

   return ret;
}

//
// __GDCC__FormatU_x
//
__GDCC__FormatDefn(U, x, unsigned)
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
__GDCC__FormatDefn(U, xl, long unsigned)
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
__GDCC__FormatDefn(U, xll, long long unsigned)
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
__GDCC__FormatDefn(X, d, _Accum)
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
__GDCC__FormatDefn(X, dh, short _Accum)
{
   FormatFixPreS();
   FormatFixCoreD(signed, kh);
   FormatFixWidth();
   FormatIntSign();

   return ret;
}

//
// __GDCC__FormatX_dl
//
__GDCC__FormatDefn(X, dl, long _Accum)
{
   FormatFixPreS();
   FormatFixCoreD(signed, kl);
   FormatFixWidth();
   FormatIntSign();

   return ret;
}

//
// __GDCC__GetFormatBuf
//
char __sta *__GDCC__GetFormatBuf(void)
{
   return __GDCC__FormatBuf;
}

// EOF

