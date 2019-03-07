//-----------------------------------------------------------------------------
//
// Copyright(C) 2019 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Float to string formatting functions.
//
// Based heavily on this implementation:
//    http://www.ryanjuckett.com/programming/printing-floating-point-numbers/
//
//-----------------------------------------------------------------------------

#define _GNU_SOURCE

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <GDCC_libc.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// FormatFltCore_f
//
#define FormatFltCore_f(bufINF, bufNAN) \
   union {float f; int i; int i0;} u = {data}; \
   \
   FormatFltCoreSign(u.i0); \
   \
   int exp = (u.i0 >> 23) - 127; \
   u.i0 &= 0x007FFFFF; \
   \
   FormatFltCoreSpec(128, bufINF, bufNAN);

//
// FormatFltCore_fl
//
#define FormatFltCore_fl(bufINF, bufNAN) \
   union {double f; long i; struct {int i0, i1;};} u = {data}; \
   \
   FormatFltCoreSign(u.i1); \
   \
   int exp = (u.i1 >> 20) - 1023; \
   u.i1 &= 0x000FFFFF; \
   \
   FormatFltCoreSpec(1024, bufINF, bufNAN);

//
// FormatFltCoreE
//
#define FormatFltCoreE(op, c, bufINF, bufNAN) \
   FormatFltCore_##op(bufINF, bufNAN); \
   FormatFltCoreMan_##op(); \
   \
   /* Perform core conversion. */ \
   ret = FormatFltSci(FormatFltDragon(u.i, exp, manL2, -(int)format._prec + 1), format._prec, c);

//
// FormatFltCoreF
//
#define FormatFltCoreF(op, bufINF, bufNAN) \
   FormatFltCore_##op(bufINF, bufNAN); \
   FormatFltCoreMan_##op(); \
   \
   /* Perform core conversion. */ \
   ret = FormatFltPos(FormatFltDragon(u.i, exp, manL2, -(int)format._prec), format._prec);

//
// FormatFltCoreG
//
#define FormatFltCoreG(op, c, bufINF, bufNAN) \
   FormatFltCore_##op(bufINF, bufNAN); \
   FormatFltCoreMan_##op(); \
   \
   /* Perform core conversion. */ \
   DragonRet dragon = FormatFltDragon(u.i, exp, manL2, -(int)format._prec); \
   \
   /* Strip trailing zeroes. */ \
   if(!(format._flags & __GDCC__FormatFlag_AltF)) \
      while(dragon.len != 1 && dragon.buf[dragon.len - 1] == '0') \
         --dragon.len; \
   \
   if((int)format._prec > dragon.exp && dragon.exp > -4) \
   { \
      /* Reduce precision. */ \
      format._prec -= dragon.exp + 1; \
      int digLenF = (int)dragon.len - (dragon.exp + 1); \
      if(digLenF > (int)format._prec) \
      { \
         /* TODO: Proper rounding. */ \
         dragon.len -= digLenF - format._prec; \
      } \
      \
      ret = FormatFltPos(dragon, format._prec); \
   } \
   else \
      ret = FormatFltSci(dragon, format._prec - 1, c);

//
// FormatFltCoreMan_f
//
#define FormatFltCoreMan_f() \
   unsigned manL2; \
   \
   if(exp != -0x7F) \
   { \
      exp     -= 23; \
      u.i     |= 0x00800000; \
      manL2    = 23; \
   } \
   else \
   { \
      exp      = 1 - 0x7F - 24; \
      manL2    = Log2_32(u.i); \
   }

