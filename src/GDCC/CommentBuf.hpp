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
      explicit CommentBufLine(Buf &buf_) : buf{buf_}, pbc{EOF} {}

   protected:
      virtual int pbackfail(int c) {return pbc == EOF ? pbc = c : EOF;}

      //
      // uflow
      //
      virtual int uflow()
      {
         int c;

         if((c = pbc) != EOF) return pbc = EOF, c;
         if((c = buf.sbumpc()) != Start) return c;

         do {if((c = buf.sbumpc()) == EOF) return EOF;} while(c != '\n');
         return uflow();
      }

      //
      // underflow
      //
      virtual int underflow()
      {
         if(pbc != EOF) return pbc;
         if((pbc = buf.sbumpc()) != Start) return pbc;

         do {if((pbc = buf.sbumpc()) == EOF) return EOF;} while(pbc != '\n');
         return underflow();
      }

   private:
      Buf &buf;
      int  pbc;
   };
}

#endif//GDCC__CommentBuf_H__

