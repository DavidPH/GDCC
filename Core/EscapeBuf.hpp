//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Escape handling streambufs.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__EscapeBuf_H__
#define GDCC__Core__EscapeBuf_H__

#include "../Core/BufferBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // StripEscapeBuf
   //
   template<
      std::size_t BufSize,
      std::size_t BufBack,
      std::size_t BufRead,
      typename    CharT,
      CharT       Escape,
      CharT       Start  = '\\',
      typename    Traits = std::char_traits<CharT>>
   class StripEscapeBuf final :
      public IBufferBuf<BufSize, BufBack, BufRead, CharT, Traits>
   {
   public:
      using Super = IBufferBuf<BufSize, BufBack, BufRead, CharT, Traits>;

      using Src = typename Super::Src;
      using typename Super::int_type;


      explicit StripEscapeBuf(Src &src_) : Super{src_} {}

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

         if(c == Start)
         {
            if(src.sgetc() == Escape)
               return gbump(1), src.sbumpc(), underflow();

            // If EOF is hit, pretend there was an EOL instead.
            if(src.sgetc() == Traits::eof())
               return *gptr() = '\n';
         }

         return c;
      }
   };
}

#endif//GDCC__Core__EscapeBuf_H__

