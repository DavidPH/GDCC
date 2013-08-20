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

#include "ConcatTBuf.hpp"
#include "IStream.hpp"
#include "StringTBuf.hpp"

#include "GDCC/StreamTBuf.hpp"
#include "GDCC/TokenStream.hpp"
#include "GDCC/WSpaceTBuf.hpp"


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
      TStream(std::streambuf &buf, GDCC::String file) : GDCC::TokenStream{&cbuf},
         istr{buf, file}, tbuf{istr}, wbuf{tbuf}, sbuf{wbuf}, cbuf{sbuf} {}

   protected:
      using CBuf = ConcatTBuf;
      using SBuf = StringTBuf;
      using WBuf = GDCC::WSpaceTBuf;
      using TBuf = GDCC::StreamTBuf<IStream>;

      IStream istr;
      TBuf    tbuf;
      WBuf    wbuf;
      SBuf    sbuf;
      CBuf    cbuf;
   };
}

#endif//C__TStream_H__

