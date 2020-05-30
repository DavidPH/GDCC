//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2020 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C scoped glyph token buffer.
//
//-----------------------------------------------------------------------------

#include "CC/AsmGlyphTBuf.hpp"

#include "CC/Scope/Local.hpp"

#include "SR/Function.hpp"
#include "SR/Object.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // AsmGlyphTBuf::underflow
   //
   void AsmGlyphTBuf::underflow()
   {
      if(tptr() != tend()) return;

      if((buf[0] = src.get()).tok != Core::TOK_Colon)
         return sett(buf, buf, buf + 1);

      if((buf[1] = src.get()).tok != Core::TOK_Identi)
         return sett(buf, buf, buf + 2);

      // Label lookup.
      if(src.peek().tok == Core::TOK_Colon)
      {
         src.get();
         buf[0].setStrTok(scope.getLabel(buf[1].str), Core::TOK_String);
         return sett(buf, buf, buf + 1);
      }

      // Scoped identifier lookup.
      if(auto lookup = scope.lookup(buf[1].str)) switch(lookup.res)
      {
      case Lookup::Func:
         buf[0].setStrTok(lookup.resFunc->glyph, Core::TOK_String);
         return sett(buf, buf, buf + 1);

      case Lookup::Obj:
         buf[0].setStrTok(lookup.resObj->glyph, Core::TOK_String);
         return sett(buf, buf, buf + 1);

      default:
         break;
      }

      sett(buf, buf, buf + 2);
   }
}

// EOF

