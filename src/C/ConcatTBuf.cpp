//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// String concatenating token buffer.
//
//-----------------------------------------------------------------------------

#include "ConcatTBuf.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace C
{
   //
   // ConcatTBuf::underflow
   //
   void ConcatTBuf::underflow()
   {
      if(tptr() != tend()) return;

      buf.clear();

      buf.emplace_back(src.get());

      if(buf[0].tok == GDCC::TOK_String)
      {
         for(;;) switch(src.peek().tok)
         {
         case GDCC::TOK_String:
            buf[0].str += src.get().str;
            break;

         case GDCC::TOK_WSpace:
         case GDCC::TOK_LnEnd:
            buf.emplace_back(src.get());
            break;

         default: goto flowed;
         }
      }

   flowed:
      sett(buf.data(), buf.data(), buf.data() + buf.size());
   }
}

// EOF

