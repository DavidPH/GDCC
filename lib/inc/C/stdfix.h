//-----------------------------------------------------------------------------
//
// Copyright(C) 2015-2017 David Hill
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

#include <bits/features.h>


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
#if __GDCC_Target__Doominati__
# define SACCUM_FBIT 7
# define SACCUM_IBIT 24
# define SACCUM_MIN (-0x800000p-0HK-0x800000p-0HK)
# define SACCUM_MAX 0xFFFFFF.FEp-0HK
# define SACCUM_EPSILON 0x1p-7HK
#else
# define SACCUM_FBIT 16
# define SACCUM_IBIT 15
# define SACCUM_MIN (-0x4000p-0HK-0x4000p-0HK)
# define SACCUM_MAX 0x7FFF.FFFFp-0HK
# define SACCUM_EPSILON 0x1p-16HK
#endif

//
// USACCUM_*
//
#if __GDCC_Target__Doominati__
# define USACCUM_FBIT 7
# define USACCUM_IBIT 25
# define USACCUM_MAX 0x1FFFFFF.FEp-0UHK
# define USACCUM_EPSILON 0x1p-7UHK
#else
# define USACCUM_FBIT 16
# define USACCUM_IBIT 16
# define USACCUM_MAX 0xFFFF.FFFFp-0UHK
# define USACCUM_EPSILON 0x1p-16UHK
#endif

//
// ACCUM_*
//
#if __GDCC_Target__Doominati__
# define ACCUM_FBIT 15
# define ACCUM_IBIT 48
# define ACCUM_MIN (-0x800000000000p-0K-0x800000000000p-0K)
# define ACCUM_MAX 0xFFFFFFFFFFFF.FFFEp-0K
# define ACCUM_EPSILON 0x1p-15K
#else
# define ACCUM_FBIT 16
# define ACCUM_IBIT 15
# define ACCUM_MIN (-0x4000p-0K-0x4000p-0K)
# define ACCUM_MAX 0x7FFF.FFFFp-0K
# define ACCUM_EPSILON 0x1p-16K
#endif

//
// UACCUM_*
//
#if __GDCC_Target__Doominati__
# define UACCUM_FBIT 15
# define UACCUM_IBIT 49
# define UACCUM_MAX 0x1FFFFFFFFFFFF.FFFEp-0K
# define UACCUM_EPSILON 0x1p-15UK
#else
# define UACCUM_FBIT 16
# define UACCUM_IBIT 16
# define UACCUM_MAX 0xFFFF.FFFFp-0UK
# define UACCUM_EPSILON 0x1p-16UK
#endif

//
// LACCUM_*
//
#if __GDCC_Target__Doominati__
# define LACCUM_FBIT 32
# define LACCUM_IBIT 63
# define LACCUM_MIN (-0x4000000000000000p-0LK-0x4000000000000000p-0LK)
# define LACCUM_MAX 0x7FFFFFFFFFFFFFFF.FFFFFFFFp-0LK
# define LACCUM_EPSILON 0x1p-32LK
#else
# define LACCUM_FBIT 32
# define LACCUM_IBIT 31
# define LACCUM_MIN (-0x40000000p-0LK-0x40000000p-0LK)
# define LACCUM_MAX 0x7FFFFFFF.FFFFFFFFp-0LK
# define LACCUM_EPSILON 0x1p-32LK
#endif

//
// ULACCUM_*
//
#if __GDCC_Target__Doominati__
# define ULACCUM_FBIT 32
# define ULACCUM_IBIT 64
# define ULACCUM_MAX 0xFFFFFFFFFFFFFFFF.FFFFFFFFp-0ULK
# define ULACCUM_EPSILON 0x1p-32ULK
#else
# define ULACCUM_FBIT 32
# define ULACCUM_IBIT 32
# define ULACCUM_MAX 0xFFFFFFFF.FFFFFFFFp-0ULK
# define ULACCUM_EPSILON 0x1p-32ULK
#endif

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
#if !defined(fixed) && !defined(__STRICT_ANSI__)
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

short fract abshr(short fract _f);
fract absr(fract _f);
long fract abslr(long fract _f);
short accum abshk(short accum _f);
accum absk(accum _f);
long accum abslk(long accum _f);

//
// The fixed-point rounding functions.
//

