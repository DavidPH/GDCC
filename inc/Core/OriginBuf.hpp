//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
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

#include "Origin.hpp"
#include "WrapperBuf.hpp"


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
      template<typename Src = std::streambuf>
      class OriginBuf final : public WrapperBuf<Src>
      {
      public:
         using Super = WrapperBuf<Src>;


         OriginBuf(Src &src_, String file, std::size_t line = 1) :
            Super{src_}, pos{file, line} {}

         Origin getOrigin() const {return pos;}

      protected:
         //
         // underflow
         //
         virtual int underflow()
         {
            int c = Super::underflow();
            if(c == '\n') ++pos.line;
            return c;
         }

      private:
         Origin pos;
      };
   }
}

#endif//GDCC__Core__OriginBuf_H__

