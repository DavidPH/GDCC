//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Numeric typedefs and utilities.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Number_H__
#define GDCC__Core__Number_H__

#include "../Core/Types.hpp"

#include <cstdint>
#include <utility>

#if GDCC_Core_BigNum
#include <gmpxx.h>
#endif


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   #if GDCC_Core_BigNum
   // Unlimited precision numeric types.
   typedef mpf_class Float;
   typedef mpz_class Integ;
   typedef mpq_class Ratio;
   #endif

   // Faster, native numeric types.
   // FastU shall be suitable for storing the sizes of the target's objects.
   typedef long double   FastF;
   typedef   signed long FastI;
   typedef unsigned long FastU;

   //
   // NumberCast_T
   //
   // Implements NumberCast.
   //
   template<typename Out, typename In> struct NumberCast_T
   {
      static Out Cast(In &&i);
   };

   #if GDCC_Core_BigNum
   // NumberCast_T<FastI, Integ>
   template<> struct NumberCast_T<FastI, Integ>
      {static FastI Cast(Integ const &i) {return i.get_si();}};

   // NumberCast_T<FastU, Integ>
   template<> struct NumberCast_T<FastU, Integ>
      {static FastU Cast(Integ const &i) {return i.get_ui();}};

   // NumberCast_T<FastI>
   template<typename In> struct NumberCast_T<FastI, In>
      {static FastI Cast(In &&i) {return static_cast<Integ>(i).get_si();}};

   // NumberCast_T<FastU>
   template<typename In> struct NumberCast_T<FastU, In>
      {static FastU Cast(In &&i) {return static_cast<Integ>(i).get_ui();}};

   // NumberCast_T<Float>
   template<typename In> struct NumberCast_T<Float, In>
      {static Float Cast(In &&i) {return static_cast<Float>(i);}};

   // NumberCast_T<Integ>
   template<typename In> struct NumberCast_T<Integ, In>
      {static Integ Cast(In &&i) {return static_cast<Integ>(i);}};

   // NumberCast_T<Ratio>
   template<typename In> struct NumberCast_T<Ratio, In>
      {static Ratio Cast(In &&i) {return static_cast<Ratio>(i);}};
   #endif
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   //
   // NumberCast
   //
   // Handles conversions involving unlimited precision numeric types.
   //
   template<typename Out, typename In> Out NumberCast(In &&i)
   {
      return NumberCast_T<Out, In>::Cast(std::forward<In>(i));
   }
}

namespace std
{
   #if GDCC_Core_BigNum
   //
   // hash<mpz_class>
   //
   template<> struct hash<mpz_class>
   {
      size_t operator () (mpz_class const &i) const;
   };
   #endif
}

#endif//GDCC__Core__Number_H__

