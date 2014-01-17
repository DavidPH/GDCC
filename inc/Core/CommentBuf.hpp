//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

#include "../Core/BufferBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // CCommentBuf
      //
      // Handles C-style /**/ and C++-style // comments.
      //
      template<
         std::size_t BufSize = 1,
         std::size_t BufBack = 1,
         std::size_t BufRead = 1,
         typename    CharT   = char,
         typename    Traits  = std::char_traits<CharT>>
      class CCommentBuf final :
         public IBufferBuf<BufSize, BufBack, BufRead, CharT, Traits>
      {
      public:
         using Super = IBufferBuf<BufSize, BufBack, BufRead, CharT, Traits>;

         using typename Super::Src;
         using typename Super::int_type;


         explicit CCommentBuf(Src &src_) : Super{src_} {}

      protected:
         using Super::egptr;
         using Super::gbump;
         using Super::gptr;

         using Super::src;

         //
         // underflow
         //
         virtual int_type underflow()
         {
            if(Super::underflow() == '/')
            {
               // C-style /**/ comment.
               if(src.sgetc() == '*')
               {
                  for(src.sbumpc(); !(src.sbumpc() == '*' && src.sbumpc() == '/');)
                     if(src.sgetc() == Traits::eof()) return gbump(1), Traits::eof();
                  return src.sbumpc(), *gptr() = ' ';
               }

               // C++-style // comment.
               else if(src.sgetc() == '/')
               {
                  do if(src.sbumpc() == Traits::eof()) return gbump(1), Traits::eof();
                  while(src.sgetc() != '\n');
                  return *gptr() = ' ';
               }

               return *gptr();
            }

            return gptr() == egptr() ? Traits::eof() : *gptr();
         }
      };

      //
      // LineCommentBuf
      //
      // Handles line-oriented comments started by a single character.
      //
      template<
         std::size_t BufSize,
         std::size_t BufBack,
         std::size_t BufRead,
         typename    CharT,
         CharT       Begin,
         CharT       End    = '\n',
         typename    Traits = std::char_traits<CharT>>
      class LineCommentBuf final :
         public IBufferBuf<BufSize, BufBack, BufRead, CharT, Traits>
      {
      public:
         using Super = IBufferBuf<BufSize, BufBack, BufRead, CharT, Traits>;

         using typename Super::Src;
         using typename Super::int_type;


         explicit LineCommentBuf(Src &src_) : Super{src_} {}

      protected:
         using Super::gptr;

         using Super::src;

         //
         // underflow
         //
         virtual int_type underflow()
         {
            auto c = Super::underflow();
            if(c == Begin)
            {
               do c = src.sbumpc(); while(c != End);
               *gptr() = c;
            }
            return c;
         }
      };
   }
}

#endif//GDCC__Core__CommentBuf_H__

