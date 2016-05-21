//-----------------------------------------------------------------------------
//
// Copyright(C) 2014-2016 David Hill
//
// See COPYLIB for license information.
//
//-----------------------------------------------------------------------------
//
// Header for target library libc.
//
// Localization.
//
//-----------------------------------------------------------------------------

#ifndef __GDCC_Header__C__locale_h__
#define __GDCC_Header__C__locale_h__

#include <bits/features.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// NULL
//
#ifndef NULL
#define NULL 0
#endif

//
// LC_*
//
#define LC_ALL      0
#define LC_COLLATE  1
#define LC_CTYPE    2
#define LC_MONETARY 3
#define LC_NUMERIC  4
#define LC_TIME     5


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// lconv
//
struct lconv
{
   char *decimal_point;
   char *thousands_sep;
   char *grouping;
   char *mon_decimal_point;
   char *mon_thousands_sep;
   char *mon_grouping;
   char *positive_sign;
   char *negative_sign;
   char *currency_symbol;
   char  frac_digits;
   char  p_cs_precedes;
   char  n_cs_precedes;
   char  p_sep_by_space;
   char  n_sep_by_space;
   char  p_sign_posn;
   char  n_sign_posn;
   char *int_curr_symbol;
   char  int_frac_digits;
   char  int_p_cs_precedes;
   char  int_n_cs_precedes;
   char  int_p_sep_by_space;
   char  int_n_sep_by_space;
   char  int_p_sign_posn;
   char  int_n_sign_posn;
};


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// Locale control.
//

char *setlocale(int _category, const char *_locale);

//
// Numeric formatting convention inquiry.
//

struct lconv *localeconv(void);

#endif//__GDCC_Header__C__locale_h__

