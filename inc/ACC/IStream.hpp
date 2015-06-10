//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS input stream.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__IStream_H__
#define GDCC__ACC__IStream_H__

#include "../CPP/IStream.hpp"

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

   namespace ACC
   {
      //
      // IStream
      //
      class IStream : public CPP::IStreamHeader
      {
      public:
         IStream(std::streambuf &buf, Core::String file, std::size_t line = 1) :
            CPP::IStreamHeader{&cbuf},lbuf{buf}, obuf{lbuf, {file, line, 1}},
            ebuf{obuf}, cbuf{ebuf} {}

         void disableComments() {rdbuf(&ebuf);}

         void enableComments() {rdbuf(&cbuf);}

         Core::Origin getOrigin() const {return obuf.getOrigin();}

         using CommentsHold = Core::FeatureHold<IStream,
            &IStream::disableComments, &IStream::enableComments>;

         CommentsHold holdComments() {return CommentsHold(*this);}

         virtual void setNeedHeader() {}

      protected:
         using LBuf = Core::LineTermBuf<8>;
         using OBuf = Core::OriginBuf<8, 2>;
         using EBuf = Core::StripEscapeBuf<8, 1, 1, char, '\n'>;
         using CBuf = Core::CCommentBuf<8>;

         LBuf lbuf;
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
   namespace ACC
   {
      IStream &operator >> (IStream &in, Core::Token &out);

      bool ParseEscape(std::ostream &out, std::istream &in, char escape);
   }
}

#endif//GDCC__ACC__IStream_H__

