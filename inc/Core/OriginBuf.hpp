//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Origin-tracking streambuf.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__OriginBuf_H__
#define GDCC__Core__OriginBuf_H__

#include "../Core/BufferBuf.hpp"
#include "../Core/Origin.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // OriginBuf
      //
      template<
         std::size_t BufSize = 1,
         std::size_t BufBack = 1,
         std::size_t BufRead = 1,
         typename    CharT   = char,
         typename    Traits  = std::char_traits<CharT>>
      class OriginBuf final :
         public IBufferBuf<BufSize, BufBack, BufRead, CharT, Traits>
      {
      public:
         using Super = IBufferBuf<BufSize, BufBack, BufRead, CharT, Traits>;

         using typename Super::Src;
         using typename Super::int_type;


         OriginBuf(Src &src_, Origin pos_) : Super{src_}, pos{pos_} {}

         Origin getOrigin() const {return pos;}

      protected:
         //
         // underflow
         //
         virtual int_type underflow()
         {
            auto c = Super::underflow();

            if(c == '\n' && pos.line)
            {
               ++pos.line;
               if(pos.col) pos.col = 1;
            }
            else if(pos.col)
               ++pos.col;

            return c;
         }

      private:
         Origin pos;
      };
   }
}

#endif//GDCC__Core__OriginBuf_H__

