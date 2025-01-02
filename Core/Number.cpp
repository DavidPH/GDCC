//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Numeric typedefs and utilities.
//
//-----------------------------------------------------------------------------

#include "Core/Number.hpp"

#include <memory>


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

namespace GDCC::Core
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


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   #if GDCC_Core_BigNum
   //
   // NumberCast_T<FastI, Integ>::Cast
   //
   FastI NumberCast_T<FastI, Integ>::Cast(Integ const &in)
   {
      return static_cast<FastI>(NumberCast_T<FastU, Integ>::Cast(in));
   }

   //
   // NumberCast_T<FastU, Integ>::Cast
   //
   FastU NumberCast_T<FastU, Integ>::Cast(Integ const &in)
   {
      FastU out;
      mpz_srcptr mp = in.get_mpz_t();
      switch(mpz_size(mp))
      {
      case 0: return 0;

      case 1: out = mpz_getlimbn(mp, 0); break;

      default:
         out = mpz_getlimbn(mp, 0);
         #if GDCC_Core_SizeLongLong > GDCC_Core_SizeLong
         out |= FastU(mpz_getlimbn(mp, 1)) << GDCC_Core_BitsLong;
         #endif
         break;
      }

      if(mpz_sgn(mp) < 0)
         out = ~out + 1;

      return out;
   }

   //
   // NumberCast_T<Integ, FastI>::Cast
   //
   Integ NumberCast_T<Integ, FastI>::Cast(FastI in)
   {
      #if GDCC_Core_SizeLongLong <= GDCC_Core_SizeLong
      return Integ(static_cast<long>(in));
      #else
      if(in >= 0)
         return NumberCast_T<Integ, FastU>::Cast(static_cast<FastU>(in));

      return -NumberCast_T<Integ, FastU>::Cast(static_cast<FastU>(-in));
      #endif
   }

   //
   // NumberCast_T<Integ, FastU>::Cast
   //
   Integ NumberCast_T<Integ, FastU>::Cast(FastU in)
   {
      #if GDCC_Core_SizeLongLong <= GDCC_Core_SizeLong
      return Integ(static_cast<unsigned long>(in));
      #else
      mp_size_t n = (GDCC_Core_SizeLongLong + GDCC_Core_SizeLong - 1) / GDCC_Core_SizeLong;
      Integ     out;

      auto buf = mpz_limbs_write(out.get_mpz_t(), n);

      for(mp_size_t i = 0; i != n; ++i, in >>= GDCC_Core_BitsLong)
         buf[i] = static_cast<mp_limb_t>(in);

      mpz_limbs_finish(out.get_mpz_t(), n);

      return out;
      #endif
   }
   #endif

   #if GDCC_Core_BigNum
   //
   // WriteNumber
   //
   std::ostream &WriteNumber(std::ostream &out, Float const &in)
   {
      std::size_t lenDec = gmp_snprintf(nullptr, 0, "%.Ff", in.get_mpf_t()) + 1;
      std::size_t lenHex = gmp_snprintf(nullptr, 0, "%.FA", in.get_mpf_t()) + 1;

      std::unique_ptr<char[]> buf;
      if(lenHex < lenDec)
         gmp_snprintf((buf.reset(new char[lenHex]), buf.get()), lenHex, "%.FA", in.get_mpf_t());
      else
         gmp_snprintf((buf.reset(new char[lenDec]), buf.get()), lenDec, "%.Ff", in.get_mpf_t());

      return out << buf.get();
   }
   #endif

   #if GDCC_Core_BigNum
   //
   // WriteNumberDec
   //
   std::ostream &WriteNumberDec(std::ostream &out, Float const &in)
   {
      std::size_t len = gmp_snprintf(nullptr, 0, "%.Ff", in.get_mpf_t()) + 1;
      std::unique_ptr<char[]> buf{new char[len]};
      gmp_snprintf(buf.get(), len, "%.Ff", in.get_mpf_t());
      return out << buf.get();
   }

   //
   // WriteNumberDec
   //
   std::ostream &WriteNumberDec(std::ostream &out, Integ const &in)
   {
      return out << in.get_str(10);
   }
   #endif

   #if GDCC_Core_BigNum
   //
   // WriteNumberHex
   //
   std::ostream &WriteNumberHex(std::ostream &out, Float const &in)
   {
      std::size_t len = gmp_snprintf(nullptr, 0, "%.FA", in.get_mpf_t()) + 1;
      std::unique_ptr<char[]> buf{new char[len]};
      gmp_snprintf(buf.get(), len, "%.FA", in.get_mpf_t());
      return out << buf.get();
   }

   //
   // WriteNumberHex
   //
   std::ostream &WriteNumberHex(std::ostream &out, Integ const &in)
   {
      return out << in.get_str(-16);
   }
   #endif
}

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
         h += mpz_getlimbn(mp, n);

      if(mpz_sgn(mp) < 0)
         h = ~h;

      return h;
   }
   #endif
}

// EOF

