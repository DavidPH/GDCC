//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Buffering token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__BufferTBuf_H__
#define GDCC__Core__BufferTBuf_H__

#include "../Core/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // BufferTBuf
      //
      // A statically sized buffer that maintains a minimum number of past
      // tokens. Because reading ahead may have side effects, the default max
      // read ahead is 1.
      //
      template<std::size_t BufSize = 1, std::size_t BufBack = 1,
         std::size_t BufRead = 1>
      class BufferTBuf : public TokenBuf
      {
      public:
         BufferTBuf(TokenBuf &src_) : src(src_) {sett(buf, buf, buf);}

      protected:
         // bufAvail
         std::size_t bufAvail() {return tend() - tptr();}

         // bufEnd
         Token *bufEnd() {return buf + BufSize;}

         // bufLive
         std::size_t bufLive() {return bufAvail() + BufBack;}

         // bufSpace
         std::size_t bufSpace() {return bufEnd() - tend();}

         //
         // underflow
         //
         virtual void underflow()
         {
            std::size_t space;
            Token      *itr;

            // If no buffer space left try to shift to fit more tokens.
            if(!bufSpace() && (space = bufLive()) < BufSize)
            {
               itr = buf;
               for(Token *tok = tptr() - BufBack, *end = itr + space; itr != end;)
                  *itr++ = *tok++;

               sett(buf, itr, itr);
            }
            else
               itr = tend();

            // Read new tokens.
            space = std::min(bufSpace(), BufRead);
            while(space-- && src.peek().tok != TOK_EOF)
               *itr++ = src.get();

            sett(buf, tptr(), itr);
         }

         TokenBuf &src;
         Token     buf[BufSize];
      };
   }
}

#endif//GDCC__Core__BufferTBuf_H__

