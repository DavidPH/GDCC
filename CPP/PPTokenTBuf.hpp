//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Preprocessor token converter token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__PPTokenTBuf_H__
#define GDCC__CPP__PPTokenTBuf_H__

#include "../CPP/Types.hpp"

#include "../Core/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CPP
{
   //
   // PPTokenTBuf
   //
   class PPTokenTBuf : public Core::TokenBuf
   {
   public:
      PPTokenTBuf(Core::TokenBuf &src_) : src(src_) {}


      static bool IsKeyWrd(Core::String str);
      static bool IsNumInt(Core::String str);
      static bool IsNumFix(Core::String str);
      static bool IsNumFlt(Core::String str);

      static char const *SkipBin(char const *itr, char const *end);
      static char const *SkipDec(char const *itr, char const *end);
      static char const *SkipFlt(char const *itr, char const *end);
      static char const *SkipHex(char const *itr, char const *end);
      static char const *SkipOct(char const *itr, char const *end);

   protected:
      virtual void underflow();

      Core::TokenBuf &src;
      Core::Token     buf[1];
   };
}

#endif//GDCC__CPP__PPTokenTBuf_H__

