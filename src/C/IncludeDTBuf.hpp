//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// #include directive token buffer.
//
//-----------------------------------------------------------------------------

#ifndef C__IncludeDTBuf_H__
#define C__IncludeDTBuf_H__

#include "DirectiveTBuf.hpp"

#include <memory>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace C
{
   //
   // IncludeDTBuf
   //
   class IncludeDTBuf final : public DirectiveTBuf
   {
   public:
      explicit IncludeDTBuf(GDCC::TokenBuf &src_) : DirectiveTBuf{src_} {}

      virtual ~IncludeDTBuf();

   protected:
      virtual bool directive(GDCC::Token const &tok);

      virtual void underflow();

      bool tryIncSys(GDCC::String str);
      bool tryIncUsr(GDCC::String str);

      std::unique_ptr<std::streambuf> str;
      std::unique_ptr<IncStream>      inc;
   };
}

#endif//C__IncludeDTBuf_H__

