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
      explicit PragmaDTBuf(GDCC::TokenBuf &src_, PragmaFunc pfn_= Pragma) :
         DirectiveTBuf{src_}, pfn{pfn_} {}

   protected:
      virtual bool directive(GDCC::Token const &tok);

      PragmaFunc pfn;
   };

   //
   // PragmaTBuf
   //
   class PragmaTBuf final : public GDCC::TokenBuf
   {
   public:
      using PragmaHandler = void(*)(GDCC::Origin, GDCC::TokenStream &);


      explicit PragmaTBuf(GDCC::TokenBuf &src_, PragmaFunc pfn_ = Pragma) :
         src{src_}, pfn{pfn_}, buf{GDCC::TokenEOF} {sett(buf, buf + 1, buf + 1);}

   protected:
      virtual void underflow();

      GDCC::TokenBuf &src;
      PragmaFunc      pfn;
      GDCC::Token     buf[1];
   };
}

#endif//C__PragmaDTBuf_H__

