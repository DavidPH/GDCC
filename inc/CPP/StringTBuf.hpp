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

#ifndef GDCC__CPP__StringTBuf_H__
#define GDCC__CPP__StringTBuf_H__

#include "../Core/Parse.hpp"
#include "../Core/TokenBuf.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // StringTBuf
      //
      class StringTBuf : public Core::TokenBuf
      {
      public:
         StringTBuf(Core::TokenBuf &src_) : src(src_) {sett(buf, buf + 1, buf + 1);}

      protected:
         //
         // underflow
         //
         virtual void underflow()
         {
            if(tptr() != tend()) return;

            switch((buf[0] = src.get()).tok)
            {
            case Core::TOK_Charac:
            case Core::TOK_String:
               buf[0].str = Core::ParseStringC(buf[0].str);
               break;

            default: break;
            }

            sett(buf, buf, buf + 1);
         }

         Core::TokenBuf &src;
         Core::Token     buf[1];
      };
   }
}

#endif//GDCC__CPP__StringTBuf_H__

