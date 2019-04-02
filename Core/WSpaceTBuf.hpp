//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Whitespace stripping token buffer.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Core__WSpaceTBuf_H__
#define GDCC__Core__WSpaceTBuf_H__

#include "../Core/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::Core
{
   //
   // WSpaceTBuf
   //
   class WSpaceTBuf : public TokenBuf
   {
   public:
      WSpaceTBuf(TokenBuf &src_) : src(src_) {sett(buf, buf + 1, buf + 1);}

   protected:
      //
      // underflow
      //
      virtual void underflow()
      {
         if(tptr() != tend()) return;

         for(;;) switch((buf[0] = src.get()).tok)
         {
         case TOK_WSpace:

         case TOK_LnEnd:
         case TOK_Space:
         case TOK_Tabul:
            break;

         default: goto flowed;
         }

      flowed:
         sett(buf, buf, buf + 1);
      }

      TokenBuf &src;
      Token     buf[1];
   };
}

#endif//GDCC__Core__WSpaceTBuf_H__

