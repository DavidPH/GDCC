//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// String concatenating token buffer.
//
//-----------------------------------------------------------------------------

#include "CPP/ConcatTBuf.hpp"

#include "Core/Exception.hpp"


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

         switch(buf[0].tok)
         {
         case Core::TOK_StrIdx:
         case Core::TOK_StrU08:
         case Core::TOK_StrU16:
         case Core::TOK_StrU32:
         case Core::TOK_StrWid:
         case Core::TOK_String:
            for(;;) switch(src.peek().tok)
            {
            case Core::TOK_StrIdx:
            case Core::TOK_StrU08:
            case Core::TOK_StrU16:
            case Core::TOK_StrU32:
            case Core::TOK_StrWid:
               if(buf[0].tok != Core::TOK_String && buf[0].tok != src.peek().tok)
                  throw Core::ExceptStr(src.peek().pos, "invalid concatenation");

               buf[0].tok = src.peek().tok;
            case Core::TOK_String:
               buf[0].str += src.get().str;
               break;

            case Core::TOK_WSpace:
            case Core::TOK_LnEnd:
               buf.emplace_back(src.get());
               break;

            default: goto flowed;
            }

         default: break;
         }

      flowed:
         sett(buf.data(), buf.data(), buf.data() + buf.size());
      }
   }
}

// EOF

