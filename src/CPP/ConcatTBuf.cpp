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

#include "CPP/ConcatTBuf.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // ConcatTBuf::underflow
      //
      void ConcatTBuf::underflow()
      {
         if(tptr() != tend()) return;

         buf.clear();

         buf.emplace_back(src.get());

         if(buf[0].tok == Core::TOK_String)
         {
            for(;;) switch(src.peek().tok)
            {
            case Core::TOK_String:
               buf[0].str += src.get().str;
               break;

            case Core::TOK_WSpace:
            case Core::TOK_LnEnd:
               buf.emplace_back(src.get());
               break;

            default: goto flowed;
            }
         }

      flowed:
         sett(buf.data(), buf.data(), buf.data() + buf.size());
      }
   }
}

// EOF

