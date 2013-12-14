//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Basic istream token generator.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__StreamTBuf_H__
#define GDCC__Core__StreamTBuf_H__

#include "TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // StreamTBuf
      //
      template<typename Src, std::size_t BufSize = 1>
      class StreamTBuf : public TokenBuf
      {
      public:
         explicit StreamTBuf(Src &src_) : src(src_) {}

      protected:
         //
         // underflow
         //
         virtual void underflow()
         {
            Token *itr = buf;
            if(src) while((src >> *itr++) && itr != buf + BufSize) {}

            sett(buf, buf, itr);
         }

         Token buf[BufSize];
         Src &src;
      };
   }
}

#endif//GDCC__Core__StreamTBuf_H__