//
// FormatFltCoreMan_fl
//
#define FormatFltCoreMan_fl() \
   unsigned manL2; \
   bool     marginNE; \
   \
   if(exp != -0x3FF) \
   { \
      marginNE = (exp != 1) && !u.i; \
      exp     -= 52; \
      u.i     |= 0x0010000000000000; \
      manL2    = 52; \
   } \
   else \
   { \
      marginNE = false; \
      exp      = 1 - 0x3FF - 53; \
      manL2    = Log2_64(u.i); \
   }

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
#define FormatFltCoreSpec(expMax, bufINF, bufNAN) \
   /* Check for NaN or infinity. */ \
   if(exp == expMax) \
   { \
      /* NaN */ \
      if(u.i) \
         ret._begin = bufNAN + 1, ret._end = bufNAN + 4, ret._len = 3; \
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
#define FormatFltCoreX(op, table, c, bufINF, bufNAN) \
   FormatFltCore_##op(bufINF, bufNAN); \
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
// FormatFltSign
//
#define FormatFltSign() \
   if(sign) *--ret._begin = sign, ++ret._len;

//
// FormatFltWidth
//
#define FormatFltWidth() \
   /* Pad to width. */ \
   if(format._flags & __GDCC__FormatFlag_Pad0 && \
      !(format._flags & __GDCC__FormatFlag_Left)) \
   { \
      if(sign && format._width) --format._width; \
      \
      while(ret._len < format._width) \
         *--ret._begin = '0', ++ret._len; \
   }

#define VLN_U35_IsZero(n) ((n)->len == 0)

#define VLN_U35_SetZero(n) ((n)->len = 0)


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// DragonRet
//
typedef struct DragonRet
{
   char   *buf;
   size_t  len;
   int32_t exp;
} DragonRet;

//
// VLN_U35
//
typedef struct VLN_U35
{
   uint32_t len;
   uint32_t buf[35];
} VLN_U35;


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

#if !__GDCC__NoFloat
static char Buffer_INFINITY[9] = " INFINITY";
static char Buffer_NAN[4]      = " NAN";
static char Buffer_infinity[9] = " infinity";
static char Buffer_nan[4]      = " nan";

static char const HexTableL[16] = "0123456789abcdef";
static char const HexTableU[16] = "0123456789ABCDEF";

static VLN_U35 VLN_U35_Pow10Tab[] =
{
   // 10 ^ 8
   {1, {0x05F5E100}},
   // 10 ^ 16
   {2, {0x6FC10000, 0x002386F2}},
   // 10 ^ 32
   {4, {0x00000000, 0x85ACEf81, 0x2D6D415B, 0x000004EE}},
   // 10 ^ 64
   {7, {
      0x00000000, 0x00000000, 0xBF6A1F01, 0x6E38ED64, 0xDAA797ED, 0xE93FF9F4,
      0x00184F03}},
   // 10 ^ 128
   {14, {
      0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x2E953E01, 0x03DF9909,
      0x0F1538FD, 0x2374E42F, 0xD3CFF5EC, 0xC404DC08, 0xBCCDB0DA, 0xA6337F19,
      0xE91F2603, 0x0000024E}},
   // 10 ^ 256
   {27, {
      0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
      0x00000000, 0x00000000, 0x982E7C01, 0xBED3875B, 0xD8D99F72, 0x12152F87,
      0x6BDE50C6, 0xCF4A6E70, 0xD595D80F, 0x26B2716E, 0xADC666B0, 0x1D153624,
      0x3C42D35A, 0x63FF540E, 0xCC5573C0, 0x65F9EF17, 0x55BC28F2, 0x80DCC7F7,
      0xF46EEDDC, 0x5FDCEFCE, 0x000553F7}},
};

static uint32_t VLN_U35_Pow10TabU32[8] =
{
   1,
   10,
   100,
   1000,
   10000,
   100000,
   1000000,
   10000000,
};
#endif // !__GDCC__NoFloat


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

#if !__GDCC__NoFloat
static void VLN_U35_MulU32(VLN_U35 *dst, VLN_U35 const *lop, uint32_t rop);
static void VLN_U35_Reserve(VLN_U35 *n, uint32_t len);
static void VLN_U35_Set(VLN_U35 *l, VLN_U35 const *r);
static void VLN_U35_SetU32(VLN_U35 *l, uint32_t r);

//
// Log2_32
//
static uint32_t Log2_32(uint32_t val)
{
   [[return]] __asm
   (
      "Move  W 1(Stk() Lit(31))"
      "Bclz  W 1(Stk() LocReg(Lit(:val)))"
      "Sub:U W 1(Stk() Stk() Stk())"
      "Retn  W 1(Stk())"
   );
}

//
// Log2_64
//
static uint32_t Log2_64(uint64_t val)
{
   [[return]] __asm
   (
      "Move  W 1(Stk() Lit(63))"
      "Bclz  W 2(Stk W 1() LocReg(Lit(:val)))"
      "Sub:U W 1(Stk() Stk() Stk())"
      "Retn  W 1(Stk())"
   );
}

//
// VLN_U35_Add
//
// Adds lop with rop and stores the result in dst. Aliasing is permitted.
//
static void VLN_U35_Add(VLN_U35 *dst, VLN_U35 const *lop, VLN_U35 const *rop)
{
   // Move the shorter operand to the right.
   if(lop->len < rop->len)
   {
      VLN_U35 const *
      tmp = lop;
      lop = rop;
      rop = tmp;
   }

   uint32_t lopLen = lop->len;
   uint32_t ropLen = rop->len;

   // Result is at least as wide as the larger input.
   dst->len = lopLen;

   // Addition loop.
   struct {uint32_t sum, carry, tmp;} sum = {0};
   uint32_t const *lopItr = lop->buf;
   uint32_t const *lopEnd = lopItr + lopLen;
   uint32_t       *dstItr = dst->buf;

   for(uint32_t const *ropItr = rop->buf, *ropEnd = ropItr + rop->len; ropItr != ropEnd;)
   {
      sum.sum = *lopItr++;
      sum.tmp = *ropItr++;
      __asm
      (
         "AddX:U W 1("
            "LocReg W 2(Lit(:sum))"
            "LocReg(Lit(AddPtrRaw(:sum Mul(W 1))))"
            "LocReg(Lit(AddPtrRaw(:sum Mul(W 0))))"
            "LocReg(Lit(AddPtrRaw(:sum Mul(W 2)))))"
      );
      *dstItr++ = sum.sum;
   }

   while(lopItr != lopEnd)
   {
      sum.sum = *lopItr++;
      __asm
      (
         "AddX:U W 1("
            "LocReg W 2(Lit(:sum))"
            "LocReg(Lit(AddPtrRaw(:sum Mul(W 1))))"
            "LocReg(Lit(AddPtrRaw(:sum Mul(W 0)))))"
      );
      *dstItr++ = sum.sum;
   }

   if(sum.carry && dst->len != 35)
   {
      *dstItr = sum.carry;
      ++dst->len;
   }
}

//
// VLN_U35_Cmp
//
static int32_t VLN_U35_Cmp(VLN_U35 const *l, VLN_U35 const *r)
{
   // Leading zeroes are trimmed, so a longer number is larger.
   if(l->len != r->len)
      return l->len - r->len;

   for(uint32_t i = l->len; i--;)
   {
      if(l->buf[i] != r->buf[i])
         return l->buf[i] > r->buf[i] ? 1 : -1;
   }

   return 0;
}

//
// VLN_U35_DivFastQ10
//
// Performs a division where the result is in the range [0,10) and the inputs
// satisfy all of the following:
//    The highest block of the divisor is greater than or equal to 8 so that
//    there is enough precision to make an accurate first guess.
//
//    The highest block of the divisor is less than the maximum value on an
//    unsigned 32-bit integer so that we can increment without overflow.
//
//    The dividend does not contain more blocks than the divisor so that we
//    can estimate the quotient by dividing the high blocks.
//
// Stores the remainder in dst and returns the quotient. lop may alias dst.
//
static uint32_t VLN_U35_DivFastQ10(VLN_U35 *dst, VLN_U35 const *lop, VLN_U35 const *rop)
{
   // If the dividend is less than the divisor, the result is 0 and the
   // dividend is already the remainder.
   uint32_t len = rop->len;
   if(lop->len < len)
   {
      if(dst != lop)
         VLN_U35_Set(dst, lop);
      return 0;
   }

   uint32_t const *ropEnd = rop->buf + len;

   // Compute an estimated quotient based on the high blocks. This will either
   // be correct or under by one.
   uint32_t quot = lop->buf[len - 1] / (rop->buf[len - 1] + 1);

   // Divide out the estimated quotient.
   if(quot)
   {
      // rem = lop - rop * quot

      uint32_t       *dstItr = dst->buf;
      uint32_t const *lopItr = lop->buf;
      uint32_t const *ropItr = rop->buf;

      union {uint64_t diff; struct {uint32_t res, carry;};} diff = {0};
      union {uint64_t prod; struct {uint32_t lo, hi;};} prod = {0};

      while(ropItr != ropEnd)
      {
         prod.lo  = *ropItr++;
         diff.res = *lopItr++;

         __asm
         (
            // prod = *ropItr * quot + prod.hi
            "MulX:U W 1(Stk W 2() LocReg(Lit(:prod)) LocReg(Lit(:quot)))"
            "Move   W 1(Stk() LocReg(Lit(AddPtrRaw(:prod W))))"
            "Move   W 1(Stk() Lit(0))"
            "Add:U  W 2(LocReg(Lit(:prod)) Stk() Stk())"

            // diff = *lopItr - prod.lo - diff.carry
            "SubX:U W 1("
               "LocReg W 2(Lit(:diff))"
               "LocReg(Lit(AddPtrRaw(:diff W)))"
               "LocReg(Lit(AddPtrRaw(:diff 0)))"
               "LocReg(Lit(AddPtrRaw(:prod 0))))"
         );

         *dstItr++ = diff.res;
      }

      // Trim zero blocks.
      while(len && !*--dstItr) --len;
      dst->len = len;
   }
   else if(dst != lop)
      VLN_U35_Set(dst, lop);

   // If the remainder is larger than or equal to the divisor, the quotient
   // estimate was under by one. To correct, increment the quotient and
   // subtract another divisor from the remainder.
   if(VLN_U35_Cmp(dst, rop) >= 0)
   {
      ++quot;

      uint32_t       *dstItr = dst->buf;
      uint32_t const *lopItr = dst->buf;
      uint32_t const *ropItr = rop->buf;

      union {uint64_t diff; struct {uint32_t res, carry, tmp;};} diff = {0};

      while(ropItr != ropEnd)
      {
         diff.res = *lopItr++;
         diff.tmp = *ropItr++;

         __asm
         (
            "SubX:U W 1("
               "LocReg W 2(Lit(:diff))"
               "LocReg(Lit(AddPtrRaw(:diff Mul(W 1))))"
               "LocReg(Lit(AddPtrRaw(:diff Mul(W 0))))"
               "LocReg(Lit(AddPtrRaw(:diff Mul(W 2)))))"
         );

         *dstItr++ = diff.res;
      }

      // Trim zero blocks.
      while(len && !*--dstItr) --len;
      dst->len = len;
   }

   return quot;
}

//
// VLN_U35_GetU32
//
static uint32_t VLN_U35_GetU32(VLN_U35 *n)
{
   return n->len ? n->buf[0] : 0;
}

//
// VLN_U35_Mul
//
static void VLN_U35_Mul(VLN_U35 *dst, VLN_U35 const *lop, VLN_U35 const *rop)
{
   // Move the shorter operand to the right.
   if(lop->len < rop->len)
   {
      VLN_U35 const *
      tmp = lop;
      lop = rop;
      rop = tmp;
   }

   // Result is at most as wide as the two input widths combined.
   VLN_U35_Reserve(dst, lop->len + rop->len);

   if(!dst->len) return;

   // Multiplication loop.

   uint32_t const *lopEnd = lop->buf + lop->len;
   uint32_t       *dstPtr = dst->buf;

   for(uint32_t const *ropItr = rop->buf, *ropEnd = ropItr + rop->len;
      ropItr != ropEnd; ++ropItr, ++dstPtr)
   {
      if(!*ropItr) continue;

      uint32_t const *lopItr = lop->buf;
      uint32_t       *dstItr = dstPtr;

      union {uint64_t sum; struct {uint32_t res, carry;};} sum = {0};
      union {struct {uint32_t l, r;}; uint64_t prod;} prod;

      do
      {
         prod.l  = *lopItr++;
         prod.r  = *ropItr++;
         sum.res = *dstItr;

         __asm
         (
            "MulX:U W 1("
               "LocReg W 2(Lit(:prod))"
               "LocReg(Lit(AddPtrRaw(:prod 0)))"
               "LocReg(Lit(AddPtrRaw(:prod W))))"

            "AddX:U W 1("
               "LocReg W 2(Lit(:sum))"
               "LocReg(Lit(AddPtrRaw(:sum 0)))"
               "LocReg(Lit(AddPtrRaw(:sum W))))"
         );

         sum.sum = sum.sum + prod.prod;
         *dstItr++ = sum.res;
      }
      while(lopItr != lopEnd);

      *dstItr = sum.carry;
   }

   // Truncate possible leading zero.
   if(!dst->buf[dst->len - 1])
      --dst->len;
}

//
// VLN_U35_MulPow10
//
// Multiplies src by 10^exp and stores the result in dst.
//
static void VLN_U35_MulPow10(VLN_U35 *dst, VLN_U35 const *src, uint32_t exp)
{
   // Because multiply can't alias the out and in, use a rotating buffer.
   VLN_U35  tmpBuf[2];
   unsigned tmpIdx = 0;

   // Start with a single word lookup on the low bits.
   if(exp & 7)
      VLN_U35_MulU32(tmpBuf, src, VLN_U35_Pow10TabU32[exp & 7]);
   else
      VLN_U35_Set(tmpBuf, src);

   exp >>= 3;
   for(VLN_U35 const *table = VLN_U35_Pow10Tab; exp; ++table, exp >>= 1)
   {
      if(exp & 1)
      {
         VLN_U35_Mul(&tmpBuf[!tmpIdx], &tmpBuf[tmpIdx], table);
         tmpIdx = !tmpIdx;
      }
   }

   VLN_U35_Set(dst, &tmpBuf[tmpIdx]);
}

//
// VLN_U35_MulU32
//
// As VLN_U35_Mul, but rop is a single word. Aliasing between dst and lop is
// permitted.
//
static void VLN_U35_MulU32(VLN_U35 *dst, VLN_U35 const *lop, uint32_t rop)
{
   // Pull iterators before modifying dst. This allows dst to alias lop.
   uint32_t const *lopItr = lop->buf;
   uint32_t const *lopEnd = lopItr + lop->len;
   uint32_t       *dstItr = dst->buf;

   // Result is at most as wide as the two input widths combined.
   dst->len = lop->len + 1;

   // Multiplication loop.

   union {uint64_t sum; struct {uint32_t res, carry;};} sum = {0};
   union {struct {uint32_t l, r;}; uint64_t product;} prod;

   do
   {
      prod.l = *lopItr++;

      __asm
      (
         "MulX:U("
            "LocReg W 2(Lit(:prod))"
            "LocReg W 1(Lit(:prod))"
            "LocReg W 1(Lit(:rop)))"
      );

      sum.sum = sum.carry + prod.product;
      *dstItr++ = sum.res;
   }
   while(lopItr != lopEnd);

   *dstItr = sum.carry;

   // Truncate possible leading zero.
   if(!dst->buf[dst->len - 1])
      --dst->len;
}

//
// VLN_U35_Pow2
//
static void VLN_U35_Pow2(VLN_U35 *dst, uint32_t exp)
{
   uint32_t idx = exp >> 5, *itr = dst->buf, *end = itr + idx;

   while(itr != end)
      *itr++ = 0;
   *itr = 1 << (exp & 31);

   dst->len = idx + 1;
}

//
// VLN_U35_Reserve
//
static void VLN_U35_Reserve(VLN_U35 *n, uint32_t len)
{
   n->len = len;

   for(uint32_t *itr = n->buf, *end = itr + n->len; itr != end;)
      *itr++ = 0;
}

//
// VLN_U35_Set
//
static void VLN_U35_Set(VLN_U35 *l, VLN_U35 const *r)
{
   uint32_t len = l->len = r->len;

   uint32_t const *in = r->buf;
   for(uint32_t *out = l->buf, *end = out + len; out != end;)
      *out++ = *in++;
}

//
// VLN_U35_SetU32
//
static void VLN_U35_SetU32(VLN_U35 *l, uint32_t r)
{
   l->buf[0] = r;
   l->len    = !!r;
}

//
// VLN_U35_SetU64
//
static void VLN_U35_SetU64(VLN_U35 *l, uint64_t r)
{
   l->buf[0] = r;
   l->buf[1] = r >> 32;
   l->len = l->buf[1] ? 2 : l->buf[0] ? 1 : 0;
}

//
// VLN_U35_ShLU32
//
// lop may alias dst.
//
static void VLN_U35_ShLU32(VLN_U35 *dst, VLN_U35 const *lop, uint32_t rop)
{
   uint32_t len = lop->len;

   if(!len)
   {
      dst->len = 0;
      return;
   }

   uint32_t shiftBlk = rop >> 5;
   uint32_t shiftBit = rop & 31;

   uint32_t       *dstPtr = dst->buf;
   uint32_t const *lopPtr = lop->buf;

   if(!shiftBit)
   {
      uint32_t       *dstItr = dstPtr + len + shiftBlk;
      uint32_t const *lopItr = lopPtr + len;

      while(lopItr != lopPtr)
         *--dstItr = *--lopItr;

      while(dstItr != dstPtr)
         *--dstItr = 0;

      dst->len = len + shiftBlk;
   }
   else
   {
      uint32_t       *dstItr = dstPtr + len + shiftBlk + 1;
      uint32_t const *lopItr = lopPtr + len;

      uint32_t shiftInv = 32 - shiftBit;

      uint32_t bits = 0;

      // Write high words.
      while(lopItr != lopPtr)
      {
         uint32_t word = *--lopItr;
         *--dstItr     = bits | (word >> shiftInv);
         bits          = word << shiftBit;
      }

      // Write mid word.
      *--dstItr = bits;

      // Clear low words.
      while(dstItr != dstPtr)
         *--dstItr = 0;

      // Trim output.
      dst->len = len + shiftBlk + 1;
      if(!dst->buf[len + shiftBlk])
         --dst->len;
   }
}

//
// FormatFltDragonInit
//
static void FormatFltDragonInit(VLN_U35 *value, VLN_U35 *scale, uint64_t man, int exp)
{
   // Calculate the initial state in rational form.
   if(exp > 0)
   {
      VLN_U35_SetU32(scale, 2);

      VLN_U35_SetU64(value, man * 2);
      VLN_U35_ShLU32(value, value, exp);
   }
   else
   {
      VLN_U35_Pow2(scale, -exp + 1);

      VLN_U35_SetU64(value, man * 2);
   }
}

//
// FormatFltDragonExp
//
static int FormatFltDragonExp(VLN_U35 *value, VLN_U35 *scale, int manLog2,
   int exp, int truncExp)
{
   // Compute an estimate for digitExp that will be correct or undershoot by
   // one. This optimization is based on the paper "Printing Floating-Point
   // Numbers Quickly and Accurately" by Burger and Dybvig
   // http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.72.4656&rep=rep1&type=pdf
   // We perform an additional subtraction of 0.69 to increase the frequency of
   // a failed estimate because that lets us take a faster branch in the code.
   // 0.69 is chosen because 0.69 + log10(2) is less than one by a reasonable
   // epsilon that will account for any floating point error.
   //
   // We want to set digitExp to floor(log10(v)) + 1
   //  v = man*2^exp
   //  log2(v) = log2(man) + exp;
   //  log10(v) = log2(v) * log10(2)
   //  floor(log2(v)) = log2(man) + exp;
   //  log10(v) - log10(2) < (log2(man) + exp) * log10(2) <= log10(v)
   //  log10(v) < (log2(man) + exp) * log10(2) + log10(2) <= log10(v) + log10(2)
   //  floor(log10(v)) < ceil((log2(man) + exp) * log10(2)) <= floor(log10(v)) + 1
   double const log10_2 = 0.30102999566398119521373889472449;
   int digitExp = ceil((manLog2 + exp) * log10_2 - 0.69);

   // If the digit exponent is smaller than the smallest desired digit for
   // fractional cutoff, pull the digit back into legal range at which point we
   // will round to the appropriate value.
   // Note that while our value for digitExp is still an estimate, this is safe
   // because it only increases the number. This will either correct digitExp
   // to an accurate value or it will clamp it above the accurate value.
   if(digitExp <= truncExp)
      digitExp = truncExp + 1;

   // Divide value by 10^digitExp.
   if(digitExp > 0)
      VLN_U35_MulPow10(scale, scale, digitExp);

   else if(digitExp < 0)
      VLN_U35_MulPow10(value, value, -digitExp);

   // If value >= 1, our estimate for digitExp was too low.
   if(VLN_U35_Cmp(value, scale) >= 0)
   {
      // Increment exponent and skip pre-multiply.
      ++digitExp;
   }
   else
   {
      // Multiply value to prepare for first loop iteration.
      VLN_U35_MulU32(value, value, 10);
   }

   return digitExp;
}

//
// FormatFltDragonScale
//
static void FormatFltDragonScale(VLN_U35 *value, VLN_U35 *scale)
{
   // In preparation for calling DivFastQ10, we need to scale up our values
   // such that the highest block of the denominator is greater than or equal
   // to 8. We also need to guarantee that the numerator can never have a
   // length greater than the denominator after each loop iteration. This
   // requires the highest block of the denominator to be less than or equal to
   // 429496729 which is the highest number that can be multiplied by 10
   // without overflowing to a new block.
   uint32_t scaleHiLog2 = Log2_32(scale->buf[scale->len - 1]);
   if(scaleHiLog2 < 3 || scaleHiLog2 > 27)
   {
      // Perform a bit shift on all values to get the highest block of the
      // denominator into the range [8,429496729]. We are more likely to make
      // accurate quotient estimations in DivFastQ10 with higher denominator
      // values so we shift the denominator to place the highest bit at index
      // 27 of the highest block. This is safe because (2^28 - 1) = 268435455
      // which is less than 429496729. This means that all values with a
      // highest bit at index 27 are within range.
      uint32_t shift = (32 + 27 - scaleHiLog2) & 31;

      VLN_U35_ShLU32(scale, scale, shift);
      VLN_U35_ShLU32(value, value, shift);
   }
}

//
// FormatFltDragonHeart
//
static DragonRet FormatFltDragonHeart(VLN_U35 *value, VLN_U35 *scale,
   int digitExp, int truncExp)
{
   DragonRet ret;
   ret.buf = __GDCC__FormatBufMid;

   // Set the exponent result.
   ret.exp = digitExp - 1;

   // Core format loop.
   char *bufItr = ret.buf;
   int digit;
   for(;;)
   {
      --digitExp;

      digit = VLN_U35_DivFastQ10(value, value, scale);

      // Check if we have generated enough digits. The above divide must be
      // done regardless, however.
      if(digitExp == truncExp || VLN_U35_IsZero(value)) break;

      *bufItr++ = '0' + digit;

      VLN_U35_MulU32(value, value, 10);
   }

   // Round off final digit.
   bool roundDown;
   {
      VLN_U35_ShLU32(value, value, 1);
      int cmp = VLN_U35_Cmp(value, scale);
      if(cmp == 0)
         roundDown = !(digit & 1);
      else
         roundDown = cmp < 0;
   }

   if(roundDown)
   {
      *bufItr++ = '0' + digit;
   }
   else
   {
      // Find the first non-nine digit.
      if(digit == 9) for(;;)
      {
         // Ran out of digits. Generate "1" and increment exponent.
         if(bufItr == ret.buf) {*bufItr++ = '1'; ++ret.exp; break;}

         if(*--bufItr != '9') {++*bufItr++; break;}
      }
      else
         *bufItr++ = '0' + digit + 1;
   }

   ret.len = bufItr - ret.buf;
   return ret;
}

//
// FormatFltDragon
//
static DragonRet FormatFltDragon(uint64_t man, int exp, int manLog2, int truncExp)
{
   if(!man)
   {
      DragonRet ret;
      ret.buf = __GDCC__FormatBufMid;

      *ret.buf = '0';
      ret.len = 1;
      ret.exp = 0;
      return ret;
   }

   // Calculate the initial state in rational form.
   VLN_U35 value;
   VLN_U35 scale;
   FormatFltDragonInit(&value, &scale, man, exp);

   // Calculate first digit exponent.
   int digitExp = FormatFltDragonExp(&value, &scale, manLog2, exp, truncExp);

   // Adjust value for use with DivFastQ10.
   FormatFltDragonScale(&value, &scale);

   // Core formatter.
   return FormatFltDragonHeart(&value, &scale, digitExp, truncExp);
}

//
// FormatFltPos
//
static __GDCC__FormatRet FormatFltPos(DragonRet dragon, uint32_t prec)
{
   __GDCC__FormatRet ret;
   ret._begin = dragon.buf;
   ret._end = dragon.buf + dragon.len;

   uint32_t digLenF = 0;
   if(dragon.exp >= 0)
   {
      uint32_t digLenI = dragon.exp + 1;
      if(dragon.len < digLenI)
      {
         // Add trailing zeroes to the (currently omitted) decimal point.

         while(dragon.len < digLenI)
            *ret._end++ = '0', ++dragon.len;
      }
      else if(dragon.len > digLenI)
      {
         // Insert decimal point.

         digLenF = dragon.len - digLenI;

         if(digLenF > digLenI)
         {
            memmove(ret._begin + digLenI + 1, ret._begin + digLenI, digLenF);
            ++ret._end;
         }
         else
         {
            --ret._begin;
            memmove(ret._begin, ret._begin + 1, digLenI);
         }

         ret._begin[digLenI] = '.';
      }
   }
   else
   {
      // Add leading zeroes.
      uint32_t digLenF0 = (uint32_t)-dragon.exp - 1;

      for(uint32_t i = digLenF0; i--;)
         *--ret._begin = '0';

      *--ret._begin = '.';
      *--ret._begin = '0';

      digLenF = dragon.len + digLenF0;
   }

   // FIXME: When called for %g, there may be excess fractional digits.

   // Add trailing zeroes to the precision length.
   if(digLenF < prec)
   {
      // Add decimal point if there have been no fractional digits.
      if(!digLenF)
         *ret._end++ = '.';

      while(digLenF != prec)
         *ret._end++ = '0', ++digLenF;
   }

   ret._len = ret._end - ret._begin;
   return ret;
}

//
// FormatFltSci
//
static __GDCC__FormatRet FormatFltSci(DragonRet dragon, uint32_t prec, char expC)
{
   __GDCC__FormatRet ret;
   ret._begin = dragon.buf;
   ret._end = dragon.buf + dragon.len;

   // Insert decimal point.
   size_t digLenF = dragon.len - 1;
   if(digLenF)
   {
      --ret._begin;
      ret._begin[0] = ret._begin[1];
      ret._begin[1] = '.';
   }

   // Add trailing zeroes to precision length.
   if(prec > digLenF)
   {
      if(!digLenF)
         *ret._end++ = '.';

      for(uint32_t zeroes = prec - digLenF; zeroes--;)
         *ret._end++ = '0';
   }

   // Write exponent.
   *ret._end++ = expC;

   if(dragon.exp < 0)
   {
      *ret._end++ = '-';
      dragon.exp = -dragon.exp;
   }
   else
      *ret._end++ = '+';

   if(dragon.exp >= 100)
   {
      div_t expDiv = div(dragon.exp, 10);
      ret._end[2] = '0' + expDiv.rem;
      expDiv = div(expDiv.quot, 10);
      ret._end[1] = '0' + expDiv.rem;
      ret._end[0] = '0' + expDiv.quot;
      ret._end += 3;
   }
   else
   {
      div_t expDiv = div(dragon.exp, 10);
      ret._end[1] = '0' + expDiv.rem;
      ret._end[0] = '0' + expDiv.quot;
      ret._end += 2;
   }

   ret._len = ret._end - ret._begin;
   return ret;
}
#endif // !__GDCC__NoFloat


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

#if !__GDCC__NoFloat
//
// __GDCC__FormatF_E
//
__GDCC__FormatDefn(F, E, float)
{
   FormatFltPreS(6);
   FormatFltCoreE(f, 'E', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_El
//
__GDCC__FormatDefn(F, El, double)
{
   FormatFltPreS(6);
   FormatFltCoreE(fl, 'E', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_Ell
//
__GDCC__FormatDefn(F, Ell, long double)
{
   FormatFltPreS(6);
   FormatFltCoreE(fl, 'E', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_F
//
__GDCC__FormatDefn(F, F, float)
{
   FormatFltPreS(6);
   FormatFltCoreF(f, Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_Fl
//
__GDCC__FormatDefn(F, Fl, double)
{
   FormatFltPreS(6);
   FormatFltCoreF(fl, Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_Fll
//
__GDCC__FormatDefn(F, Fll, long double)
{
   FormatFltPreS(6);
   FormatFltCoreF(fl, Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_G
//
__GDCC__FormatDefn(F, G, float)
{
   FormatFltPreS(6);
   FormatFltCoreG(f, 'E', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_Gl
//
__GDCC__FormatDefn(F, Gl, double)
{
   FormatFltPreS(6);
   FormatFltCoreG(fl, 'E', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_Gll
//
__GDCC__FormatDefn(F, Gll, long double)
{
   FormatFltPreS(6);
   FormatFltCoreG(fl, 'E', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_X
//
__GDCC__FormatDefn(F, X, float)
{
   FormatFltPreS(6);
   FormatFltCoreX(f, HexTableU, 'P', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltPrefixX('X');
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_Xl
//
__GDCC__FormatDefn(F, Xl, double)
{
   FormatFltPreS(13);
   FormatFltCoreX(fl, HexTableU, 'P', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltPrefixX('X');
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_Xll
//
__GDCC__FormatDefn(F, Xll, long double)
{
   FormatFltPreS(13);
   FormatFltCoreX(fl, HexTableU, 'P', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltPrefixX('X');
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_e
//
__GDCC__FormatDefn(F, e, float)
{
   FormatFltPreS(6);
   FormatFltCoreE(f, 'e', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_el
//
__GDCC__FormatDefn(F, el, double)
{
   FormatFltPreS(6);
   FormatFltCoreE(fl, 'e', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_ell
//
__GDCC__FormatDefn(F, ell, long double)
{
   FormatFltPreS(6);
   FormatFltCoreE(fl, 'e', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_f
//
__GDCC__FormatDefn(F, f, float)
{
   FormatFltPreS(6);
   FormatFltCoreF(f, Buffer_infinity, Buffer_nan);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_fl
//
__GDCC__FormatDefn(F, fl, double)
{
   FormatFltPreS(6);
   FormatFltCoreF(fl, Buffer_infinity, Buffer_nan);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_fll
//
__GDCC__FormatDefn(F, fll, long double)
{
   FormatFltPreS(6);
   FormatFltCoreF(fl, Buffer_infinity, Buffer_nan);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_g
//
__GDCC__FormatDefn(F, g, float)
{
   FormatFltPreS(6);
   FormatFltCoreG(f, 'e', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_gl
//
__GDCC__FormatDefn(F, gl, double)
{
   FormatFltPreS(6);
   FormatFltCoreG(fl, 'e', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_gll
//
__GDCC__FormatDefn(F, gll, long double)
{
   FormatFltPreS(6);
   FormatFltCoreG(fl, 'e', Buffer_INFINITY, Buffer_NAN);
   FormatFltWidth();
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_x
//
__GDCC__FormatDefn(F, x, float)
{
   FormatFltPreS(6);
   FormatFltCoreX(f, HexTableL, 'p', Buffer_infinity, Buffer_nan);
   FormatFltWidth();
   FormatFltPrefixX('x');
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_xl
//
__GDCC__FormatDefn(F, xl, double)
{
   FormatFltPreS(13);
   FormatFltCoreX(fl, HexTableL, 'p', Buffer_infinity, Buffer_nan);
   FormatFltWidth();
   FormatFltPrefixX('x');
   FormatFltSign();

   return ret;
}

//
// __GDCC__FormatF_xll
//
__GDCC__FormatDefn(F, xll, long double)
{
   FormatFltPreS(13);
   FormatFltCoreX(fl, HexTableL, 'p', Buffer_infinity, Buffer_nan);
   FormatFltWidth();
   FormatFltPrefixX('x');
   FormatFltSign();

   return ret;
}
#endif // !__GDCC__NoFloat

// EOF

