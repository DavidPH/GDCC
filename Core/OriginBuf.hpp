//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

namespace GDCC::Core
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
      public IBufferBuf<BufSize, BufBack, BufRead, CharT, Traits>, public OriginSource
   {
   public:
      using Super = IBufferBuf<BufSize, BufBack, BufRead, CharT, Traits>;

      using Src = typename Super::Src;
      using typename Super::int_type;


      OriginBuf(Src &src_, Origin pos_) : Super{src_}, pos{pos_} {}

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
         else if((c & 0xC0) != 0x80 && pos.col)
            ++pos.col;

         return c;
      }

      virtual Origin v_getOrigin() const {return pos;}

      Origin pos;
   };
}

#endif//GDCC__Core__OriginBuf_H__

