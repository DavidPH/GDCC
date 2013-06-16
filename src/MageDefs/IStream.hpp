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
   // FeatureHold
   //
   template<typename T, void(T::*D)(), void(T::*E)()> class FeatureHold
   {
   public:
      FeatureHold(FeatureHold const &) = delete;
      FeatureHold(FeatureHold &&hold) : str{hold.str} {hold.str = nullptr;}
      explicit FeatureHold(T &str_) : str{&str_} {(str->*D)();}
      ~FeatureHold() {if(str) (str->*E)();}

   private:
      T *str;
   };

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

      FeatureHold<IStream, &IStream::disableComments, &IStream::enableComments> holdComments()
         {return FeatureHold<IStream, &IStream::disableComments, &IStream::enableComments>(*this);}

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

