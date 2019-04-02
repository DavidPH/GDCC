//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
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

#include "../ACC/Types.hpp"

#include "../CPP/DirectiveTBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::ACC
{
   //
   // DefineDTBuf
   //
   // Handles #define and #libdefine directives.
   //
   class DefineDTBuf : public CPP::DirectiveTBuf
   {
   public:
      DefineDTBuf(Core::TokenBuf &src, MacroMap &macros, PragmaData &pragd,
         bool importing);

   protected:
      virtual bool directive(Core::Token const &tok);

      MacroMap   &macros;
      PragmaData &pragd;

      bool importing;
   };
}

#endif//GDCC__ACC__MacroDTBuf_H__

