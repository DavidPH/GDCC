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

#ifndef GDCC__Core__Number_H__
#define GDCC__Core__Number_H__

#include <cstdint>
#include <gmpxx.h>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      // Unlimited precision numeric types.
      typedef mpf_class Float;
      typedef mpz_class Integ;
      typedef mpq_class Ratio;

      // Faster, native numeric types.
      // FastU shall be suitable for storing the sizes of the target's objects.
      typedef long double   FastF;
      typedef   signed long FastI;
      typedef unsigned long FastU;

      //
      // NumberCastType
      //
      // Implements ::number_cast.
      //
      template<typename Out, typename In> struct NumberCastType
      {
         static Out Cast(In &&i);
      };

      // NumberCastType<FastI, Integ>
      template<> struct NumberCastType<FastI, Integ>
         {static FastI Cast(Integ const &i) {return i.get_si();}};

      // NumberCastType<FastU, Integ>
      template<> struct NumberCastType<FastU, Integ>
         {static FastU Cast(Integ const &i) {return i.get_ui();}};

      // NumberCastType<FastI>
      template<typename In> struct NumberCastType<FastI, In>
         {static FastI Cast(In &&i) {return static_cast<Integ>(i).get_si();}};

      // NumberCastType<FastU>
      template<typename In> struct NumberCastType<FastU, In>
         {static FastU Cast(In &&i) {return static_cast<Integ>(i).get_ui();}};

      // NumberCastType<Float>
      template<typename In> struct NumberCastType<Float, In>
         {static Float Cast(In &&i) {return static_cast<Float>(i);}};

      // NumberCastType<Integ>
      template<typename In> struct NumberCastType<Integ, In>
         {static Integ Cast(In &&i) {return static_cast<Integ>(i);}};

      // NumberCastType<Ratio>
      template<typename In> struct NumberCastType<Ratio, In>
         {static Ratio Cast(In &&i) {return static_cast<Ratio>(i);}};
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

template<typename Out, typename In> Out number_cast(In &&i);

//
// number_cast
//
// Handles conversions involving unlimited precision numeric types.
//
template<typename Out, typename In> Out number_cast(In &&i)
{
   return GDCC::Core::NumberCastType<Out, In>::Cast(std::forward<In>(i));
}

#endif//GDCC__Core__Number_H__

