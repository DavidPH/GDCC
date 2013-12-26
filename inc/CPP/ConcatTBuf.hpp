//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// String concatenating token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__ConcatTBuf_H__
#define GDCC__CPP__ConcatTBuf_H__

#include "../Core/TokenBuf.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // ConcatTBuf
      //
      class ConcatTBuf : public Core::TokenBuf
      {
      public:
         ConcatTBuf(Core::TokenBuf &src_) : src(src_) {}

      protected:
         virtual void underflow();

         Core::TokenBuf          &src;
         std::vector<Core::Token> buf;
      };
   }
}

#endif//GDCC__CPP__ConcatTBuf_H__

