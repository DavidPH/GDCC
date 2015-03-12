//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
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

#include "../Core/Parse.hpp"


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
         StringTBuf(Core::TokenBuf &src_,
            bool (*escaper_)(std::ostream &, std::istream &, char) = Core::ParseEscapeC) :
            src(src_), escaper{escaper_} {sett(buf, buf, buf);}

      protected:
         virtual void underflow();

         Core::TokenBuf &src;
         Core::Token     buf[1];

         bool (*escaper)(std::ostream &, std::istream &, char);
      };
   }
}

#endif//GDCC__CPP__StringTBuf_H__

