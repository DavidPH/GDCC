//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// AS input stream.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AS__IStream_H__
#define GDCC__AS__IStream_H__

#include "../Core/CommentBuf.hpp"
#include "../Core/EscapeBuf.hpp"
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
      struct Token;
   }

   namespace AS
   {
      //
      // IStream
      //
      class IStream : public std::istream
      {
      public:
         IStream(std::streambuf &buf, Core::String file) : std::istream{&cbuf},
            obuf{buf, file}, ebuf{obuf}, cbuf{ebuf} {}

         void disableComments() {rdbuf(&obuf);}

         void enableComments() {rdbuf(&cbuf);}

         Core::Origin getOrigin() const {return obuf.getOrigin();}

         using CommentsHold = Core::FeatureHold<IStream,
            &IStream::disableComments, &IStream::enableComments>;

         CommentsHold holdComments() {return CommentsHold(*this);}

      protected:
         using OBuf = Core::OriginBuf<8>;
         using EBuf = Core::StripEscapeBuf<8, 1, 1, char, '\n'>;
         using CBuf = Core::LineCommentBuf<8, 1, 1, char, ';'>;

         OBuf obuf;
         EBuf ebuf;
         CBuf cbuf;
      };
   }
}

//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AS
   {
      IStream &operator >> (IStream &in, Core::Token &out);
   }
}

#endif//GDCC__AS__IStream_H__

