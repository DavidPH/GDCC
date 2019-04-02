//-----------------------------------------------------------------------------
//
// Copyright (C) 2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// TokenSource-based token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__SourceTBuf_H__
#define GDCC__Core__SourceTBuf_H__

#include "../Core/TokenBuf.hpp"
#include "../Core/TokenSource.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // SourceTBuf
   //
   template<std::size_t BufSize = 1>
   class SourceTBuf : public TokenBuf
   {
   public:
      explicit SourceTBuf(TokenSource &src_) : src(src_) {}

   protected:
      //
      // underflow
      //
      virtual void underflow()
      {
         Token *itr = buf;
         while(itr != buf + BufSize)
         {
            Token tok = src.getToken();
            if(tok.tok == TOK_EOF) break;
            *itr++ = tok;
         }

         sett(buf, buf, itr);
      }

      Token        buf[BufSize];
      TokenSource &src;
   };
}

#endif//GDCC__Core__SourceTBuf_H__

