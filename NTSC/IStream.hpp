//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// NTSC input stream.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__NTSC__IStream_H__
#define GDCC__NTSC__IStream_H__

#include "../NTSC/Types.hpp"

#include "../Core/OriginBuf.hpp"

#include <istream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::NTSC
{
   //
   // IStream
   //
   class IStream : public std::istream
   {
   public:
      explicit IStream(std::streambuf &buf, Core::String file) :
         std::istream{&obuf}, obuf{buf, {file, 1, 1}} {}

      Core::OriginSource &getOriginSource() {return obuf;}

   protected:
      using OBuf = Core::OriginBuf<8>;

      OBuf obuf;
   };
}

#endif//GDCC__NTSC__IStream_H__

