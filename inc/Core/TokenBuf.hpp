//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source token stream buffers.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__TokenBuf_H__
#define GDCC__Core__TokenBuf_H__

#include "Token.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Core
   {
      //
      // TokenBuf
      //
      class TokenBuf
      {
      public:
         TokenBuf() : tback{nullptr}, tcurr{nullptr}, tfrnt{nullptr} {}
         virtual ~TokenBuf() {}

         //
         // get
         //
         Token const &get()
         {
            if(tcurr == tfrnt && (underflow(), tcurr == tfrnt))
               return TokenEOF;

            return *tcurr++;
         }

         //
         // peek
         //
         Token const &peek()
         {
            if(tcurr == tfrnt && (underflow(), tcurr == tfrnt))
               return TokenEOF;

            return *tcurr;
         }

         //
         // reget
         //
         Token const &reget()
         {
            if(tcurr == tback)
               return TokenEOF;

            return tcurr[-1];
         }

         //
         // unget
         //
         bool unget()
         {
            if(tcurr == tback)
               return false;

            return --tcurr, true;
         }

      protected:
         void bumpt(std::size_t n) {tcurr += n;}

         void sett(Token *tback_, Token *tcurr_, Token *tfrnt_)
            {tback = tback_; tcurr = tcurr_; tfrnt = tfrnt_;}

         Token *tbegin() const {return tback;}
         Token *tend() const {return tfrnt;}
         Token *tptr() const {return tcurr;}

         virtual void underflow() {}

      private:
         Token *tback, *tcurr, *tfrnt;
      };

      //
      // ArrayTBuf
      //
      class ArrayTBuf : public TokenBuf
      {
      public:
         //
         // constructor
         //
         ArrayTBuf(Token const *data_, std::size_t size)
         {
            auto data = const_cast<Token *>(data_);
            sett(data, data, data + size);
         }
      };
   }
}

#endif//GDCC__Core__TokenBuf_H__

