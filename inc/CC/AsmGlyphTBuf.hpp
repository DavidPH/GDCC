//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C scoped glyph token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__LabelTBuf_H__
#define GDCC__CC__LabelTBuf_H__

#include "../CC/Types.hpp"

#include "../Core/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // AsmGlyphTBuf
   //
   class AsmGlyphTBuf : public Core::TokenBuf
   {
   public:
      AsmGlyphTBuf(Core::TokenBuf &src_, Scope &scope_) :
         src(src_), scope(scope_) {sett(buf, buf, buf);}

   protected:
      virtual void underflow();

      Core::TokenBuf &src;
      Core::Token     buf[1];
      Scope          &scope;
   };
}

#endif//GDCC__CC__LabelTBuf_H__

