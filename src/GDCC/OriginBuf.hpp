//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Origin-tracking streambuf.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__OriginBuf_H__
#define GDCC__OriginBuf_H__

#include "Origin.hpp"
#include "WrapperBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   //
   // OriginBuf
   //
   template<typename Buf = std::streambuf>
   class OriginBuf final : public WrapperBuf<Buf>
   {
   public:
      OriginBuf(Buf &buf_, String file, std::size_t line = 1) :
         WrapperBuf<Buf>{buf_}, pos{file, line} {}

      Origin getOrigin() const {return pos;}

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
         if((pbb = buf.sbumpc()) == '\n') ++pos.line;

         return pbb;
      }

      //
      // underflow
      //
      virtual int underflow()
      {
         if(pbc != (pbb = EOF)) return pbc;
         if((pbc = buf.sbumpc()) == '\n') ++pos.line;

         return pbc;
      }

   private:
      Origin pos;
   };
}

#endif//GDCC__OriginBuf_H__

