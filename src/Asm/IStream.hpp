//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Asm input stream.
//
//-----------------------------------------------------------------------------

#ifndef Asm__IStream_H__
#define Asm__IStream_H__

#include "GDCC/CommentBuf.hpp"
#include "GDCC/EscapeBuf.hpp"
#include "GDCC/FeatureHold.hpp"
#include "GDCC/OriginBuf.hpp"

#include <istream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   struct Token;
}

namespace Asm
{
   //
   // IStream
   //
   class IStream : public std::istream
   {
   public:
      IStream(std::streambuf &buf, GDCC::String file) : std::istream{&cbuf},
         obuf{buf, file}, ebuf{obuf}, cbuf{ebuf} {}

      void disableComments() {rdbuf(&obuf);}

      void enableComments() {rdbuf(&cbuf);}

      GDCC::Origin getOrigin() const {return obuf.getOrigin();}

      GDCC::FeatureHold<IStream, &IStream::disableComments, &IStream::enableComments> holdComments()
         {return GDCC::FeatureHold<IStream, &IStream::disableComments, &IStream::enableComments>(*this);}

   protected:
      typedef GDCC::OriginBuf<>                      OBuf;
      typedef GDCC::EscapeBufStrip<'\n', '\\', OBuf> EBuf;
      typedef GDCC::CommentBufLine<';', EBuf>        CBuf;

      OBuf obuf;
      EBuf ebuf;
      CBuf cbuf;
   };
}

//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Asm
{
   IStream &operator >> (IStream &in, GDCC::Token &out);
}

#endif//Asm__IStream_H__

