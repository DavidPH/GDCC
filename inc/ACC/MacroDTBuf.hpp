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
   namespace ACC
   {
      class MacroMap;
      class PragmaData;

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
}

#endif//GDCC__ACC__MacroDTBuf_H__

