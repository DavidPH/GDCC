//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

#include "../Core/CommentBuf.hpp"
#include "../Core/FeatureHold.hpp"
#include "../Core/LineTermBuf.hpp"
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
         std::istream{&cbuf}, lbuf{buf}, obuf{lbuf, {file, 1, 1}}, cbuf{obuf} {}

      void disableComments() {rdbuf(&obuf);}

      void enableComments() {rdbuf(&cbuf);}

      Core::Origin getOrigin() const {return obuf.getOrigin();}

      using CommentsHold = Core::FeatureHold<IStream,
         &IStream::disableComments, &IStream::enableComments>;

      CommentsHold holdComments() {return CommentsHold(*this);}

   protected:
      using LBuf = Core::LineTermBuf<8>;
      using OBuf = Core::OriginBuf<8>;
      using CBuf = Core::LineCommentBuf<8, 1, 1, char, '#'>;

      LBuf lbuf;
      OBuf obuf;
      CBuf cbuf;
   };
}

//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::NTSC
{
   IStream &operator >> (IStream &in, Core::Token &out);
}

#endif//GDCC__NTSC__IStream_H__

