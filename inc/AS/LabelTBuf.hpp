//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Function label mangling token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__LabelTBuf_H__
#define GDCC__CPP__LabelTBuf_H__

#include "../Core/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AS
   {
      //
      // LabelTBuf
      //
      class LabelTBuf : public Core::TokenBuf
      {
      public:
         LabelTBuf(Core::TokenBuf &src_, Core::String prefix_) :
            src(src_), prefix{prefix_} {sett(buf, buf, buf);}

      protected:
         virtual void underflow();

         Core::TokenBuf &src;
         Core::Token     buf[1];
         Core::String prefix;
      };
   }
}

#endif//GDCC__CPP__LabelTBuf_H__

