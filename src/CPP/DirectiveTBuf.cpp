//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Preprocessor directive token buffers.
//
//-----------------------------------------------------------------------------

#include "CPP/DirectiveTBuf.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CPP
{
   //
   // DirectiveTBuf::underflow
   //
   void DirectiveTBuf::underflow()
   {
      if(tptr() != tend()) return;

      switch((buf[0] = src.get()).tok)
      {
      case Core::TOK_Hash:
         if(!endl) break;

         // Ignore, but store, intervening whitespace.
         if(src.peek().tok == Core::TOK_WSpace)
         {
            buf[1] = src.get();
            sett(buf, buf, buf + 2);
         }
         else
            sett(buf, buf, buf + 1);

         // Silently ignore empty directives.
         if(src.peek().tok == Core::TOK_LnEnd)
         {
            // Yield the line.
            buf[0] = src.get();
            break;
         }

         // Attempt to process directive.
         if(directive(src.peek()))
         {
            // Skip any remaining tokens on the line.
            while(src.peek().tok != Core::TOK_LnEnd)
            {
               // Some people's code...
               if(src.peek().tok == Core::TOK_EOF)
               {
                  // Yield a line-end, in case the directive made tokens.
                  (buf[0] = src.get()).tok = Core::TOK_LnEnd;
                  goto flowed;
               }

               src.get();
            }

            // Yield the line.
            buf[0] = src.get();
            break;
         }

         return;

      case Core::TOK_WSpace:
         break;

      case Core::TOK_LnEnd:
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
   // ErrorDTBuf::directive
   //
   bool ErrorDTBuf::directive(Core::Token const &tok)
   {
      if(tok.tok != Core::TOK_Identi || tok.str != Core::STR_error)
         return false;

      std::cerr << "ERROR: " << tok.pos << ": #error:";

      for(src.get(); src.peek().tok != Core::TOK_LnEnd && src.peek().tok != Core::TOK_EOF;)
         std::cerr << src.get().str;

      std::cerr << std::endl;
      throw EXIT_FAILURE;
   }
}

// EOF

