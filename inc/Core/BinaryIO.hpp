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

#ifndef GDCC__Core__BinaryIO_H__
#define GDCC__Core__BinaryIO_H__

#include "../Core/Types.hpp"

#include <ostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::Core
{
   std::uint_fast32_t ReadLE4(char const *in);

   String ReadStrN(char const *in, std::size_t n);

   void WriteLE4(std::ostream &out, std::uint_fast32_t in);

   void WriteStrN(std::ostream &out, String in, std::size_t n);
}

#endif//GDCC__Core__BinaryIO_H__

