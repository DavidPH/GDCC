//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// String parsing token buffer.
//
//-----------------------------------------------------------------------------

#ifndef C__StringTBuf_H__
#define C__StringTBuf_H__

#include "GDCC/Parse.hpp"
#include "GDCC/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace C
{
   //
   // StringTBuf
   //
   class StringTBuf : public GDCC::TokenBuf
   {
   public:
      StringTBuf(GDCC::TokenBuf &src_) : src{src_} {sett(buf, buf + 1, buf + 1);}

   protected:
      //
      // underflow
      //
      virtual void underflow()
      {
         if(tptr() != tend()) return;

         switch((buf[0] = src.get()).tok)
         {
         case GDCC::TOK_Charac:
         case GDCC::TOK_String:
            buf[0].str = GDCC::ParseStringC(buf[0].str);
            break;

         default: break;
         }

         sett(buf, buf, buf + 1);
      }

      GDCC::TokenBuf &src;
      GDCC::Token     buf[1];
   };
}

#endif//C__StringTBuf_H__

