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

#include <vector>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace C
{
   //
   // MacroTBuf
   //
   class MacroTBuf : public GDCC::TokenBuf
   {
   public:
      explicit MacroTBuf(GDCC::TokenBuf &src_) : src{src_}
         {sett(nullptr, nullptr, nullptr);}

   protected:
      virtual void underflow();

      GDCC::TokenBuf &src;

      std::vector<GDCC::Token> buf;
   };
}

#endif//C__MacroTBuf_H__

