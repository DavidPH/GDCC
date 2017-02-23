//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2017 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// Format conversion of integer types.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__inttypes_h__
#define __GDCC_Header__C__inttypes_h__

#include <stdint.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// PRId*
//
#if !__GDCC_Family__ZDACS__
#define PRId8       "hhd"
#define PRId16       "hd"
#endif
#define PRId32        "d"
#define PRId64       "ld"
#define PRId96      "lld"

#define PRIdLEAST8    "d"
#define PRIdLEAST16   "d"
#define PRIdLEAST32   "d"
#define PRIdLEAST64  "ld"
#define PRIdLEAST96 "lld"

#define PRIdFAST8     "d"
#define PRIdFAST16    "d"
#define PRIdFAST32    "d"
#define PRIdFAST64   "ld"
#define PRIdFAST96  "lld"

#define PRIdMAX     "lld"
#define PRIdPTR       "d"

//
// PRIi*
//
#if !__GDCC_Family__ZDACS__
#define PRIi8       "hhi"
#define PRIi16       "hi"
#endif
#define PRIi32        "i"
#define PRIi64       "li"
#define PRIi96      "lli"

#define PRIiLEAST8    "i"
#define PRIiLEAST16   "i"
#define PRIiLEAST32   "i"
#define PRIiLEAST64  "li"
#define PRIiLEAST96 "lli"

#define PRIiFAST8     "i"
#define PRIiFAST16    "i"
#define PRIiFAST32    "i"
#define PRIiFAST64   "li"
#define PRIiFAST96  "lli"

#define PRIiMAX     "lli"
#define PRIiPTR       "i"

//
// PRIo*
//
#if !__GDCC_Family__ZDACS__
#define PRIo8       "hho"
#define PRIo16       "ho"
#endif
#define PRIo32        "o"
#define PRIo64       "lo"
#define PRIo96      "llo"

#define PRIoLEAST8    "o"
#define PRIoLEAST16   "o"
#define PRIoLEAST32   "o"
#define PRIoLEAST64  "lo"
#define PRIoLEAST96 "llo"

#define PRIoFAST8     "o"
#define PRIoFAST16    "o"
#define PRIoFAST32    "o"
#define PRIoFAST64   "lo"
#define PRIoFAST96  "llo"

#define PRIoMAX     "llo"
#define PRIoPTR       "o"

//
// PRIu*
//
#if !__GDCC_Family__ZDACS__
#define PRIu8       "hhu"
#define PRIu16       "hu"
#endif
#define PRIu32        "u"
#define PRIu64       "lu"
#define PRIu96      "llu"

#define PRIuLEAST8    "u"
#define PRIuLEAST16   "u"
#define PRIuLEAST32   "u"
#define PRIuLEAST64  "lu"
#define PRIuLEAST96 "llu"

#define PRIuFAST8     "u"
#define PRIuFAST16    "u"
#define PRIuFAST32    "u"
#define PRIuFAST64   "lu"
#define PRIuFAST96  "llu"

#define PRIuMAX     "llu"
#define PRIuPTR       "u"

//
// PRIx*
//
#if !__GDCC_Family__ZDACS__
#define PRIx8       "hhx"
#define PRIx16       "hx"
#endif
#define PRIx32        "x"
#define PRIx64       "lx"
#define PRIx96      "llx"

#define PRIxLEAST8    "x"
#define PRIxLEAST16   "x"
#define PRIxLEAST32   "x"
#define PRIxLEAST64  "lx"
#define PRIxLEAST96 "llx"

#define PRIxFAST8     "x"
#define PRIxFAST16    "x"
#define PRIxFAST32    "x"
#define PRIxFAST64   "lx"
#define PRIxFAST96  "llx"

#define PRIxMAX     "llx"
#define PRIxPTR       "x"

//
// PRIX*
//
#if !__GDCC_Family__ZDACS__
#define PRIX8       "hhX"
#define PRIX16       "hX"
#endif
#define PRIX32        "X"
#define PRIX64       "lX"
#define PRIX96      "llX"

#define PRIXLEAST8    "X"
#define PRIXLEAST16   "X"
#define PRIXLEAST32   "X"
#define PRIXLEAST64  "lX"
#define PRIXLEAST96 "llX"

#define PRIXFAST8     "X"
#define PRIXFAST16    "X"
#define PRIXFAST32    "X"
#define PRIXFAST64   "lX"
#define PRIXFAST96  "llX"

#define PRIXMAX     "llX"
#define PRIXPTR       "X"

