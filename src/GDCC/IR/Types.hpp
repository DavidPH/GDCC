//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation types.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__Types_H__
#define GDCC__IR__Types_H__

#include <cstdint>
#include <gmpxx.h>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      // Unlimited precision numeric types.
      typedef mpz_class Integ;
      typedef mpf_class Float;

      // Faster, native numeric types.
      // These shall be suitable for storing the sizes of the target's objects.
      typedef std:: intmax_t FastI;
      typedef std::uintmax_t FastU;

      //
      // Linkage
      //
      enum class Linkage
      {
         None,
         ExtACS,
         ExtASM,
         ExtC,
         ExtCXX,
         ExtDS,
         IntC,
         IntCXX,
      };
   }
}

#endif//GDCC__IR__Types_H__

