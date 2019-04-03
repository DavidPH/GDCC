//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// String concatenating token buffer.
//
//-----------------------------------------------------------------------------

#include "NTSC/ConcatTBuf.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::NTSC
{
   //
   // ConcatTBuf::underflow
   //
   void ConcatTBuf::underflow()
   {
      if(tptr() != tend()) return;

      buf[0] = src.get();

      switch(buf[0].tok)
      {
      case Core::TOK_Charac:
      case Core::TOK_String:
         while(src.peek().tok == buf->tok)
            buf->str += src.get().str;
         break;

      default:
         break;
      }

      sett(buf, buf, buf + 1);
   }
}

// EOF

