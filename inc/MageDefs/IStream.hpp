//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// MageDefs input stream.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__MageDefs__IStream_H__
#define GDCC__MageDefs__IStream_H__

#include "../Core/CommentBuf.hpp"
#include "../Core/FeatureHold.hpp"
#include "../Core/OriginBuf.hpp"

#include <istream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      class Token;
   }

   namespace MageDefs
   {
      //
      // IStream
      //
      class IStream : public std::istream
      {
      public:
         explicit IStream(std::streambuf &buf, Core::String file) :
            std::istream{&cbuf}, obuf{buf, file}, cbuf{obuf} {}

         void disableComments() {rdbuf(&obuf);}

         void enableComments() {rdbuf(&cbuf);}

         Core::Origin getOrigin() const {return obuf.getOrigin();}

         using CommentsHold = Core::FeatureHold<IStream,
            &IStream::disableComments, &IStream::enableComments>;

         CommentsHold holdComments() {return CommentsHold(*this);}

      protected:
         using OBuf = Core::OriginBuf<8>;
         using CBuf = Core::LineCommentBuf<8, 1, 1, char, '#'>;

         OBuf obuf;
         CBuf cbuf;
      };
   }
}

//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace MageDefs
   {
      IStream &operator >> (IStream &in, Core::Token &out);
   }
}

#endif//GDCC__MageDefs__IStream_H__

