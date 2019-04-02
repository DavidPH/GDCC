//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Function label mangling token buffer.
//
//-----------------------------------------------------------------------------

#include "AS/LabelTBuf.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::AS
{
   //
   // LabelTBuf::underflow
   //
   void LabelTBuf::underflow()
   {
      if(tptr() != tend()) return;

      if((buf[0] = src.get()).tok == Core::TOK_Colon)
      {
         if(src.peek().tok == Core::TOK_String)
            buf[0].setStrTok(prefix + src.get().str, Core::TOK_String);
      }

      sett(buf, buf, buf + 1);
   }
}

// EOF

