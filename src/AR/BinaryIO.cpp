//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Binary I/O.
//
//-----------------------------------------------------------------------------

#include "AR/BinaryIO.hpp"

#include "Core/String.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace AR
   {
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
      void WriteStrN(std::ostream &out, Core::String in, std::size_t n)
      {
         for(auto itr = in.begin(), end = in.end(); n && in != end; ++itr, --n)
            out.put(*itr);

         for(; n; --n)
            out.put('\0');
      }
   }
}

// EOF

