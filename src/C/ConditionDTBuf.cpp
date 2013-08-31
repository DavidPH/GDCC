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

#include "ConcatTBuf.hpp"
#include "GetExpIR.hpp"
#include "Macro.hpp"
#include "MacroTBuf.hpp"
#include "PPTokenTBuf.hpp"
#include "StringTBuf.hpp"

#include "GDCC/Number.hpp"
#include "GDCC/TokenStream.hpp"
#include "GDCC/WSpaceTBuf.hpp"

#include "GDCC/IR/Exp.hpp"

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
      // Read expression tokens.
      std::vector<GDCC::Token> toks;
      while(src.peek().tok != GDCC::TOK_LnEnd && src.peek().tok != GDCC::TOK_EOF)
         toks.emplace_back(src.get());

      // Set up token stream.
      GDCC::ArrayTBuf abuf{toks.data(), toks.size()};
      DefinedTBuf dbuf{abuf};
      MacroTBuf mbuf{dbuf};
      IdentiTBuf ibuf{mbuf};
      StringTBuf sbuf{ibuf};
      GDCC::WSpaceTBuf wbuf{sbuf};
      ConcatTBuf cbuf{wbuf};
      PPTokenTBuf pbuf{cbuf};
      GDCC::TokenStream in{&pbuf};

      // Read expression.
      GDCC::IR::Exp::Ref exp = GetExpIR(in);

      // Ensure full consumption.
      if(in.peek().tok != GDCC::TOK_EOF)
      {
         std::cerr << "ERROR: " << exp->pos << ": unused tokens\n";
         throw EXIT_FAILURE;
      }

      // Evaluate expression.
      return !exp->getValue();
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

   //
   // DefinedTBuf::underflow
   //
   void DefinedTBuf::underflow()
   {
      if(tptr() != tend()) return;

      if((buf[0] = src.get()).tok == GDCC::TOK_Identi && buf[0].str == GDCC::STR_defined)
      {
         if(src.peek().tok == GDCC::TOK_WSpace) src.get();

         bool paren = src.peek().tok == GDCC::TOK_ParenO;

         if(paren)
         {
            src.get();

            if(src.peek().tok == GDCC::TOK_WSpace) src.get();
         }

         if(src.peek().tok != GDCC::TOK_Identi)
         {
            std::cerr << "ERROR: " << src.peek().pos << ": expected identifier\n";
            throw EXIT_FAILURE;
         }

         buf[0].tok = GDCC::TOK_Number;
         buf[0].str = Macro::Get(src.get()) ? GDCC::STR_1 : GDCC::STR_0;

         if(paren)
         {
            if(src.peek().tok == GDCC::TOK_WSpace) src.get();

            if(src.peek().tok != GDCC::TOK_ParenC)
            {
               std::cerr << "ERROR: " << (src.unget(), src.peek().pos) << ": expected )\n";
               throw EXIT_FAILURE;
            }

            src.get();
         }
      }

      sett(buf, buf, buf + 1);
   }

   //
   // IdentiTBuf::underflow
   //
   void IdentiTBuf::underflow()
   {
      if(tptr() != tend()) return;

      if((buf[0] = src.get()).tok == GDCC::TOK_Identi)
         buf[0].setStrTok(GDCC::STR_0, GDCC::TOK_Number);

      sett(buf, buf, buf + 1);
   }
}

// EOF

