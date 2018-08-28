//-----------------------------------------------------------------------------
//
// Copyright (C) 2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Token sources.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__TokenSource_H__
#define GDCC__Core__TokenSource_H__

#include "../Core/Token.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // TokenSource
   //
   class TokenSource
   {
   public:
      virtual ~TokenSource() {}

      bool disableToken(TokenType type) {return v_disableToken(type);}

      bool enableToken(TokenType type) {return v_enableToken(type);}

      Token getToken() {return v_getToken();}

   protected:
      // Enables a specified token type. Returns true if token was enabled
      // before the call, false otherwise.
      virtual bool v_disableToken(TokenType) {return false;}

      // Disables a specified token type. Returns false if token was disabled
      // before the call, true otherwise.
      virtual bool v_enableToken(TokenType) {return true;}

      // Returns the next token.
      virtual Token v_getToken() = 0;
   };

   //
   // ArrayTSource
   //
   class ArrayTSource : public TokenSource
   {
   public:
      ArrayTSource(Token const *buf, std::size_t len) : itr{buf}, end{buf + len} {}

   protected:
      virtual Token v_getToken() {return itr != end ? *itr++ : TokenEOF;}

      Token const *itr, *end;
   };
}

#endif//GDCC__Core__TokenSource_H__

