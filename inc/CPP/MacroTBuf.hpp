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

#ifndef GDCC__CPP__MacroTBuf_H__
#define GDCC__CPP__MacroTBuf_H__

#include "../Core/TokenBuf.hpp"
#include "../Core/Utility.hpp"

#include <list>
#include <unordered_set>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CPP
   {
      class Macro;
      class MacroMap;

      //
      // MacroTBuf
      //
      class MacroTBuf : public Core::TokenBuf
      {
      public:
         using Itr = std::list<Core::Token>::iterator;
         using Rng = Core::Range<Itr>;


         MacroTBuf(Core::TokenBuf &src_, MacroMap &macros_) :
            src(src_), macros(macros_), ignoreAll{false} {}

      protected:
         void applyMarker(Core::String str);

         bool expand(Itr itr);
         void expand(Itr end, Macro const &macro, Rng const &argRng);

         Core::String stringize(Rng const &arg);

         virtual void underflow();

         std::list<Core::Token> buf;

         std::unordered_set<Core::String> ignore;

         Core::TokenBuf &src;
         MacroMap       &macros;

         bool ignoreAll : 1;
      };
   }
}

#endif//GDCC__CPP__MacroTBuf_H__

