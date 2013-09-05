//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Assembly token stream.
//
//-----------------------------------------------------------------------------

#ifndef Asm__TStream_H__
#define Asm__TStream_H__

#include "IStream.hpp"

#include "GDCC/StreamTBuf.hpp"
#include "GDCC/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace Asm
{
   //
   // TStream
   //
   class TStream : public GDCC::TokenStream
   {
   public:
      TStream(std::streambuf &buf, GDCC::String file) :
         GDCC::TokenStream{&tbuf}, istr{buf, file}, tbuf{istr} {}

   protected:
      using IStr = IStream;
      using TBuf = GDCC::StreamTBuf<IStream, 8>;

      IStr istr;
      TBuf tbuf;
   };
}

#endif//Asm__TStream_H__

