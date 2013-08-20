//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source token streams.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__TokenStream_H__
#define GDCC__TokenStream_H__

#include "TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   //
   // TokenStream
   //
   class TokenStream
   {
   public:
      TokenStream() : buf{nullptr} {}
      explicit TokenStream(TokenBuf *buf_) : buf{buf_}, eof{false}, err{false} {}

      explicit operator bool () const {return buf && !eof && !err;}

      TokenStream &operator >> (Token &out) {out = get(); return *this;}

      //
      // get
      //
      Token const &get()
      {
         auto const &tok = buf->get();
         if(tok.tok == TOK_EOF)
            eof = true;

         return tok;
      }

      //
      // peek
      //
      Token const &peek()
      {
         auto const &tok = buf->peek();
         if(tok.tok == TOK_EOF)
            eof = true;

         return tok;
      }

      //
      // unget
      //
      void unget()
      {
         if(!buf->unget())
            err = true;
      }

   private:
      TokenBuf *buf;

      bool eof : 1;
      bool err : 1;
   };
}

#endif//GDCC__TokenStream_H__

