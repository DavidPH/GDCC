//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Macro preprocessor directive token buffer.
//
//-----------------------------------------------------------------------------

#ifndef C__MacroDTBuf_H__
#define C__MacroDTBuf_H__

#include "DirectiveTBuf.hpp"

#include "GDCC/Array.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace C
{
   //
   // DefineDTBuf
   //
   // Handles #define directives.
   //
   class DefineDTBuf final : public DirectiveTBuf
   {
   public:
      explicit DefineDTBuf(GDCC::TokenBuf &src_) : DirectiveTBuf{src_} {}


      static GDCC::Array<GDCC::String> GetArgs(GDCC::TokenBuf &src);
      static GDCC::Array<GDCC::Token> GetList(GDCC::TokenBuf &src);

   protected:
      virtual bool directive(GDCC::Token const &tok);
   };

   //
   // LineDTBuf
   //
   // Handles #line directives.
   //
   class LineDTBuf final : public DirectiveTBuf
   {
   public:
      explicit LineDTBuf(GDCC::TokenBuf &src_) : DirectiveTBuf{src_} {}

   protected:
      virtual bool directive(GDCC::Token const &tok);
   };

   //
   // UndefDTBuf
   //
   // Handles #under directives.
   //
   class UndefDTBuf final : public DirectiveTBuf
   {
   public:
      explicit UndefDTBuf(GDCC::TokenBuf &src_) : DirectiveTBuf{src_} {}

   protected:
      virtual bool directive(GDCC::Token const &tok);
   };
}

#endif//C__MacroDTBuf_H__

