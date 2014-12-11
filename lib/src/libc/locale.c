//-----------------------------------------------------------------------------
//
// Copyright(C) 2014 David Hill
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

#include <locale.h>

#include <limits.h>


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static struct lconv locale_C =
{
   .decimal_point      = (char *)".",
   .thousands_sep      = (char *)"",
   .grouping           = (char *)"",
   .mon_decimal_point  = (char *)"",
   .mon_thousands_sep  = (char *)"",
   .mon_grouping       = (char *)"",
   .positive_sign      = (char *)"",
   .negative_sign      = (char *)"",
   .currency_symbol    = (char *)"",
   .frac_digits        = CHAR_MAX,
   .p_cs_precedes      = CHAR_MAX,
   .n_cs_precedes      = CHAR_MAX,
   .p_sep_by_space     = CHAR_MAX,
   .n_sep_by_space     = CHAR_MAX,
   .p_sign_posn        = CHAR_MAX,
   .n_sign_posn        = CHAR_MAX,
   .int_curr_symbol    = (char *)"",
   .int_frac_digits    = CHAR_MAX,
   .int_p_cs_precedes  = CHAR_MAX,
   .int_n_cs_precedes  = CHAR_MAX,
   .int_p_sep_by_space = CHAR_MAX,
   .int_n_sep_by_space = CHAR_MAX,
   .int_p_sign_posn    = CHAR_MAX,
   .int_n_sign_posn    = CHAR_MAX,
};


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//=========================================================
// Locale control.
//

//
// setlocale
//
char *setlocale(int category, const char *locale)
{
   return NULL;
}

//=========================================================
// Numeric formatting convention inquiry.
//

//
// localeconv
//
struct lconv *localeconv(void)
{
   return &locale_C;
}

// EOF

