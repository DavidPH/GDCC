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

#ifndef GDCC__AR__BinaryIO_H__
#define GDCC__AR__BinaryIO_H__

#include <cstdint>
#include <ostream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      class String;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace AR
   {
      void WriteLE4(std::ostream &out, std::uint_fast32_t in);

      void WriteStrN(std::ostream &out, Core::String in, std::size_t n);
   }
}

#endif//GDCC__AR__BinaryIO_H__

