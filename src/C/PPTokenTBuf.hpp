//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Preprocessor token converter token buffer.
//
//-----------------------------------------------------------------------------

#ifndef C__PPTokenTBuf_H__
#define C__PPTokenTBuf_H__

#include "GDCC/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace C
{
   //
   // PPTokenTBuf
   //
   class PPTokenTBuf : public GDCC::TokenBuf
   {
   public:
      PPTokenTBuf(GDCC::TokenBuf &src_) : src{src_} {}


      static bool IsKeyWrd(GDCC::String str);
      static bool IsNumInt(GDCC::String str);
      static bool IsNumFix(GDCC::String str);
      static bool IsNumFlt(GDCC::String str);

   protected:
      virtual void underflow();

      GDCC::TokenBuf &src;
      GDCC::Token     buf[1];


      static char const *SkipDec(char const *itr, char const *end);
      static char const *SkipFlt(char const *itr, char const *end);
      static char const *SkipHex(char const *itr, char const *end);
      static char const *SkipOct(char const *itr, char const *end);
   };
}

#endif//C__PPTokenTBuf_H__

