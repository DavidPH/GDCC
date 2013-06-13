//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// MageDefs input stream.
//
//-----------------------------------------------------------------------------

#ifndef MageDefs__IStream_H__
#define MageDefs__IStream_H__

#include "GDCC/CommentBuf.hpp"

#include <istream>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   struct Token;
}

namespace MageDefs
{
   //
   // IStream
   //
   class IStream : public std::istream
   {
   public:
      explicit IStream(std::streambuf &buf_) : std::istream{&cbuf}, buf{buf_},
         cbuf{buf} {unsetf(std::ios_base::skipws);}

      void disableComments() {rdbuf(&buf);}

      void enableComments() {rdbuf(&cbuf);}

   protected:
      std::streambuf            &buf;
      GDCC::CommentBufLine<'#'> cbuf;
   };
}

//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace MageDefs
{
   IStream &operator >> (IStream &in, GDCC::Token &out);
}

#endif//MageDefs__IStream_H__

