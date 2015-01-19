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

#ifndef __GDCC_Header__C__stdfix_h__
#define __GDCC_Header__C__stdfix_h__


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// fract
//
#ifndef fract
#define fract _Fract
#endif

//
// accum
//
#ifndef accum
#define accum _Accum
#endif

//
// sat
//
#ifndef sat
#define sat _Sat
#endif

//
// SFRACT_*
//
#define SFRACT_FBIT 7
#define SFRACT_MIN (-0.5HR-0.5HR)
#define SFRACT_MAX 0x.FEp-0HR
#define SFRACT_EPSILON 0x1p-7HR

//
// USFRACT_*
//
#define USFRACT_FBIT 8
#define USFRACT_MAX 0x.FFp-0UHR
#define USFRACT_EPSILON 0x1p-8UHR

//
// FRACT_*
//
#define FRACT_FBIT 15
#define FRACT_MIN (-0.5R-0.5R)
#define FRACT_MAX 0x.FFFEp-0R
#define FRACT_EPSILON 0x1p-15R

//
// UFRACT_*
//
#define UFRACT_FBIT 16
#define UFRACT_MAX 0x.FFFFp-0UR
#define UFRACT_EPSILON 0x1p-16UR

//
// LFRACT_*
//
#define LFRACT_FBIT 31
#define LFRACT_MIN (-0.5LR-0.5LR)
#define LFRACT_MAX 0x.FFFFFFFEp-0LR
#define LFRACT_EPSILON 0x1p-31LR

//
// ULFRACT_*
//
#define ULFRACT_FBIT 32
#define ULFRACT_MAX 0x.FFFFFFFFp-0ULR
#define ULFRACT_EPSILON 0x1p-32ULR

//
// SACCUM_*
//
#define SACCUM_FBIT 16
#define SACCUM_IBIT 15
#define SACCUM_MIN (-0x4000p-0HK-0x4000p-0HK)
#define SACCUM_MAX 0x7FFF.FFFFp-0HK
#define SACCUM_EPSILON 0x1p-16HK

//
// USACCUM_*
//
#define USACCUM_FBIT 16
#define USACCUM_IBIT 16
#define USACCUM_MAX 0xFFFF.FFFFp-0UHK
#define USACCUM_EPSILON 0x1p-16UHK

//
// ACCUM_*
//
#define ACCUM_FBIT 16
#define ACCUM_IBIT 15
#define ACCUM_MIN (-0x4000p-0K-0x4000p-0K)
#define ACCUM_MAX 0x7FFF.FFFFp-0K
#define ACCUM_EPSILON 0x1p-16K

//
// UACCUM_*
//
#define UACCUM_FBIT 16
#define UACCUM_IBIT 16
#define UACCUM_MAX 0xFFFF.FFFFp-0UK
#define UACCUM_EPSILON 0x1p-16UK

//
// LACCUM_*
//
#define LACCUM_FBIT 32
#define LACCUM_IBIT 31
#define LACCUM_MIN (-0x40000000p-0LK-0x40000000p-0LK)
#define LACCUM_MAX 0x7FFFFFFF.FFFFFFFFp-0LK
#define LACCUM_EPSILON 0x1p-32LK

//
// ULACCUM_*
//
#define ULACCUM_FBIT 32
#define ULACCUM_IBIT 32
#define ULACCUM_MAX 0xFFFFFFFF.FFFFFFFFp-0ULK
#define ULACCUM_EPSILON 0x1p-32ULK

//
// absfx
//
#define absfx(f) (__GDCC__TGMathFX((f), abs,)((f)))

//
// roundfx
//
#define roundfx(f, n) (__GDCC__TGMathFX((f), round, u)((f), (n)))

//
// countlsfx
//
#define countlsfx(f) (__GDCC__TGMathFX((f), countls, u)((f)))

//
// fixed
//
// Not standard, but there is sufficient community momentum to justify it.
//
#ifndef fixed
#define fixed __fixed
#endif

