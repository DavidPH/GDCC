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

#include "WrapperBuf.hpp"


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
   template<char Start, typename Src = std::streambuf>
   class CommentBufLine final : public WrapperBuf<Src>
   {
   public:
      using Super = WrapperBuf<Src>;


      explicit CommentBufLine(Src &src_) : Super{src_} {}

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
            while(c != '\n') c = src.sbumpc();
            *this->gptr() = c;
         }
         return c;
      }
   };
}

#endif//GDCC__CommentBuf_H__

