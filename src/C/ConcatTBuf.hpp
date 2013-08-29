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

#ifndef C__ConcatTBuf_H__
#define C__ConcatTBuf_H__

#include "GDCC/TokenBuf.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace C
{
   //
   // ConcatTBuf
   //
   class ConcatTBuf : public GDCC::TokenBuf
   {
   public:
      ConcatTBuf(GDCC::TokenBuf &src_) : src{src_} {}

   protected:
      virtual void underflow();

      GDCC::TokenBuf          &src;
      std::vector<GDCC::Token> buf;
   };
}

#endif//C__ConcatTBuf_H__

