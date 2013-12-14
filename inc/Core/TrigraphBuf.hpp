//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Trigraph converting streambufs.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__Trigraph_H__
#define GDCC__Core__Trigraph_H__

#include "WrapperBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // TrigraphBuf
      //
      template<typename Src = std::streambuf>
      class TrigraphBuf final : public WrapperBuf<Src>
      {
      public:
         using Super = WrapperBuf<Src>;


         explicit TrigraphBuf(Src &src_) : Super{src_} {}

      protected:
         using Super::src;

         //
         // underflow
         //
         virtual int underflow()
         {
            if(Super::underflow() == '?' && src.sgetc() == '?')
               switch(src.sbumpc(), src.sgetc())
            {
            case '=':  src.sbumpc(); return *this->gptr() = '#';
            case '(':  src.sbumpc(); return *this->gptr() = '[';
            case '/':  src.sbumpc(); return *this->gptr() = '\\';
            case ')':  src.sbumpc(); return *this->gptr() = ']';
            case '\'': src.sbumpc(); return *this->gptr() = '^';
            case '<':  src.sbumpc(); return *this->gptr() = '{';
            case '!':  src.sbumpc(); return *this->gptr() = '|';
            case '>':  src.sbumpc(); return *this->gptr() = '}';
            case '-':  src.sbumpc(); return *this->gptr() = '~';

            default: src.sungetc(); return *this->gptr();
            }

            return this->gptr() == this->egptr() ? EOF : *this->gptr();
         }
      };
   }
}

#endif//GDCC__Core__Trigraph_H__

