//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C token stream.
//
//-----------------------------------------------------------------------------

#ifndef C__TStream_H__
#define C__TStream_H__

#include "IStream.hpp"

#include "GDCC/StreamTBuf.hpp"
#include "GDCC/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace C
{
   //
   // TStream
   //
   class TStream : public GDCC::TokenStream
   {
   public:
      TStream(std::streambuf &buf, GDCC::String file) : GDCC::TokenStream{&tbuf},
         istr{buf, file}, tbuf{istr} {}

   protected:
      using TBuf = GDCC::StreamTBuf<IStream>;

      IStream istr;
      TBuf    tbuf;
   };
}

#endif//C__TStream_H__

