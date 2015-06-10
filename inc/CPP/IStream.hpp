//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C input stream.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__IStream_H__
#define GDCC__CPP__IStream_H__

#include "../CPP/TrigraphBuf.hpp"

#include "../Core/CommentBuf.hpp"
#include "../Core/EscapeBuf.hpp"
#include "../Core/FeatureHold.hpp"
#include "../Core/LineTermBuf.hpp"
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

   namespace CPP
   {
      //
      // IStreamHeader
      //
      class IStreamHeader : public std::istream
      {
      public:
         IStreamHeader(std::streambuf *buf) : std::istream{buf} {}

         virtual void setNeedHeader() = 0;


         static bool GetHeader(std::istream &in, Core::Token &out);
      };

      //
      // IStream
      //
      class IStream : public IStreamHeader
      {
      public:
         IStream(std::streambuf &buf, Core::String file, std::size_t line = 1) :
            IStreamHeader{&cbuf}, needHeader{false}, lbuf{buf},
            obuf{lbuf, {file, line, 1}}, tbuf{obuf}, ebuf{tbuf}, cbuf{ebuf} {}

         void disableComments() {rdbuf(&ebuf);}

         void enableComments() {rdbuf(&cbuf);}

         Core::Origin getOrigin() const {return obuf.getOrigin();}

         using CommentsHold = Core::FeatureHold<IStream,
            &IStream::disableComments, &IStream::enableComments>;

         CommentsHold holdComments() {return CommentsHold(*this);}

         virtual void setNeedHeader() {needHeader = true;}

         bool needHeader : 1;

      protected:
         using LBuf = Core::LineTermBuf<8>;
         using OBuf = Core::OriginBuf<8, 2>;
         using TBuf = TrigraphBuf<8>;
         using EBuf = Core::StripEscapeBuf<8, 1, 1, char, '\n'>;
         using CBuf = Core::CCommentBuf<8>;

         LBuf lbuf;
         OBuf obuf;
         TBuf tbuf;
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
   namespace CPP
   {
      IStream &operator >> (IStream &in, Core::Token &out);
   }
}

#endif//GDCC__CPP__IStream_H__

