//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Conditional inclusion directive token buffer.
//
//-----------------------------------------------------------------------------

#include "CPP/ConditionDTBuf.hpp"

#include "CPP/ConcatTBuf.hpp"
#include "CPP/GetExp.hpp"
#include "CPP/Macro.hpp"
#include "CPP/MacroTBuf.hpp"
#include "CPP/PPTokenTBuf.hpp"
#include "CPP/StringTBuf.hpp"

#include "Core/Exception.hpp"
#include "Core/Number.hpp"
#include "Core/TokenStream.hpp"
#include "Core/WSpaceTBuf.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CPP
{
   //
   // ConditionDTBuf::getSkip
   //
   bool ConditionDTBuf::getSkip()
   {
      // Read expression tokens.
      std::vector<Core::Token> toks;
      while(src.peek().tok != Core::TOK_LnEnd && src.peek().tok != Core::TOK_EOF)
         toks.emplace_back(src.get());

      // Set up token stream.
      Core::ArrayTBuf abuf{toks.data(), toks.size()};
      DefinedTBuf dbuf{abuf, macros};
      MacroTBuf mbuf{dbuf, macros};
      IdentiTBuf ibuf{mbuf};
      StringTBuf sbuf{ibuf};
      Core::WSpaceTBuf wbuf{sbuf};
      ConcatTBuf cbuf{wbuf};
      PPTokenTBuf pbuf{cbuf};
      Core::TokenStream in{&pbuf};

      // Read expression.
      auto exp = GetExp(in);

      // Ensure full consumption.
      if(in.peek().tok != Core::TOK_EOF)
         Core::Error(in.peek().pos, "unused tokens");

      // Evaluate expression.
      return !exp->getValue();
   }

   //
   // ConditionDTBuf::directive
   //
   bool ConditionDTBuf::directive(Core::Token const &tok)
   {
      if(tok.tok != Core::TOK_Identi)
         return isSkip();

      switch(tok.str)
      {
      case Core::STR_endif:
         src.get();

         if(state.empty())
            Core::Error(tok.pos, "unmatched #endif");

         state.pop_back();

         return true;

      case Core::STR_elif:
         src.get();

         if(state.empty())
            Core::Error(tok.pos, "unmatched #elif");

         if(state.back().isElse)
            Core::Error(tok.pos, "#elif after #else");

         // If not possible to not skip, don't try to process expression.
         if(state.back().isDead || state.back().isElif)
            return state.back().isSkip = true;

         state.back().isSkip = getSkip();
         state.back().isElif = !state.back().isSkip;

         return true;

      case Core::STR_else:
         src.get();

         if(state.empty())
            Core::Error(tok.pos, "unmatched #else");

         if(state.back().isElse)
            Core::Error(tok.pos, "duplicate #else");

         state.back().isElse = true;
         state.back().isSkip = state.back().isElif || state.back().isDead;
         state.back().isElif = state.back().isElif || !state.back().isSkip;

         return true;

      case Core::STR_if:
         src.get();

         state.emplace_back(isSkip());

         // If not possible to not skip, don't try to process expression.
         if(state.back().isDead) return true;

         state.back().isSkip = getSkip();
         state.back().isElif = !state.back().isSkip;

         return true;

      case Core::STR_ifdef:
         src.get();

         state.emplace_back(isSkip());

         while(src.peek().tok == Core::TOK_WSpace) src.get();

         if(src.peek().tok != Core::TOK_Identi)
            Core::Error(tok.pos, "expected identifier");

         state.back().isSkip = state.back().isDead || !macros.find(src.get());
         state.back().isElif = !state.back().isSkip;

         return true;

      case Core::STR_ifndef:
         src.get();

         state.emplace_back(isSkip());

         while(src.peek().tok == Core::TOK_WSpace) src.get();

         if(src.peek().tok != Core::TOK_Identi)
            Core::Error(tok.pos, "expected identifier");

         state.back().isSkip = state.back().isDead || macros.find(src.get());
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
         if(tptr() == tend() || tptr()->tok == Core::TOK_EOF || !isSkip()) break;
         bumpt(1);
      }
   }

   //
   // DefinedTBuf::underflow
   //
   void DefinedTBuf::underflow()
   {
      if(tptr() != tend()) return;

      if((buf[0] = src.get()).tok == Core::TOK_Identi && buf[0].str == Core::STR_defined)
      {
         while(src.peek().tok == Core::TOK_WSpace) src.get();

         bool paren = src.peek().tok == Core::TOK_ParenO;

         if(paren)
         {
            src.get();

            while(src.peek().tok == Core::TOK_WSpace) src.get();
         }

         if(src.peek().tok != Core::TOK_Identi)
            Core::Error(src.peek().pos, "expected identifier");

         buf[0].tok = Core::TOK_Number;
         buf[0].str = macros.find(src.get()) ? Core::STR_1 : Core::STR_0;

         if(paren)
         {
            while(src.peek().tok == Core::TOK_WSpace) src.get();

            if(src.peek().tok != Core::TOK_ParenC)
               Core::ErrorExpect(")", src.peek(), true);

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

      if((buf[0] = src.get()).tok == Core::TOK_Identi)
         buf[0].setStrTok(Core::STR_0, Core::TOK_Number);

      sett(buf, buf, buf + 1);
   }
}

// EOF

