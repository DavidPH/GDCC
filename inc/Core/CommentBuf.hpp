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

#ifndef GDCC__Core__CommentBuf_H__
#define GDCC__Core__CommentBuf_H__

#include "WrapperBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // CommentBufC
      //
      // Handles C-style /**/ and C++-style // comments.
      //
      template<typename Src = std::streambuf>
      class CommentBufC final : public WrapperBuf<Src>
      {
      public:
         using Super = WrapperBuf<Src>;


         explicit CommentBufC(Src &src_) : Super{src_} {}

      protected:
         using Super::src;

         //
         // underflow
         //
         virtual int underflow()
         {
            if(Super::underflow() == '/')
            {
               if(src.sgetc() == '*')
               {
                  for(src.sbumpc(); !(src.sbumpc() == '*' && src.sbumpc() == '/');)
                     if(src.sgetc() == EOF) return this->gbump(1), EOF;
                  return src.sbumpc(), *this->gptr() = ' ';
               }
               else if(src.sgetc() == '/')
               {
                  do if(src.sbumpc() == EOF) return this->gbump(1), EOF;
                  while(src.sgetc() != '\n');
                  return *this->gptr() = ' ';
               }
            }

            return this->gptr() == this->egptr() ? EOF : *this->gptr();
         }
      };

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
}

#endif//GDCC__Core__CommentBuf_H__

