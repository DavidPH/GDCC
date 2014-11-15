//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Pragma parsing.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__PragmaDTBuf_H__
#define GDCC__CPP__PragmaDTBuf_H__

#include "../CPP/DirectiveTBuf.hpp"
#include "../CPP/Pragma.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // PragmaDTBuf
      //
      class PragmaDTBuf final : public DirectiveTBuf
      {
      public:
         explicit PragmaDTBuf(Core::TokenBuf &src_, Pragma &prag_) :
            DirectiveTBuf{src_}, prag(prag_) {}

      protected:
         virtual bool directive(Core::Token const &tok);

         Pragma &prag;
      };

      //
      // PragmaPushTBuf
      //
      class PragmaPushTBuf final : public Core::TokenBuf
      {
      public:
         explicit PragmaPushTBuf(Core::TokenBuf &src_, Pragma &prag_) :
            src(src_), prag(prag_) {sett(buf, buf, buf);}

      protected:
         virtual void underflow();

         Core::TokenBuf &src;
         Pragma         &prag;
         Core::Token     buf[1];
      };

      //
      // PragmaTBuf
      //
      class PragmaTBuf final : public Core::TokenBuf
      {
      public:
         explicit PragmaTBuf(Core::TokenBuf &src_, Pragma &prag_) :
            src(src_), prag(prag_) {sett(buf, buf, buf);}

      protected:
         virtual void underflow();

         Core::TokenBuf &src;
         Pragma         &prag;
         Core::Token     buf[1];
      };
   }
}

#endif//GDCC__CPP__PragmaDTBuf_H__

