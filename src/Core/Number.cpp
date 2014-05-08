//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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
// Global Functions                                                           |
//

namespace std
{
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
}

// EOF

