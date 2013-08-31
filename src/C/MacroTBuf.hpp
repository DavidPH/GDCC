//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Macro expanding token buffer.
//
//-----------------------------------------------------------------------------

#ifndef C__MacroTBuf_H__
#define C__MacroTBuf_H__

#include "GDCC/TokenBuf.hpp"
#include "GDCC/Utility.hpp"

#include <list>
#include <unordered_set>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace C
{
   class Macro;

   //
   // MacroTBuf
   //
   class MacroTBuf : public GDCC::TokenBuf
   {
   public:
      using Itr = std::list<GDCC::Token>::iterator;
      using Rng = GDCC::Range<Itr>;



      explicit MacroTBuf(GDCC::TokenBuf &src_) : src{src_}, ignoreAll{false} {}

   protected:
      void applyMarker(GDCC::String str);

      bool expand(Itr itr);
      void expand(Itr end, Macro const &macro, Rng const &argRng);

      GDCC::String stringize(Rng const &arg);

      virtual void underflow();

      GDCC::TokenBuf &src;

      std::list<GDCC::Token> buf;

      std::unordered_set<GDCC::String> ignore;

      bool ignoreAll : 1;
   };
}

#endif//C__MacroTBuf_H__

