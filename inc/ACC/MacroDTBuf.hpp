//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Macro preprocessor directive token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__MacroDTBuf_H__
#define GDCC__ACC__MacroDTBuf_H__

#include "../CPP/DirectiveTBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CPP
   {
      class MacroMap;
   }

   namespace ACC
   {
      //
      // DefineDTBuf
      //
      // Handles #define and #libdefine directives.
      //
      class DefineDTBuf : public CPP::DirectiveTBuf
      {
      public:
         DefineDTBuf(Core::TokenBuf &src_, CPP::MacroMap &macros_,
            bool importing_) :
            CPP::DirectiveTBuf{src_},
            macros(macros_),
            importing{importing_} {}

      protected:
         virtual bool directive(Core::Token const &tok);

         CPP::MacroMap &macros;

         bool importing;
      };
   }
}

#endif//GDCC__ACC__MacroDTBuf_H__

