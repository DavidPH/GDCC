//-----------------------------------------------------------------------------
//
// Copyright (C) 2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C tokenizing.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__TSource_H__
#define GDCC__CPP__TSource_H__

#include "../CPP/Types.hpp"

#include "../Core/TokenSource.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CPP
{
   //
   // TSource
   //
   class TSource : public Core::TokenSource
   {
   public:
      TSource(std::istream &in_, Core::OriginSource &orig_) :
         in{in_}, orig{orig_}, tokHeader{false} {}


      static Core::Token &SkipCommentB(std::istream &in, Core::Token &out);
      static Core::Token &SkipCommentL(std::istream &in, Core::Token &out);
      static bool TryHeader(std::istream &in, Core::Token &out);

   protected:
      virtual bool v_disableToken(Core::TokenType type);

      virtual bool v_enableToken(Core::TokenType type);

      virtual Core::Token v_getToken();

      std::istream       &in;
      Core::OriginSource &orig;

      bool tokHeader : 1;
   };
}

#endif//GDCC__CPP__TSource_H__

