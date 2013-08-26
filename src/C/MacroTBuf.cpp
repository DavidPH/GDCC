//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Macro expanding token buffer.
//
//-----------------------------------------------------------------------------

#include "MacroTBuf.hpp"

#include "Macro.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace C
{
   //
   // MacroTBuf::underflow
   //
   void MacroTBuf::underflow()
   {
      if(tptr() != tend()) return;

      buf.clear();
      buf.emplace_back(src.get());

      if(buf[0].tok == GDCC::TOK_Identi) if(auto macro = Macro::Get(buf[0]))
      {
         if(macro->func)
         {
            // Skip past whitespace, but keep in case no expansion occurs.
            while(src.peek().tok == GDCC::TOK_WSpace || src.peek().tok == GDCC::TOK_LnEnd)
               buf.emplace_back(src.get());

            // If not immediately followed by (, then don't expand.
            if(src.peek().tok != GDCC::TOK_ParenO)
               goto flowed;

            src.get();

            std::vector<GDCC::Token> args;

            for(std::size_t depth = 0;;)
            {
               if(src.peek().tok == GDCC::TOK_ParenO) ++depth;
               if(src.peek().tok == GDCC::TOK_ParenC)
                  {if(depth) --depth; else {src.get(); break;}}

               args.emplace_back(src.get());
            }

            auto tok = buf[0];
            buf.clear();
            macro->expand(buf, tok, GDCC::MakeRange(&*args.cbegin(), &*args.cend()));
         }
         else
         {
            auto tok = buf[0];
            buf.clear();
            macro->expand(buf, tok);
         }
      }

   flowed:
      sett(&*buf.begin(), &*buf.begin(), &*buf.end());
   }
}

// EOF

