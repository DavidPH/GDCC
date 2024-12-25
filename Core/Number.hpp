//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2024 David Hill
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
#include <ostream>
#include <utility>

#if GDCC_Core_BigNum
#include <gmpxx.h>
#endif


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define GDCC_Core_BitsLong (GDCC_Core_SizeLong * CHAR_BIT)
#define GDCC_Core_BitsLongLong (GDCC_Core_SizeLongLong * CHAR_BIT)


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
   typedef   signed long long FastI;
   typedef unsigned long long FastU;

   //
   // NumberCast_T
   //
   // Implements NumberCast.
   //
   template<typename Out, typename In>
   struct NumberCast_T;

   // NumberCast_T<FastI, FastI>
   template<> struct NumberCast_T<FastI, FastI>
      {static FastI Cast(FastI in) {return in;}};

   // NumberCast_T<FastI, FastU>
   template<> struct NumberCast_T<FastI, FastU>
      {static FastI Cast(FastU in) {return static_cast<FastI>(in);}};

   // NumberCast_T<FastU, FastI>
   template<> struct NumberCast_T<FastU, FastI>
      {static FastU Cast(FastI in) {return static_cast<FastU>(in);}};

   // NumberCast_T<FastU, FastU>
   template<> struct NumberCast_T<FastU, FastU>
      {static FastU Cast(FastU in) {return in;}};

   #if GDCC_Core_BigNum
   // NumberCast_T<Float, Float>
   template<> struct NumberCast_T<Float, Float>
   {
      static Float Cast(Float const &in) {return in;}
      static Float Cast(Float &&in) {return std::move(in);}
   };

   // NumberCast_T<FastI, Integ>
   template<> struct NumberCast_T<FastI, Integ> {static FastI Cast(Integ const &in);};

   // NumberCast_T<FastU, Integ>
   template<> struct NumberCast_T<FastU, Integ> {static FastU Cast(Integ const &in);};

   // NumberCast_T<Integ, FastI>
   template<> struct NumberCast_T<Integ, FastI> {static Integ Cast(FastI in);};

   // NumberCast_T<Integ, FastU>
   template<> struct NumberCast_T<Integ, FastU> {static Integ Cast(FastU in);};

   // NumberCast_T<Integ, Integ>
   template<> struct NumberCast_T<Integ, Integ>
   {
      static Integ Cast(Integ const &in) {return in;}
      static Integ Cast(Integ &&in) {return std::move(in);}
   };

   // NumberCast_T<Ratio, Ratio>
   template<> struct NumberCast_T<Ratio, Ratio>
   {
      static Ratio Cast(Ratio const &in) {return in;}
      static Ratio Cast(Ratio &&in) {return std::move(in);}
   };

   // NumberCast_T<?, FastI>
   template<typename Out> struct NumberCast_T<Out, FastI>
      {static Out Cast(FastI in)
         {return static_cast<Out>(NumberCast_T<Integ, FastI>::Cast(in));}};

   // NumberCast_T<?, FastU>
   template<typename Out> struct NumberCast_T<Out, FastU>
      {static Out Cast(FastU in)
         {return static_cast<Out>(NumberCast_T<Integ, FastU>::Cast(in));}};

   // NumberCast_T<FastI>
   template<typename In> struct NumberCast_T<FastI, In>
      {static FastI Cast(In const &in)
         {return NumberCast_T<FastI, Integ>::Cast(static_cast<Integ>(in));}};

   // NumberCast_T<FastU>
   template<typename In> struct NumberCast_T<FastU, In>
      {static FastU Cast(In const &in)
         {return NumberCast_T<FastU, Integ>::Cast(static_cast<Integ>(in));}};

   // NumberCast_T<Float>
   template<typename In> struct NumberCast_T<Float, In>
      {static Float Cast(In const &in) {return static_cast<Float>(in);}};

   // NumberCast_T<Integ>
   template<typename In> struct NumberCast_T<Integ, In>
      {static Integ Cast(In const &in) {return static_cast<Integ>(in);}};

   // NumberCast_T<Ratio>
   template<typename In> struct NumberCast_T<Ratio, In>
      {static Ratio Cast(In const &in) {return static_cast<Ratio>(in);}};
   #endif
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   #if GDCC_Core_BigNum
   // Automatically picks between decimal/hex.
   std::ostream &WriteNumber(std::ostream &out, Float const &in);

   std::ostream &WriteNumberDec(std::ostream &out, Float const &in);
   std::ostream &WriteNumberDec(std::ostream &out, Integ const &in);

   std::ostream &WriteNumberHex(std::ostream &out, Float const &in);
   std::ostream &WriteNumberHex(std::ostream &out, Integ const &in);
   #endif

   //
   // NumberCast
   //
   // Handles conversions involving unlimited precision numeric types.
   //
   template<typename Out, typename In> Out NumberCast(In const &in)
   {
      return NumberCast_T<Out, In>::Cast(in);
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

