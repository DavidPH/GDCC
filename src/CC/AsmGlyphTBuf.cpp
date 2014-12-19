//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C scoped glyph token buffer.
//
//-----------------------------------------------------------------------------

#include "CC/AsmGlyphTBuf.hpp"

#include "CC/Scope.hpp"

#include "AST/Function.hpp"
#include "AST/Object.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // AsmGlyphTBuf::underflow
      //
      void AsmGlyphTBuf::underflow()
      {
         if(tptr() != tend()) return;

         if((buf[0] = src.get()).tok == Core::TOK_Colon)
         {
            auto tok = src.peek();
            if(tok.tok == Core::TOK_Identi)
            {
               if(auto lookup = scope.lookup(tok.str)) switch(lookup.res)
               {
               case Lookup::Func:
                  src.get();
                  buf[0].setStrTok(lookup.resFunc->glyph, Core::TOK_String);
                  break;

               case Lookup::Obj:
                  src.get();
                  buf[0].setStrTok(lookup.resObj->glyph, Core::TOK_String);
                  break;

               default:
                  break;
               }
            }
         }

         sett(buf, buf, buf + 1);
      }
   }
}

// EOF

