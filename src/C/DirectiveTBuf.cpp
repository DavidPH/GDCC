//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Preprocessor directive token buffers.
//
//-----------------------------------------------------------------------------

#include "DirectiveTBuf.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace C
{
   //
   // DirectiveTBuf::underflow
   //
   void DirectiveTBuf::underflow()
   {
      if(tptr() != tend()) return;

      switch((buf[0] = src.get()).tok)
      {
      case GDCC::TOK_Hash:
         if(!endl) break;

         // Ignore, but store, intervening whitespace.
         if(src.peek().tok == GDCC::TOK_WSpace)
         {
            buf[1] = src.get();
            sett(buf, buf, buf + 2);
         }
         else
            sett(buf, buf, buf + 1);

         // Silently ignore empty directives.
         if(src.peek().tok == GDCC::TOK_LnEnd)
         {
            // Yield the line.
            buf[0] = src.get();
            break;
         }

         // Attempt to process directive.
         if(directive(src.peek()))
         {
            // Skip any remaining tokens on the line.
            while(src.peek().tok != GDCC::TOK_LnEnd)
            {
               // Some people's code...
               if(src.peek().tok == GDCC::TOK_EOF)
               {
                  // Yield a line-end, in case the directive made tokens.
                  (buf[0] = src.get()).tok = GDCC::TOK_LnEnd;
                  goto flowed;
               }

               src.get();
            }

            // Yield the line.
            buf[0] = src.get();
            break;
         }

         return;

      case GDCC::TOK_WSpace:
         break;

      case GDCC::TOK_LnEnd:
         endl = true;
         break;

      default:
         endl = false;
         break;
      }

   flowed:
      sett(buf, buf, buf + 1);
   }

   //
   // ErrorDTBuf
   //
   bool ErrorDTBuf::directive(GDCC::Token const &tok)
   {
      if(tok.tok != GDCC::TOK_Identi || tok.str != GDCC::STR_error)
         return false;

      std::cerr << "ERROR: " << tok.pos << ": #error:";

      for(src.get(); src.peek().tok != GDCC::TOK_LnEnd && src.peek().tok != GDCC::TOK_EOF;)
         std::cerr << src.get().str;

      std::cerr << std::endl;
      throw EXIT_FAILURE;
   }
}

// EOF

