//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Preprocessor directive token buffers.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__DirectiveTBuf_H__
#define GDCC__CPP__DirectiveTBuf_H__

#include "../Core/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CPP
   {
      class IncStream;

      //
      // DirectiveTBuf
      //
      // Base class for handling preprocessor directives. Scans for lines starting
      // with # and calls directive for them.
      //
      class DirectiveTBuf : public Core::TokenBuf
      {
      public:
         explicit DirectiveTBuf(Core::TokenBuf &src_) : src(src_),
            buf{Core::TokenEOF}, endl{true} {sett(buf, buf + 1, buf + 1);}

      protected:
         //
         // directive
         //
         // Gets called with the first non-whitespace token after the starting #.
         // The token is left in src and must be stored if src is read. Returns
         // true if the directive is handled, false otherwise. Must leave the
         // terminating LnEnd token to be read by caller.
         //
         virtual bool directive(Core::Token const &tok) = 0;

         virtual void underflow();

         Core::TokenBuf &src;
         Core::Token     buf[2];

         bool endl : 1;
      };

      //
      // ErrorDTBuf
      //
      class ErrorDTBuf final : public DirectiveTBuf
      {
      public:
         explicit ErrorDTBuf(Core::TokenBuf &src_) : DirectiveTBuf{src_} {}

      protected:
         virtual bool directive(Core::Token const &tok);
      };
   }
}

#endif//GDCC__CPP__DirectiveTBuf_H__

