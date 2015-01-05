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
// Fixed-point arithmetic.
//
//-----------------------------------------------------------------------------

#include <stdfix.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// CountlsFXS
//
#define CountlsFXS(w, mod) \
   int res; \
   \
   if(f < 0) \
      __asm("Bclo_W " #w ", LocReg(Lit(:res)), LocReg(Lit(:f))\n"); \
   else \
      __asm("Bclz_W " #w ", LocReg(Lit(:res)), LocReg(Lit(:f))\n"); \
   \
   return res mod;

//
// CountlsFXU
//
#define CountlsFXU(w, mod) \
   int res; \
   __asm("Bclz_W " #w ", LocReg(Lit(:res)), LocReg(Lit(:f))\n"); \
   return res mod;

//
// RoundFX_W1
//
#define RoundFX_W1(type, mask0, max, maxret) \
   union {type f; int n;} u = {f}; \
   \
   int mask = ~((mask0) >> n); \
   \
   /* Round up? */ \
   if(u.n & ((((mask0) >> 1) + 1) >> n)) \
   { \
      /* Check for saturation. */ \
      if((u.n & mask) == (mask & (max))) \
         return (maxret); \
      \
      u.n += (mask0 + 1) >> n; \
   } \
   \
   u.n &= mask; \
   \
   return u.f

//
// RoundFX_W2
//
#define RoundFX_W2(type, max, maxret) \
   union {type f; long int n; struct {int n0, n1;};} u = {f}; \
   \
   /* Optimization for rounding to integer. (And below breaks on n == 1.) */ \
   if(!n) \
   { \
      /* Round up? */ \
      if(u.n0 & 0x80000000) \
      { \
         /* Check for saturation. */ \
         if(u.n1 == max) \
            return maxret; \
         \
         ++u.n1; \
      } \
      \
      u.n0 = 0; \
      \
      return u.f; \
   } \
   \
   int mask = ~(0x7FFFFFFF >> --n); \
   \
   /* Round up? */ \
   if(u.n0 & (0x40000000 >> n)) \
   { \
      /* Check for saturation. */ \
      if(u.n1 == max && (u.n0 & mask) == (mask & 0xFFFFFFFF)) \
         return maxret; \
      \
      u.n += 0x80000000 >> n; \
   } \
   \
   u.n0 &= mask; \
   \
   return u.f


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//=========================================================
// The fixed-point absolute value functions.
//

//
// abshr
//
short fract abshr(short fract f);

//
// absr
//
fract absr(fract f);

//
// abslr
//
long fract abslr(long fract f);

//
// abshk
//
short accum abshk(short accum f)
{
   if(f >= 0) return f;
   return f == SACCUM_MIN ? SACCUM_MAX : -f;
}

//
// absk
//
accum absk(accum f)
{
   if(f >= 0) return f;
   return f == ACCUM_MIN ? ACCUM_MAX : -f;
}

//
// abslk
//
long accum abslk(long accum f)
{
   if(f >= 0) return f;
   return f == LACCUM_MIN ? LACCUM_MAX : -f;
}

//=========================================================
// The fixed-point rounding functions.
//

//
// roundhr
//
short fract roundhr(short fract f, int n)
{
   RoundFX_W1(short fract, 0x7F, 0x7F, SFRACT_MAX);
}

//
// roundr
//
fract roundr(fract f, int n)
{
   RoundFX_W1(fract, 0x7FFF, 0x7FFF, FRACT_MAX);
}

//
// roundlr
//
long fract roundlr(long fract f, int n)
{
   RoundFX_W1(long fract, 0x7FFFFFFF, 0x7FFFFFFF, LFRACT_MAX);
}

//
// roundhk
//
short accum roundhk(short accum f, int n)
{
   RoundFX_W1(short accum, 0xFFFF, 0x7FFFFFFF, SACCUM_MAX);
}

//
// roundk
//
accum roundk(accum f, int n)
{
   RoundFX_W1(accum, 0xFFFF, 0x7FFFFFFF, ACCUM_MAX);
}

//
// roundlk
//
long accum roundlk(long accum f, int n)
{
   RoundFX_W2(long accum, 0x7FFFFFFF, LACCUM_MAX);
}

//
// rounduhr
//
unsigned short fract rounduhr(unsigned short fract f, int n)
{
   RoundFX_W1(unsigned short fract, 0xFF, 0xFF, USFRACT_MAX);
}

//
// roundur
//
unsigned fract roundur(unsigned fract f, int n)
{
   RoundFX_W1(unsigned fract, 0xFFFF, 0xFFFF, UFRACT_MAX);
}

//
// roundulr
//
unsigned long fract roundulr(unsigned long fract f, int n)
{
   RoundFX_W1(unsigned long fract, 0xFFFFFFFF, 0xFFFFFFFF, ULFRACT_MAX);
}

//
// rounduhk
//
unsigned short accum rounduhk(unsigned short accum f, int n)
{
   RoundFX_W1(unsigned short accum, 0xFFFF, 0xFFFFFFFF, USACCUM_MAX);
}

//
// rounduk
//
unsigned accum rounduk(unsigned accum f, int n)
{
   RoundFX_W1(unsigned accum, 0xFFFF, 0xFFFFFFFF, UACCUM_MAX);
}

//
// roundulk
//
unsigned long accum roundulk(unsigned long accum f, int n)
{
   RoundFX_W2(unsigned long accum, 0xFFFFFFFF, ULACCUM_MAX);
}

//=========================================================
// The fixed-point countls functions.
//

//
// countlshr
//
int countlshr(short fract f);

//
// countlsr
//
int countlsr(fract f);

//
// countlslr
//
int countlslr(long fract f);

//
// countlshk
//
int countlshk(short accum f)
{
   CountlsFXS(1, -1);
}

//
// countlsk
//
int countlsk(accum f)
{
   CountlsFXS(1, -1);
}

//
// countlslk
//
int countlslk(long accum f)
{
   CountlsFXS(2, -1);
}

//
// countlsuhr
//
int countlsuhr(unsigned short fract f)
{
   CountlsFXU(1, +24);
}

//
// countlsur
//
int countlsur(unsigned fract f)
{
   CountlsFXU(1, +16);
}

//
// countlsulr
//
int countlsulr(unsigned long fract f)
{
   CountlsFXU(1,);
}

//
// countlsuhk
//
int countlsuhk(unsigned short accum f)
{
   CountlsFXU(1,);
}

//
// countlsuk
//
int countlsuk(unsigned accum f)
{
   CountlsFXU(1,);
}

//
// countlsulk
//
int countlsulk(unsigned long accum f)
{
   CountlsFXU(2,);
}

//=========================================================
// The bitwise fixed-point to integer conversion functions.
//

//
// bitshr
//
int_hr_t bitshr(short fract f)
{
   union {short fract f; int_hr_t n;} u = {f};
   return u.n;
}

//
// bitsr
//
int_r_t bitsr(fract f)
{
   union {fract f; int_r_t n;} u = {f};
   return u.n;
}

//
// bitslr
//
int_lr_t bitslr(long fract f)
{
   union {long fract f; int_lr_t n;} u = {f};
   return u.n;
}

//
// bitshk
//
int_hk_t bitshk(short accum f)
{
   union {short accum f; int_hk_t n;} u = {f};
   return u.n;
}

//
// bitsk
//
int_k_t bitsk(accum f)
{
   union {accum f; int_k_t n;} u = {f};
   return u.n;
}

//
// bitslk
//
int_lk_t bitslk(long accum f)
{
   union {long accum f; int_lk_t n;} u = {f};
   return u.n;
}

//
// bitsuhr
//
uint_uhr_t bitsuhr(unsigned short fract f)
{
   union {unsigned short fract f; uint_uhr_t n;} u = {f};
   return u.n;
}

//
// bitsur
//
uint_ur_t bitsur(unsigned fract f)
{
   union {unsigned fract f; uint_ur_t n;} u = {f};
   return u.n;
}

//
// bitsulr
//
uint_ulr_t bitsulr(unsigned long fract f)
{
   union {unsigned long fract f; uint_ulr_t n;} u = {f};
   return u.n;
}

//
// bitsuhk
//
uint_uhk_t bitsuhk(unsigned short accum f)
{
   union {unsigned short accum f; uint_uhk_t n;} u = {f};
   return u.n;
}

//
// bitsuk
//
uint_uk_t bitsuk(unsigned accum f)
{
   union {unsigned accum f; uint_uk_t n;} u = {f};
   return u.n;
}

//
// bitsulk
//
uint_ulk_t bitsulk(unsigned long accum f)
{
   union {unsigned long accum f; uint_ulk_t n;} u = {f};
   return u.n;
}

//=========================================================
// The bitwise integer to fixed-point conversion functions.
//

//
// hrbits
//
short fract hrbits(int_hr_t n)
{
   union {int_hr_t n; short fract f;} u = {n};
   return u.f;
}

//
// rbits
//
fract rbits(int_r_t n)
{
   union {int_r_t n; fract f;} u = {n};
   return u.f;
}

//
// lrbits
//
long fract lrbits(int_lr_t n)
{
   union {int_lr_t n; long fract f;} u = {n};
   return u.f;
}

//
// hkbits
//
short accum hkbits(int_hk_t n)
{
   union {int_hk_t n; short accum f;} u = {n};
   return u.f;
}

//
// kbits
//
accum kbits(int_k_t n)
{
   union {int_k_t n; accum f;} u = {n};
   return u.f;
}

//
// lkbits
//
long accum lkbits(int_lk_t n)
{
   union {int_lk_t n; long accum f;} u = {n};
   return u.f;
}

//
// uhrbits
//
unsigned short fract uhrbits(uint_uhr_t n)
{
   union {uint_uhr_t n; unsigned short fract f;} u = {n};
   return u.f;
}

//
// urbits
//
unsigned fract urbits(uint_ur_t n)
{
   union {uint_ur_t n; unsigned fract f;} u = {n};
   return u.f;
}

//
// ulrbits
//
unsigned long fract ulrbits(uint_ulr_t n)
{
   union {uint_ulr_t n; unsigned long fract f;} u = {n};
   return u.f;
}

//
// uhkbits
//
unsigned short accum uhkbits(uint_uhk_t n)
{
   union {uint_uhk_t n; unsigned short accum f;} u = {n};
   return u.f;
}

//
// ukbits
//
unsigned accum ukbits(uint_uk_t n)
{
   union {uint_uk_t n; unsigned accum f;} u = {n};
   return u.f;
}

//
// ulkbits
//
unsigned long accum ulkbits(uint_ulk_t n)
{
   union {uint_ulk_t n; unsigned long accum f;} u = {n};
   return u.f;
}

// EOF

