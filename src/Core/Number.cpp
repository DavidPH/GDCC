//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Numeric typedefs and utilities.
//
//-----------------------------------------------------------------------------

#include "Core/Number.hpp"


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

namespace GDCC
{
   namespace Core
   {
      #if GDCC_Core_BigNum
      // Highly unfortunate hack to set default precision.
      // Will be removed when use of mpf_class is replaced with mpfr_t.
      #ifdef __GNUC__
      __attribute__((__unused__))
      #endif
      auto static PrecInit = [](){mpf_set_default_prec(256); return 0;}();
      #endif
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace std
{
   #if GDCC_Core_BigNum
   //
   // hash<mpz_class>::operator (mpz_class)
   //
   size_t hash<mpz_class>::operator () (mpz_class const &i) const
   {
      size_t h = 0;

      mpz_srcptr mp = i.get_mpz_t();

      for(size_t n = 0, e = mpz_size(mp); n != e; ++n)
         h ^= mpz_getlimbn(mp, n);

      if(mpz_sgn(mp) < 0)
         h = ~h;

      return h;
   }
   #endif
}

// EOF

