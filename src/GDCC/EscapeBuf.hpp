//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Escape handling streambufs.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__EscapeBuf_H__
#define GDCC__EscapeBuf_H__

#include "WrapperBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   //
   // EscapeBufStrip
   //
   template<char Escape = '\n', char Start = '\\', typename Src = std::streambuf>
   class EscapeBufStrip final : public WrapperBuf<Src>
   {
   public:
      using Super = WrapperBuf<Src>;


      explicit EscapeBufStrip(Src &src_) : Super{src_} {}

   protected:
      using Super::src;

      //
      // underflow
      //
      virtual int underflow()
      {
         int c = Super::underflow();

         if(c == Start)
         {
            if(src.sgetc() == Escape) return this->gbump(1), src.sbumpc(), underflow();
            if(src.sgetc() == EOF) return *this->gptr() = Escape;
         }

         return c;
      }
   };
}

#endif//GDCC__EscapeBuf_H__

