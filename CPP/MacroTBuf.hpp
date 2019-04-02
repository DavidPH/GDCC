//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
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

#include "../CPP/Types.hpp"

#include "../Core/TokenBuf.hpp"
#include "../Core/Range.hpp"

#include <list>
#include <unordered_set>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CPP
{
   //
   // MacroTBuf
   //
   class MacroTBuf : public Core::TokenBuf
   {
   public:
      using Itr = std::list<Core::Token>::iterator;
      using Rng = Core::Range<Itr>;


      MacroTBuf(Core::TokenBuf &src, MacroMap &macros,
         Core::String vaArgStr = Core::STR___VA_ARGS__);

   protected:
      void applyMarker(Core::String str);

      bool expand(Itr itr);
      void expand(Itr end, Macro const &macro, Rng const &argRng);

      Core::String stringize(Rng const &arg);

      virtual void underflow();

      std::list<Core::Token> buf;

      std::unordered_set<Core::String> ignore;

      MacroMap       &macros;
      Core::TokenBuf &src;
      Core::String    vaArgStr;

      bool ignoreAll : 1;
   };
}

#endif//GDCC__CPP__MacroTBuf_H__

