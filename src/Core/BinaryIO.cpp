//-----------------------------------------------------------------------------
//
// Copyright (C) 2016-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Binary I/O.
//
//-----------------------------------------------------------------------------

#include "Core/BinaryIO.hpp"

#include "Core/String.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   //
   // ReadLE4
   //
   std::uint_fast32_t ReadLE4(char const *in)
   {
      return
         static_cast<std::uint_fast32_t>(static_cast<unsigned char>(in[0])) <<  0 |
         static_cast<std::uint_fast32_t>(static_cast<unsigned char>(in[1])) <<  8 |
         static_cast<std::uint_fast32_t>(static_cast<unsigned char>(in[2])) << 16 |
         static_cast<std::uint_fast32_t>(static_cast<unsigned char>(in[3])) << 24;
   }

   //
   // ReadStrN
   //
   String ReadStrN(char const *in, std::size_t n)
   {
      char const *end = in;
      for(; n && *end; ++end, --n) {}
      return {in, end};
   }

   //
   // WriteLE4
   //
   void WriteLE4(std::ostream &out, std::uint_fast32_t in)
   {
      out.put((in >>  0) & 0xFF);
      out.put((in >>  8) & 0xFF);
      out.put((in >> 16) & 0xFF);
      out.put((in >> 24) & 0xFF);
   }

   //
   // WriteStrN
   //
   void WriteStrN(std::ostream &out, String in, std::size_t n)
   {
      for(auto itr = in.begin(), end = in.end(); n && in != end; ++itr, --n)
         out.put(*itr);

      for(; n; --n)
         out.put('\0');
   }
}

// EOF

