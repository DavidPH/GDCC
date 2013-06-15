//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Comment-stripping streambufs.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CommentBuf_H__
#define GDCC__CommentBuf_H__

#include <streambuf>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   //
   // CommentBufLine
   //
   // Handles line-oriented comments started by a single character.
   //
   template<char Start, typename Buf = std::streambuf>
   class CommentBufLine final : public Buf
   {
   public:
      explicit CommentBufLine(Buf &buf_) : buf{buf_}, pbc{EOF}, pbb{EOF} {}

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
         if((pbb = buf.sbumpc()) != Start) return pbb;

         do {pbb = buf.sbumpc();} while(pbb != EOF && pbb != '\n');
         return pbb;
      }

      //
      // underflow
      //
      virtual int underflow()
      {
         pbb = EOF;

         if(pbc != EOF) return pbc;
         if((pbc = buf.sbumpc()) != Start) return pbc;

         do {pbc = buf.sbumpc();} while(pbc != EOF && pbc != '\n');
         return pbc;
      }

   private:
      Buf &buf;
      int  pbc; // putback char
      int  pbb; // putback buffer
   };
}

#endif//GDCC__CommentBuf_H__

