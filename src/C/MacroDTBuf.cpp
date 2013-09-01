//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Macro preprocessor directive token buffer.
//
//-----------------------------------------------------------------------------

#include "MacroDTBuf.hpp"

#include "Macro.hpp"
#include "MacroTBuf.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace C
{
   //
   // DefineDTBuf::directive
   //
   bool DefineDTBuf::directive(GDCC::Token const &tok)
   {
      if(tok.tok != GDCC::TOK_Identi || tok.str != GDCC::STR_define)
         return false;

      src.get();

      if(src.peek().tok == GDCC::TOK_WSpace) src.get();

      if(src.peek().tok != GDCC::TOK_Identi)
      {
         std::cerr << "ERROR: " << src.peek().pos << ": expected identifier\n";
         throw EXIT_FAILURE;
      }

      GDCC::Token name = src.get();

      Macro newMacro;

      // Function-like macro.
      if(src.peek().tok == GDCC::TOK_ParenO)
      {
         src.get();
         newMacro.func = true;
         newMacro.args = GetArgs(src);
      }

      // Object-like macro.
      else if(src.peek().tok == GDCC::TOK_WSpace)
      {
         src.get();
         newMacro.func = false;
      }
      else if(src.peek().tok == GDCC::TOK_LnEnd || src.peek().tok == GDCC::TOK_EOF)
      {
         newMacro.func = false;
      }

      // Invalid macro.
      else
      {
         std::cerr << "ERROR: " << src.peek().pos << ": expected lparen or whitespace\n";
         throw EXIT_FAILURE;
      }

      // Read replacement list.
      newMacro.list = GetList(src);

      // Check against existing macro.
      if(auto oldMacro = Macro::Get(name)) if(*oldMacro != newMacro)
      {
         std::cerr << "ERROR: " << name.pos << ": incompatible macro redefinition\n";
         throw EXIT_FAILURE;
      }

      Macro::Add(name.str, std::move(newMacro));

      return true;
   }

   //
   // DefineDTBuf::GetArgs
   //
   Macro::Args DefineDTBuf::GetArgs(GDCC::TokenBuf &src)
   {
      std::vector<GDCC::String> args;

      if(src.peek().tok != GDCC::TOK_ParenC) for(;;)
      {
         if(src.peek().tok == GDCC::TOK_WSpace) src.get();

         if(src.peek().tok == GDCC::TOK_Identi)
         {
            args.emplace_back(src.get().str);

            if(src.peek().tok == GDCC::TOK_WSpace) src.get();

            if(src.peek().tok == GDCC::TOK_Comma) {src.get(); continue;}
            if(src.peek().tok == GDCC::TOK_ParenC) {src.get(); break;}

            std::cerr << "ERROR: " << src.peek().pos << ": expected , or )\n";
            throw EXIT_FAILURE;
         }

         if(src.peek().tok == GDCC::TOK_Dot3)
         {
            args.emplace_back((src.get(), GDCC::STRNULL));

            if(src.peek().tok == GDCC::TOK_WSpace) src.get();

            if(src.peek().tok == GDCC::TOK_ParenC) {src.get(); break;}

            std::cerr << "ERROR: " << src.peek().pos << ": expected )\n";
            throw EXIT_FAILURE;
         }

         std::cerr << "ERROR: " << src.peek().pos << ": expected identifier or ...\n";
         throw EXIT_FAILURE;
      }
      else
         src.get();

      return Macro::Args(GDCC::Move, args.begin(), args.end());
   }

   //
   // DefineDTBuf::GetList
   //
   Macro::List DefineDTBuf::GetList(GDCC::TokenBuf &src)
   {
      std::vector<GDCC::Token> list;

      // Skip leading whitespace.
      if(src.peek().tok == GDCC::TOK_WSpace) src.get();

      // Read replacement list.
      while(src.peek().tok != GDCC::TOK_LnEnd && src.peek().tok != GDCC::TOK_EOF)
         list.emplace_back(src.get());

      // Strip trailing whitespace.
      if(!list.empty() && list.back().tok == GDCC::TOK_WSpace)
         list.pop_back();

      return Macro::List(GDCC::Move, list.begin(), list.end());
   }

   //
   // LineDTBuf::directive
   //
   bool LineDTBuf::directive(GDCC::Token const &tok)
   {
      if(tok.tok != GDCC::TOK_Identi || tok.str != GDCC::STR_line)
         return false;

      // Clear directive name.
      src.get();

      // Read tokens.
      std::vector<GDCC::Token> toks;
      while(src.peek().tok != GDCC::TOK_LnEnd && src.peek().tok != GDCC::TOK_EOF)
         toks.emplace_back(src.get());

      // Build macro buffer.
      GDCC::ArrayTBuf abuf{toks.data(), toks.size()};
      MacroTBuf mbuf{abuf};

      while(mbuf.peek().tok == GDCC::TOK_WSpace) mbuf.get();

      if(mbuf.peek().tok != GDCC::TOK_Number)
      {
         std::cerr << "ERROR: " << mbuf.peek().pos << ": expected digit-sequence\n";
         throw EXIT_FAILURE;
      }

      std::size_t num = 0;
      auto numTok = mbuf.get();

      for(char c : numTok.str) switch(c)
      {
      case '0': num = num * 10 + 0; break;
      case '1': num = num * 10 + 1; break;
      case '2': num = num * 10 + 2; break;
      case '3': num = num * 10 + 3; break;
      case '4': num = num * 10 + 4; break;
      case '5': num = num * 10 + 5; break;
      case '6': num = num * 10 + 6; break;
      case '7': num = num * 10 + 7; break;
      case '8': num = num * 10 + 8; break;
      case '9': num = num * 10 + 9; break;

      default:
         std::cerr << "ERROR: " << numTok.pos << ": expected digit-sequence\n";
         throw EXIT_FAILURE;
      }

      Macro::LineLine(num - numTok.pos.line);

      while(mbuf.peek().tok == GDCC::TOK_WSpace) mbuf.get();

      if(mbuf.peek().tok == GDCC::TOK_LnEnd || mbuf.peek().tok == GDCC::TOK_EOF)
         return true;

      if(mbuf.peek().tok != GDCC::TOK_String)
      {
         std::cerr << "ERROR: " << numTok.pos << ": expected string\n";
         throw EXIT_FAILURE;
      }

      Macro::LineFile(src.get().str);

      return true;
   }

   //
   // UndefDTBuf::directive
   //
   bool UndefDTBuf::directive(GDCC::Token const &tok)
   {
      if(tok.tok != GDCC::TOK_Identi || tok.str != GDCC::STR_undef)
         return false;

      src.get();

      if(src.peek().tok == GDCC::TOK_WSpace) src.get();

      if(src.peek().tok != GDCC::TOK_Identi)
      {
         std::cerr << "ERROR: " << src.peek().pos << ": expected identifier\n";
         throw EXIT_FAILURE;
      }

      Macro::Rem(src.get().str);

      if(src.peek().tok == GDCC::TOK_WSpace) src.get();

      if(src.peek().tok != GDCC::TOK_LnEnd)
      {
         std::cerr << "ERROR: " << src.peek().pos << ": expected end-line\n";
         throw EXIT_FAILURE;
      }

      return true;
   }
}

// EOf

