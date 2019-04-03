//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// String concatenating token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__NTSC__ConcatTBuf_H__
#define GDCC__NTSC__ConcatTBuf_H__

#include "../NTSC/Types.hpp"

#include "../Core/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::NTSC
{
   //
   // ConcatTBuf
   //
   class ConcatTBuf : public Core::TokenBuf
   {
   public:
      ConcatTBuf(Core::TokenBuf &src_) : src(src_) {}

   protected:
      virtual void underflow();

      Core::TokenBuf &src;
      Core::Token     buf[1];
   };
}

#endif//GDCC__NTSC__ConcatTBuf_H__