//
// __GDCC__TGMathFX
//
#define __GDCC__TGMathFX(f, func, u) \
   (_Generic((f), \
               short fract: func##hr, \
                     fract: func##r, \
               long  fract: func##lr, \
               short accum: func##hk, \
                     accum: func##k, \
               long  accum: func##lk, \
      unsigned short fract: func##u##hr, \
      unsigned       fract: func##u##r, \
      unsigned long  fract: func##u##lr, \
      unsigned short accum: func##u##hk, \
      unsigned       accum: func##u##k, \
      unsigned long  accum: func##u##lk))


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// int_*_t
//
typedef int  int_hr_t;
typedef int  int_r_t;
typedef int  int_lr_t;
typedef int  int_hk_t;
typedef int  int_k_t;
typedef long int_lk_t;

//
// uint_*_t
//
typedef unsigned int  uint_uhr_t;
typedef unsigned int  uint_ur_t;
typedef unsigned int  uint_ulr_t;
typedef unsigned int  uint_uhk_t;
typedef unsigned int  uint_uk_t;
typedef unsigned long uint_ulk_t;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

#ifdef __cplusplus
extern "C" {
#endif

//
// The fixed-point arithmetic operation support functions.
//

int mulir(int, fract);
long int mulilr(long int, long fract);
int mulik(int, accum);
long int mulilk(long int, long accum);

int divir(int, fract);
long int divilr(long int, long fract);
int divik(int, accum);
long int divilk(long int, long accum);

fract rdivi(int, int);
long fract lrdivi(long int, long int);
accum kdivi(int, int);
long accum lkdivi(long int, long int);

int idivr(fract, fract);
long int idivlr(long fract, long fract);
int idivk(accum, accum);
long int idivlk(long accum, long accum);

unsigned int muliur(unsigned int, unsigned fract);
unsigned long int muliulr(unsigned long int, unsigned long fract);
unsigned int muliuk(unsigned int, unsigned accum);
unsigned long int muliulk(unsigned long int, unsigned long accum);

unsigned int diviur(unsigned int, unsigned fract);
unsigned long int diviulr(unsigned long int, unsigned long fract);
unsigned int diviuk(unsigned int, unsigned accum);
unsigned long int diviulk(unsigned long int, unsigned long accum);

unsigned fract urdivi(unsigned int, unsigned int);
unsigned long fract ulrdivi(unsigned long int, unsigned long int);
unsigned accum ukdivi(unsigned int, unsigned int);
unsigned long accum ulkdivi(unsigned long int, unsigned long int);

unsigned int idivur(unsigned fract, unsigned fract);
unsigned long int idivulr(unsigned long fract, unsigned long fract);
unsigned int idivuk(unsigned accum, unsigned accum);
unsigned long int idivulk(unsigned long accum, unsigned long accum);

//
// The fixed-point absolute value functions.
//

short fract abshr(short fract f);
fract absr(fract f);
long fract abslr(long fract f);
short accum abshk(short accum f);
accum absk(accum f);
long accum abslk(long accum f);

//
// The fixed-point rounding functions.
//

short fract roundhr(short fract f, int n);
fract roundr(fract f, int n);
long fract roundlr(long fract f, int n);
short accum roundhk(short accum f, int n);
accum roundk(accum f, int n);
long accum roundlk(long accum f, int n);

unsigned short fract rounduhr(unsigned short fract f, int n);
unsigned fract roundur(unsigned fract f, int n);
unsigned long fract roundulr(unsigned long fract f, int n);
unsigned short accum rounduhk(unsigned short accum f, int n);
unsigned accum rounduk(unsigned accum f, int n);
unsigned long accum roundulk(unsigned long accum f, int n);

//
// The fixed-point countls functions.
//

int countlshr(short fract f);
int countlsr(fract f);
int countlslr(long fract f);
int countlshk(short accum f);
int countlsk(accum f);
int countlslk(long accum f);

int countlsuhr(unsigned short fract f);
int countlsur(unsigned fract f);
int countlsulr(unsigned long fract f);
int countlsuhk(unsigned short accum f);
int countlsuk(unsigned accum f);
int countlsulk(unsigned long accum f);

//
// The bitwise fixed-point to integer conversion functions.
//

int_hr_t bitshr(short fract f);
int_r_t bitsr(fract f);
int_lr_t bitslr(long fract f);
int_hk_t bitshk(short accum f);
int_k_t bitsk(accum f);
int_lk_t bitslk(long accum f);

uint_uhr_t bitsuhr(unsigned short fract f);
uint_ur_t bitsur(unsigned fract f);
uint_ulr_t bitsulr(unsigned long fract f);
uint_uhk_t bitsuhk(unsigned short accum f);
uint_uk_t bitsuk(unsigned accum f);
uint_ulk_t bitsulk(unsigned long accum f);

//
// The bitwise integer to fixed-point conversion functions.
//

short fract hrbits(int_hr_t n);
fract rbits(int_r_t n);
long fract lrbits(int_lr_t n);
short accum hkbits(int_hk_t n);
accum kbits(int_k_t n);
long accum lkbits(int_lk_t n);

unsigned short fract uhrbits(uint_uhr_t n);
unsigned fract urbits(uint_ur_t n);
unsigned long fract ulrbits(uint_ulr_t n);
unsigned short accum uhkbits(uint_uhk_t n);
unsigned accum ukbits(uint_uk_t n);
unsigned long accum ulkbits(uint_ulk_t n);

//
// Numeric conversion functions.
//

short fract strtofxhr(char const *restrict nptr, char **restrict endptr);
fract strtofxr(char const *restrict nptr, char **restrict endptr);
long fract strtofxlr(char const *restrict nptr, char **restrict endptr);
short accum strtofxhk(char const *restrict nptr, char **restrict endptr);
accum strtofxk(char const *restrict nptr, char **restrict endptr);
long accum strtofxlk(char const *restrict nptr, char **restrict endptr);

unsigned short fract strtofxuhr(char const *restrict nptr, char **restrict endptr);
unsigned fract strtofxur(char const *restrict nptr, char **restrict endptr);
unsigned long fract strtofxulr(char const *restrict nptr, char **restrict endptr);
unsigned short accum strtofxuhk(char const *restrict nptr, char **restrict endptr);
unsigned accum strtofxuk(char const *restrict nptr, char **restrict endptr);
unsigned long accum strtofxulk(char const *restrict nptr, char **restrict endptr);

#ifdef __cplusplus
}
#endif


#endif//__GDCC_Header__C__stdfix_h__

