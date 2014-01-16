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
         // bufEnd
         Token *bufEnd() {return buf + BufSize;}

         // bufLive
         // Number of tokens available to be read.
         std::size_t bufLive() {return tend() - tptr();}

         // bufNeeded
         std::size_t bufNeeded() {return bufLive() + BufBack;}

         // bufSpace
         std::size_t bufSpace() {return bufEnd() - tend();}

         //
         // underflow
         //
         virtual void underflow()
         {
            // If room is available, just extend the buffer area.
            if(std::size_t space = bufSpace())
            {
               auto itr = tend();
               for(space = std::min(space, BufRead); space--;)
                  *itr++ = src.get();

               sett(buf, tptr(), itr);
               return;
            }

            // Otherwise, try to push the buffer back.
            std::size_t need = bufNeeded();
            if(need < BufSize)
            {
               // Push back tokens.
               auto itr = buf;
               for(auto tok = tptr() - BufBack, end = itr + need; itr != end;)
                  *itr++ = *tok++;

               sett(buf, itr, itr);

               // Read new tokens.
               for(std::size_t i = std::min(bufSpace(), BufRead); i--;)
                  *itr++ = src.get();

               sett(buf, tptr(), itr);
               return;
            }

            // Otherwise, do nothing.
         }

         TokenBuf &src;
         Token     buf[BufSize];
      };
   }
}

#endif//GDCC__Core__BufferTBuf_H__

