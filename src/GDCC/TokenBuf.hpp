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
      Token get()
      {
         if(tcurr == tfrnt)
            return v_get();

         return *tcurr++;
      }

      //
      // unget
      //
      bool unget(Token const &tok)
      {
         if(tcurr == tback || *(tcurr - 1) != tok)
            return v_unget(tok);

         return --tcurr, true;
      }

   protected:
      void sett(Token *tback_, Token *tcurr_, Token *tfrnt_)
         {tback = tback_; tcurr = tcurr_; tfrnt = tfrnt_;}

      Token *tbegin() const {return tback;}
      Token *tend() const {return tfrnt;}
      Token *tptr() const {return tcurr;}

      virtual Token v_get() {return Token(Origin(STRNULL, 0), STRNULL, TOK_EOF);}

      virtual bool v_unget(Token const &/*tok*/) {return false;}

   private:
      Token *tback, *tcurr, *tfrnt;
   };
}

#endif//GDCC__TokenBuf_H__

