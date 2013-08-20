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

#ifndef C__ConcatTBuf_H__
#define C__ConcatTBuf_H__

#include "GDCC/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace C
{
   //
   // ConcatTBuf
   //
   class ConcatTBuf : public GDCC::TokenBuf
   {
   public:
      ConcatTBuf(GDCC::TokenBuf &src_) : src{src_} {sett(buf, buf + 1, buf + 1);}

   protected:
      //
      // underflow
      //
      virtual void underflow()
      {
         if(tptr() != tend()) return;

         switch((buf[0] = src.get()).tok)
         {
         case GDCC::TOK_String:
            for(;;) switch(src.peek().tok)
            {
            case GDCC::TOK_String:
               buf[0].str += src.get().str;
               break;

            default: goto flowed;
            }
            break;

         default: break;
         }

      flowed:
         sett(buf, buf, buf + 1);
      }

      GDCC::TokenBuf &src;
      GDCC::Token     buf[1];
   };
}

#endif//C__ConcatTBuf_H__