//
// SCNd*
//
#if !__GDCC_Family__ZDACS__
#define SCNd8       "hhd"
#define SCNd16       "hd"
#endif
#define SCNd32        "d"
#define SCNd64       "ld"
#define SCNd96      "lld"

#define SCNdLEAST8  "hhd"
#define SCNdLEAST16  "hd"
#define SCNdLEAST32   "d"
#define SCNdLEAST64  "ld"
#define SCNdLEAST96 "lld"

#define SCNdFAST8     "d"
#define SCNdFAST16    "d"
#define SCNdFAST32    "d"
#define SCNdFAST64   "ld"
#define SCNdFAST96  "lld"

#define SCNdMAX     "lld"
#define SCNdPTR       "d"

//
// SCNi*
//
#if !__GDCC_Family__ZDACS__
#define SCNi8       "hhi"
#define SCNi16       "hi"
#endif
#define SCNi32        "i"
#define SCNi64       "li"
#define SCNi96      "lli"

#define SCNiLEAST8  "hhi"
#define SCNiLEAST16  "hi"
#define SCNiLEAST32   "i"
#define SCNiLEAST64  "li"
#define SCNiLEAST96 "lli"

#define SCNiFAST8     "i"
#define SCNiFAST16    "i"
#define SCNiFAST32    "i"
#define SCNiFAST64   "li"
#define SCNiFAST96  "lli"

#define SCNiMAX     "lli"
#define SCNiPTR       "i"

//
// SCNo*
//
#if !__GDCC_Family__ZDACS__
#define SCNo8       "hho"
#define SCNo16       "ho"
#endif
#define SCNo32        "o"
#define SCNo64       "lo"
#define SCNo96      "llo"

#define SCNoLEAST8  "hho"
#define SCNoLEAST16  "ho"
#define SCNoLEAST32   "o"
#define SCNoLEAST64  "lo"
#define SCNoLEAST96 "llo"

#define SCNoFAST8     "o"
#define SCNoFAST16    "o"
#define SCNoFAST32    "o"
#define SCNoFAST64   "lo"
#define SCNoFAST96  "llo"

#define SCNoMAX     "llo"
#define SCNoPTR       "o"

//
// SCNu*
//
#if !__GDCC_Family__ZDACS__
#define SCNu8       "hhu"
#define SCNu16       "hu"
#endif
#define SCNu32        "u"
#define SCNu64       "lu"
#define SCNu96      "llu"

#define SCNuLEAST8  "hhu"
#define SCNuLEAST16  "hu"
#define SCNuLEAST32   "u"
#define SCNuLEAST64  "lu"
#define SCNuLEAST96 "llu"

#define SCNuFAST8     "u"
#define SCNuFAST16    "u"
#define SCNuFAST32    "u"
#define SCNuFAST64   "lu"
#define SCNuFAST96  "llu"

#define SCNuMAX     "llu"
#define SCNuPTR       "u"

//
// SCNx*
//
#if !__GDCC_Family__ZDACS__
#define SCNx8       "hhx"
#define SCNx16       "hx"
#endif
#define SCNx32        "x"
#define SCNx64       "lx"
#define SCNx96      "llx"

#define SCNxLEAST8  "hhx"
#define SCNxLEAST16  "hx"
#define SCNxLEAST32   "x"
#define SCNxLEAST64  "lx"
#define SCNxLEAST96 "llx"

#define SCNxFAST8     "x"
#define SCNxFAST16    "x"
#define SCNxFAST32    "x"
#define SCNxFAST64   "lx"
#define SCNxFAST96  "llx"

#define SCNxMAX     "llx"
#define SCNxPTR       "x"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// imaxdiv_t
//
typedef long long __div_t imaxdiv_t;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

#ifdef __cplusplus
extern "C" {
#endif

//
// Functions for greatest-width integer types.
//

intmax_t imaxabs(intmax_t _j);
imaxdiv_t imaxdiv(intmax_t _numer, intmax_t _denom);
intmax_t strtoimax(char const *restrict _nptr, char **restrict _endptr, int _base);
uintmax_t strtoumax(char const *restrict _nptr, char **restrict _endptr, int _base);
intmax_t wcstoimax(__wchar_t const *restrict _nptr, __wchar_t **restrict _endptr, int _base);
uintmax_t wcstoumax(__wchar_t const *restrict _nptr, __wchar_t **restrict _endptr, int _base);

//
// Implementation extensions.
//

intmax_t strtoimax_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr, int _base);
uintmax_t strtoumax_str(char __str_ars const *restrict _nptr, char __str_ars **restrict _endptr, int _base);

#ifdef __cplusplus
};
#endif

#endif//__GDCC_Header__C__inttypes_h__

