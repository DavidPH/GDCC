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

#ifndef GDCC__StreamTBuf_H__
#define GDCC__StreamTBuf_H__

#include "TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   //
   // StreamTBuf
   //
   template<typename IS, std::size_t BufSize = 1>
   class StreamTBuf : public TokenBuf
   {
   public:
      explicit StreamTBuf(IS &in_) : in(in_) {}

   protected:
      //
      // v_get
      //
      virtual Token v_get()
      {
         if(!in) return TokenBuf::v_get();

         Token *itr = buf;
         while((in >> *itr++) && itr != buf + BufSize) {}

         sett(buf, buf+1, itr);

         return *buf;
      }

      Token buf[BufSize];
      IS &in;
   };
}

#endif//GDCC__StreamTBuf_H__

