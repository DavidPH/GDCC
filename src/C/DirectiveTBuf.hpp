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

#ifndef C__DirectiveTBuf_H__
#define C__DirectiveTBuf_H__

#include "GDCC/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace C
{
   class IncStream;

   //
   // DirectiveTBuf
   //
   // Base class for handling preprocessor directives. Scans for lines starting
   // with # and calls directive for them.
   //
   class DirectiveTBuf : public GDCC::TokenBuf
   {
   public:
      explicit DirectiveTBuf(GDCC::TokenBuf &src_) : src{src_},
         buf{GDCC::TokenEOF}, endl{true} {sett(buf, buf + 1, buf + 1);}

   protected:
      //
      // directive
      //
      // Gets called with the first non-whitespace token after the starting #.
      // The token is left in src and must be stored if src is read. Returns
      // true if the directive is handled, false otherwise. Must leave the
      // terminating LnEnd token to be read by caller.
      //
      virtual bool directive(GDCC::Token const &tok) = 0;

      virtual void underflow();

      GDCC::TokenBuf &src;
      GDCC::Token     buf[2];

      bool endl : 1;
   };

   //
   // ErrorDTBuf
   //
   class ErrorDTBuf final : public DirectiveTBuf
   {
   public:
      explicit ErrorDTBuf(GDCC::TokenBuf &src_) : DirectiveTBuf{src_} {}

   protected:
      virtual bool directive(GDCC::Token const &tok);
   };
}

#endif//C__DirectiveTBuf_H__

