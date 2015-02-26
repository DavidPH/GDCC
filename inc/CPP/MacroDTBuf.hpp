//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Macro preprocessor directive token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CPP__MacroDTBuf_H__
#define GDCC__CPP__MacroDTBuf_H__

#include "../CPP/DirectiveTBuf.hpp"

#include "../Core/Array.hpp"


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
      // DefineDTBuf
      //
      // Handles #define directives.
      //
      class DefineDTBuf final : public DirectiveTBuf
      {
      public:
         DefineDTBuf(Core::TokenBuf &src_, MacroMap &macros_) :
            DirectiveTBuf{src_}, macros(macros_) {}


         static Core::Array<Core::String> GetArgs(Core::TokenBuf &src);

         static Macro GetMacro(Core::TokenBuf &src);

         static Core::Array<Core::Token> GetList(Core::TokenBuf &src);

      protected:
         virtual bool directive(Core::Token const &tok);

         MacroMap &macros;
      };

      //
      // LineDTBuf
      //
      // Handles #line directives.
      //
      class LineDTBuf final : public DirectiveTBuf
      {
      public:
         LineDTBuf(Core::TokenBuf &src_, MacroMap &macros_) :
            DirectiveTBuf{src_}, macros(macros_) {}

      protected:
         virtual bool directive(Core::Token const &tok);

         MacroMap &macros;
      };

      //
      // UndefDTBuf
      //
      // Handles #under directives.
      //
      class UndefDTBuf final : public DirectiveTBuf
      {
      public:
         UndefDTBuf(Core::TokenBuf &src_, MacroMap &macros_) :
            DirectiveTBuf{src_}, macros(macros_) {}

      protected:
         virtual bool directive(Core::Token const &tok);

         MacroMap &macros;
      };
   }
}

#endif//GDCC__CPP__MacroDTBuf_H__

