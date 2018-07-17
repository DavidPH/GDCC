//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Trigraph converting streambufs.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__Trigraph_H__
#define GDCC__CPP__Trigraph_H__

#include "../CPP/Types.hpp"

#include "../Core/BufferBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CPP
{
   //
   // TrigraphBuf
   //
   template<
      std::size_t BufSize = 1,
      std::size_t BufBack = 1,
      std::size_t BufRead = 1,
      typename    CharT   = char,
      typename    Traits  = std::char_traits<CharT>>
   class TrigraphBuf final :
      public Core::IBufferBuf<BufSize, BufBack, BufRead, CharT, Traits>
   {
   public:
      using Super = Core::IBufferBuf<BufSize, BufBack, BufRead, CharT, Traits>;

      using Src = typename Super::Src;
      using typename Super::int_type;


      explicit TrigraphBuf(Src &src_) : Super{src_} {}

   protected:
      using Super::gptr;
      using Super::egptr;

      using Super::src;

      //
      // underflow
      //
      virtual int_type underflow()
      {
         if(Super::underflow() == '?' && src.sgetc() == '?')
            switch(src.sbumpc(), src.sgetc())
         {
         case '=':  src.sbumpc(); return *gptr() = '#';
         case '(':  src.sbumpc(); return *gptr() = '[';
         case '/':  src.sbumpc(); return *gptr() = '\\';
         case ')':  src.sbumpc(); return *gptr() = ']';
         case '\'': src.sbumpc(); return *gptr() = '^';
         case '<':  src.sbumpc(); return *gptr() = '{';
         case '!':  src.sbumpc(); return *gptr() = '|';
         case '>':  src.sbumpc(); return *gptr() = '}';
         case '-':  src.sbumpc(); return *gptr() = '~';

         default: src.sungetc(); return *gptr();
         }

         return gptr() == egptr() ? EOF : *gptr();
      }
   };
}

#endif//GDCC__CPP__Trigraph_H__

