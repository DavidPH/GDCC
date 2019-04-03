//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Assembly token stream.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__NTSC__TStream_H__
#define GDCC__NTSC__TStream_H__

#include "../NTSC/ConcatTBuf.hpp"

#include "../Core/SourceTBuf.hpp"
#include "../Core/TokenStream.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::NTSC
{
   //
   // TStream
   //
   class TStream : public Core::TokenStream
   {
   public:
      TStream(Core::TokenSource &tsrc) :
         Core::TokenStream{&cbuf}, tbuf{tsrc}, cbuf{tbuf} {}

   protected:
      using TBuf = Core::SourceTBuf<>;
      using CBuf = ConcatTBuf;

      TBuf tbuf;
      CBuf cbuf;
   };
}

#endif//GDCC__NTSC__TStream_H__

