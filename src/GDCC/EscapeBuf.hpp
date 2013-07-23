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
   template<char Escape = '\n', char Start = '\\', typename Buf = std::streambuf>
   class EscapeBufStrip final : public WrapperBuf<Buf>
   {
   public:
      explicit EscapeBufStrip(Buf &buf_) : WrapperBuf<Buf>{buf_} {}

   protected:
      using WrapperBuf<Buf>::buf;
      using WrapperBuf<Buf>::pbc;
      using WrapperBuf<Buf>::pbb;

      //
      // uflow
      //
      virtual int uflow()
      {
         if(pbc != EOF) return pbb = pbc, pbc = EOF, pbb;
         if((pbb = buf.sbumpc()) != Start) return pbb;

         // Check for escape character.
         int c = buf.sgetc();
         if(c == Escape) return buf.sbumpc(), uflow();
         if(c == EOF) return pbb = Escape;

         // Otherwise, carry on.
         return pbb;
      }

      //
      // underflow
      //
      virtual int underflow()
      {
         if(pbc != (pbb = EOF)) return pbc;
         if((pbc = buf.sbumpc()) != Start) return pbc;

         // Check for escape character.
         int c = buf.sgetc();
         if(c == Escape) return buf.sbumpc(), underflow();
         if(c == EOF) return pbc = Escape;

         // Otherwise, carry on.
         buf.sungetc();
         return pbc;
      }
   };
}

#endif//GDCC__EscapeBuf_H__

