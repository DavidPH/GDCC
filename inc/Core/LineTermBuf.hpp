//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Line-terminator normalizing streambuf.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__LineTermBuf_H__
#define GDCC__Core__LineTermBuf_H__

#include "../Core/BufferBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // LineTermBuf
      //
      template<
         std::size_t BufSize = 1,
         std::size_t BufBack = 1,
         std::size_t BufRead = 1,
         typename    CharT   = char,
         typename    Traits  = std::char_traits<CharT>>
      class LineTermBuf final :
         public IBufferBuf<BufSize, BufBack, BufRead, CharT, Traits>
      {
      public:
         using Super = IBufferBuf<BufSize, BufBack, BufRead, CharT, Traits>;

         using typename Super::Src;
         using typename Super::int_type;


         explicit LineTermBuf(Src &src_) : Super{src_} {}

      protected:
         using Super::src;

         using Super::gbump;
         using Super::gptr;

         //
         // underflow
         //
         virtual int_type underflow()
         {
            auto c = Super::underflow();

            if(c == '\r')
            {
               // Convert \r\n to \n.
               if(src.sgetc() == '\n')
                  return *gptr() = src.sbumpc();

               // Otherwise, convert loose \r to \n.
               return *gptr() = '\n';
            }

            return c;
         }
      };
   }
}

#endif//GDCC__Core__LineTermBuf_H__

