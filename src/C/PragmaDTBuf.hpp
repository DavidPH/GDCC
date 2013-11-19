//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Pragma parsing.
//
//-----------------------------------------------------------------------------

#ifndef C__PragmaDTBuf_H__
#define C__PragmaDTBuf_H__

#include "DirectiveTBuf.hpp"
#include "Pragma.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace C
{
   //
   // PragmaDTBuf
   //
   class PragmaDTBuf final : public DirectiveTBuf
   {
   public:
      explicit PragmaDTBuf(GDCC::TokenBuf &src_, Pragma &prag_) :
         DirectiveTBuf{src_}, prag{prag_} {}

   protected:
      virtual bool directive(GDCC::Token const &tok);

      Pragma &prag;
   };

   //
   // PragmaTBuf
   //
   class PragmaTBuf final : public GDCC::TokenBuf
   {
   public:
      explicit PragmaTBuf(GDCC::TokenBuf &src_, Pragma &prag_) :
         src{src_}, prag{prag_}, buf{GDCC::TokenEOF} {sett(buf, buf + 1, buf + 1);}

   protected:
      virtual void underflow();

      GDCC::TokenBuf &src;
      Pragma         &prag;
      GDCC::Token     buf[1];
   };
}

#endif//C__PragmaDTBuf_H__

