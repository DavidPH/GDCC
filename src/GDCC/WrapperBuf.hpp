//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Streambuf-wrapping streambuf.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__WrapperBuf_H__
#define GDCC__WrapperBuf_H__

#include <streambuf>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   //
   // WrapperBuf
   //
   template<typename Buf = std::streambuf>
   class WrapperBuf : public std::streambuf
   {
   public:
      explicit WrapperBuf(Buf &buf_) : buf(buf_), pbc{EOF}, pbb{EOF} {}

   protected:
      //
      // pbackfail
      //
      virtual int pbackfail(int c)
      {
         pbc = c == EOF ? pbb : c;
         pbb = EOF;
         return pbc;
      }

      //
      // uflow
      //
      virtual int uflow()
      {
         if(pbc != EOF) return pbb = pbc, pbc = EOF, pbb;
         return pbb = buf.sbumpc();
      }

      //
      // underflow
      //
      virtual int underflow()
      {
         if(pbc != (pbb = EOF)) return pbc;
         return pbc = buf.sbumpc();
      }

      Buf &buf;
      int  pbc; // putback char
      int  pbb; // putback buffer
   };
}

#endif//GDCC__WrapperBuf_H__

