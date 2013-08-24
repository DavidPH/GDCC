//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Conditional inclusion directive token buffer.
//
//-----------------------------------------------------------------------------

#include "ConditionDTBuf.hpp"

#include "Macro.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace C
{
   //
   // ConditionDTBuf::getSkip
   //
   bool ConditionDTBuf::getSkip()
   {
      std::cerr << "STUB: " __FILE__ << ':' << __LINE__ << '\n';
      throw EXIT_FAILURE;
   }

   //
   // ConditionDTBuf::directive
   //
   bool ConditionDTBuf::directive(GDCC::Token const &tok)
   {
      if(tok.tok != GDCC::TOK_Identi)
         return isSkip();

      switch(static_cast<GDCC::StringIndex>(tok.str))
      {
      case GDCC::STR_endif:
         src.get();

         if(state.empty())
         {
            std::cerr << "ERROR: " << tok.pos << ": unmatched #endif\n";
            throw EXIT_FAILURE;
         }

         state.pop_back();

         return true;

      case GDCC::STR_elif:
         src.get();

         if(state.empty())
         {
            std::cerr << "ERROR: " << tok.pos << ": unmatched #elif\n";
            throw EXIT_FAILURE;
         }

         if(state.back().isElse)
         {
            std::cerr << "ERROR: " << tok.pos << ": #elif after #else\n";
            throw EXIT_FAILURE;
         }

         // If not possible to not skip, don't try to process expression.
         if(state.back().isDead || state.back().isElif) return true;

         state.back().isSkip = getSkip() && !state.back().isElif && !state.back().isDead;
         state.back().isElif = state.back().isElif || !state.back().isSkip;

         return true;

      case GDCC::STR_else:
         src.get();

         if(state.empty())
         {
            std::cerr << "ERROR: " << tok.pos << ": unmatched #else\n";
            throw EXIT_FAILURE;
         }

         if(state.back().isElse)
         {
            std::cerr << "ERROR: " << tok.pos << ": duplicate #else\n";
            throw EXIT_FAILURE;
         }

         state.back().isElse = true;
         state.back().isSkip = state.back().isElif || state.back().isDead;
         state.back().isElif = state.back().isElif || !state.back().isSkip;

         return true;

      case GDCC::STR_if:
         src.get();

         state.emplace_back(isSkip());

         // If not possible to not skip, don't try to process expression.
         if(state.back().isDead || state.back().isElif) return true;

         state.back().isSkip = getSkip() && !state.back().isElif && !state.back().isDead;
         state.back().isElif = state.back().isElif || !state.back().isSkip;

         return true;

      case GDCC::STR_ifdef:
         src.get();

         state.emplace_back(isSkip());

         if(src.peek().tok == GDCC::TOK_WSpace) src.get();

         if(src.peek().tok != GDCC::TOK_Identi)
         {
            std::cerr << "ERROR: " << tok.pos << ": expected identifier\n";
            throw EXIT_FAILURE;
         }

         state.back().isSkip = !Macro::Get(src.get());
         state.back().isElif = !state.back().isSkip;

         return true;

      case GDCC::STR_ifndef:
         src.get();

         state.emplace_back(isSkip());

         if(src.peek().tok == GDCC::TOK_WSpace) src.get();

         if(src.peek().tok != GDCC::TOK_Identi)
         {
            std::cerr << "ERROR: " << tok.pos << ": expected identifier\n";
            throw EXIT_FAILURE;
         }

         state.back().isSkip = Macro::Get(src.get());
         state.back().isElif = !state.back().isSkip;

         return true;

      default:
         return isSkip();
      }
   }

   //
   // ConditionDTBuf::underflow
   //
   void ConditionDTBuf::underflow()
   {
      for(;;)
      {
         DirectiveTBuf::underflow();
         if(tptr() == tend() || tptr()->tok == GDCC::TOK_EOF || !isSkip()) break;
         bumpt(1);
      }
   }
}

// EOF

