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

#ifndef GDCC__AS__TStream_H__
#define GDCC__AS__TStream_H__

#include "../AS/IStream.hpp"

#include "../Core/StreamTBuf.hpp"
#include "../Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AS
   {
      //
      // TStream
      //
      class TStream : public Core::TokenStream
      {
      public:
         TStream(std::streambuf &buf_, Core::String file) :
            Core::TokenStream{&tbuf}, istr{buf_, file}, tbuf{istr} {}

      protected:
         using IStr = IStream;
         using TBuf = Core::StreamTBuf<IStr, 8>;

         IStr istr;
         TBuf tbuf;
      };
   }
}

#endif//GDCC__AS__TStream_H__

