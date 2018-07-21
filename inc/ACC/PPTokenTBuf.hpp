//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Preprocessor token converter token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__PPTokenTBuf_H__
#define GDCC__ACC__PPTokenTBuf_H__

#include "../ACC/Types.hpp"

#include "../Core/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::ACC
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

   protected:
      virtual void underflow();

      Core::TokenBuf &src;
      Core::Token     buf[1];
   };
}

#endif//GDCC__ACC__PPTokenTBuf_H__

