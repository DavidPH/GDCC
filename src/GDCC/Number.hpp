//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Numeric typedefs.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Number_H__
#define GDCC__Number_H__

#include <cstdint>
#include <gmpxx.h>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   // Unlimited precision numeric types.
   typedef mpf_class Float;
   typedef mpz_class Integ;

   // Faster, native numeric types.
   // FastU shall be suitable for storing the sizes of the target's objects.
   typedef long double    FastF;
   typedef std:: intmax_t FastI;
   typedef std::uintmax_t FastU;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

template<typename T>
T number_cast(GDCC::Integ const &i);

template<>
inline GDCC::FastI number_cast<GDCC::FastI>(GDCC::Integ const &i) {return i.get_si();}
template<>
inline GDCC::FastU number_cast<GDCC::FastU>(GDCC::Integ const &i) {return i.get_ui();}

#endif//GDCC__Number_H__

