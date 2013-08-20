//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source token stream buffers.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__TokenBuf_H__
#define GDCC__TokenBuf_H__

#include "Token.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
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
      // unget
      //
      bool unget()
      {
         if(tcurr == tback)
            return false;

         return --tcurr, true;
      }

   protected:
      void sett(Token *tback_, Token *tcurr_, Token *tfrnt_)
         {tback = tback_; tcurr = tcurr_; tfrnt = tfrnt_;}

      Token *tbegin() const {return tback;}
      Token *tend() const {return tfrnt;}
      Token *tptr() const {return tcurr;}

      virtual void underflow() {}

   private:
      Token *tback, *tcurr, *tfrnt;
   };
}

#endif//GDCC__TokenBuf_H__

