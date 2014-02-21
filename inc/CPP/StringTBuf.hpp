//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// String parsing token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__StringTBuf_H__
#define GDCC__CPP__StringTBuf_H__

#include "../Core/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // StringTBuf
      //
      class StringTBuf : public Core::TokenBuf
      {
      public:
         StringTBuf(Core::TokenBuf &src_) : src(src_) {sett(buf, buf, buf);}

      protected:
         virtual void underflow();

         Core::TokenBuf &src;
         Core::Token     buf[1];
      };
   }
}

#endif//GDCC__CPP__StringTBuf_H__

