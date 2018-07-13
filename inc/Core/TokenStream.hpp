//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source token streams.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__TokenStream_H__
#define GDCC__Core__TokenStream_H__

#include "../Core/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // TokenStream
   //
   class TokenStream
   {
   public:
      TokenStream() : buf{nullptr} {}
      explicit TokenStream(TokenBuf *buf_) : buf{buf_}, eof{false}, err{false} {}

      virtual ~TokenStream() {}

      explicit operator bool () const {return buf && !eof && !err;}

      TokenStream &operator >> (Token &out) {out = get(); return *this;}

      //
      // drop
      //
      bool drop(TokenType tt)
      {
         auto const &tok = buf->peek();
         if(tok.tok == TOK_EOF)
            return eof = true, tt == TOK_EOF;

         if(tok.tok == tt)
            return buf->get(), true;

         return false;
      }

      //
      // drop
      //
      bool drop(TokenType tt, String ts)
      {
         auto const &tok = buf->peek();
         if(tok.tok == TOK_EOF)
            return eof = true, false;

         if(tok.tok == tt && tok.str == ts)
            return buf->get(), true;

         return false;
      }

      //
      // drop
      //
      bool drop(TokenType tt1, String ts1, TokenType tt2)
      {
         if(drop(tt1, ts1))
         {
            if(drop(tt2))
               return true;

            unget();
            return false;
         }
         else
            return false;
      }

      //
      // drop
      //
      bool drop(TokenType tt1, TokenType tt2, String ts2)
      {
         return drop(tt1) ? drop(tt2, ts2) ? true : (unget(), false) : false;
      }

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
      // peek
      //
      bool peek(TokenType tt)
      {
         auto const &tok = buf->peek();
         if(tok.tok == TOK_EOF)
            return eof = true, tt == TOK_EOF;

         return tok.tok == tt;
      }

      //
      // peek
      //
      bool peek(TokenType tt, String ts)
      {
         auto const &tok = buf->peek();
         if(tok.tok == TOK_EOF)
            return eof = true, false;

         return tok.tok == tt && tok.str == ts;
      }

      //
      // peek
      //
      // Performs a two-token peek.
      //
      bool peek(TokenType tt1, String ts1, TokenType tt2)
      {
         if(drop(tt1, ts1))
         {
            bool ret = peek(tt2);
            unget();
            return ret;
         }
         else
            return false;
      }

      //
      // peek
      //
      // Performs a two-token peek.
      //
      bool peek(TokenType tt1, TokenType tt2)
      {
         if(drop(tt1))
         {
            bool ret = peek(tt2);
            unget();
            return ret;
         }
         else
            return false;
      }

      //
      // reget
      //
      Token const &reget()
      {
         auto const &tok = buf->reget();
         if(tok.tok == TOK_EOF)
            err = true;

         return tok;
      }

      TokenBuf *tkbuf() const {return buf;}

      //
      // tkbuf
      //
      TokenBuf *tkbuf(TokenBuf *buf_)
      {
         auto oldbuf = buf;
         buf = buf_;
         eof = false;
         err = false;
         return oldbuf;
      }

      //
      // unget
      //
      void unget()
      {
         if(eof || err) return;

         if(!buf->unget())
            err = true;
      }

   private:
      TokenBuf *buf;

      bool eof : 1;
      bool err : 1;
   };

   //
   // ArrayTStream
   //
   class ArrayTStream : public TokenStream
   {
   public:
      //
      // constructor
      //
      ArrayTStream(Token const *data, std::size_t size) :
         TokenStream{&buf}, buf{data, size}
      {
      }

   private:
      ArrayTBuf buf;
   };
}

#endif//GDCC__Core__TokenStream_H__