short fract roundhr(short fract _f, int _n);
fract roundr(fract _f, int _n);
long fract roundlr(long fract _f, int _n);
short accum roundhk(short accum _f, int _n);
accum roundk(accum _f, int _n);
long accum roundlk(long accum _f, int _n);

unsigned short fract rounduhr(unsigned short fract _f, int _n);
unsigned fract roundur(unsigned fract _f, int _n);
unsigned long fract roundulr(unsigned long fract _f, int _n);
unsigned short accum rounduhk(unsigned short accum _f, int _n);
unsigned accum rounduk(unsigned accum _f, int _n);
unsigned long accum roundulk(unsigned long accum _f, int _n);

//
// The fixed-point countls functions.
//

int countlshr(short fract _f);
int countlsr(fract _f);
int countlslr(long fract _f);
int countlshk(short accum _f);
int countlsk(accum _f);
int countlslk(long accum _f);

int countlsuhr(unsigned short fract _f);
int countlsur(unsigned fract _f);
int countlsulr(unsigned long fract _f);
int countlsuhk(unsigned short accum _f);
int countlsuk(unsigned accum _f);
int countlsulk(unsigned long accum _f);

//
// The bitwise fixed-point to integer conversion functions.
//

int_hr_t bitshr(short fract _f);
int_r_t bitsr(fract _f);
int_lr_t bitslr(long fract _f);
int_hk_t bitshk(short accum _f);
int_k_t bitsk(accum _f);
int_lk_t bitslk(long accum _f);

uint_uhr_t bitsuhr(unsigned short fract _f);
uint_ur_t bitsur(unsigned fract _f);
uint_ulr_t bitsulr(unsigned long fract _f);
uint_uhk_t bitsuhk(unsigned short accum _f);
uint_uk_t bitsuk(unsigned accum _f);
uint_ulk_t bitsulk(unsigned long accum _f);

//
// The bitwise integer to fixed-point conversion functions.
//

short fract hrbits(int_hr_t _n);
fract rbits(int_r_t _n);
long fract lrbits(int_lr_t _n);
short accum hkbits(int_hk_t _n);
accum kbits(int_k_t _n);
long accum lkbits(int_lk_t _n);

unsigned short fract uhrbits(uint_uhr_t _n);
unsigned fract urbits(uint_ur_t _n);
unsigned long fract ulrbits(uint_ulr_t _n);
unsigned short accum uhkbits(uint_uhk_t _n);
unsigned accum ukbits(uint_uk_t _n);
unsigned long accum ulkbits(uint_ulk_t _n);

//
// Numeric conversion functions.
//

short fract strtofxhr(char const *restrict _nptr, char **restrict _endptr);
fract strtofxr(char const *restrict _nptr, char **restrict _endptr);
long fract strtofxlr(char const *restrict _nptr, char **restrict _endptr);
short accum strtofxhk(char const *restrict _nptr, char **restrict _endptr);
accum strtofxk(char const *restrict _nptr, char **restrict _endptr);
long accum strtofxlk(char const *restrict _nptr, char **restrict _endptr);

unsigned short fract strtofxuhr(char const *restrict _nptr, char **restrict _endptr);
unsigned fract strtofxur(char const *restrict _nptr, char **restrict _endptr);
unsigned long fract strtofxulr(char const *restrict _nptr, char **restrict _endptr);
unsigned short accum strtofxuhk(char const *restrict _nptr, char **restrict _endptr);
unsigned accum strtofxuk(char const *restrict _nptr, char **restrict _endptr);
unsigned long accum strtofxulk(char const *restrict _nptr, char **restrict _endptr);

//
// Implementation extensions.
//

short fract strtofxhr_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr);
fract strtofxr_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr);
long fract strtofxlr_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr);
short accum strtofxhk_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr);
accum strtofxk_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr);
long accum strtofxlk_str(char const __str_ars *restrict _nptr, char __str_ars **restrict _endptr);

unsigned short fract strtofxuhr_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr);
unsigned fract strtofxur_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr);
unsigned long fract strtofxulr_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr);
unsigned short accum strtofxuhk_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr);
unsigned accum strtofxuk_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr);
unsigned long accum strtofxulk_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr);

#ifdef __cplusplus
}
#endif


#endif//__GDCC_Header__C__stdfix_h__

