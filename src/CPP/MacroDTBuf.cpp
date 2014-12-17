//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Macro preprocessor directive token buffer.
//
//-----------------------------------------------------------------------------

#include "CPP/MacroDTBuf.hpp"

#include "CPP/Macro.hpp"
#include "CPP/MacroTBuf.hpp"

#include "Core/Exception.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CPP
   {
      //
      // DefineDTBuf::directive
      //
      bool DefineDTBuf::directive(Core::Token const &tok)
      {
         if(tok.tok != Core::TOK_Identi || tok.str != Core::STR_define)
            return false;

         src.get();

         if(src.peek().tok == Core::TOK_WSpace) src.get();

         if(src.peek().tok != Core::TOK_Identi)
            throw Core::ParseExceptExpect(src.peek().pos, "identifier",
               src.peek().str, false);

         Core::Token name = src.get();

         Macro newMacro;

         // Function-like macro.
         if(src.peek().tok == Core::TOK_ParenO)
         {
            src.get();
            newMacro.func = true;
            newMacro.args = GetArgs(src);
         }

         // Object-like macro.
         else if(src.peek().tok == Core::TOK_WSpace)
         {
            src.get();
            newMacro.func = false;
         }
         else if(src.peek().tok == Core::TOK_LnEnd || src.peek().tok == Core::TOK_EOF)
         {
            newMacro.func = false;
         }

         // Invalid macro.
         else
         {
            throw Core::ParseExceptExpect(src.peek().pos,
               "lparen or whitespace", src.peek().str, false);
         }

         // Read replacement list.
         newMacro.list = GetList(src);

         // Check against existing macro.
         if(auto oldMacro = macros.find(name)) if(*oldMacro != newMacro)
         {
            throw Core::ParseExceptStr(name.pos,
               "incompatible macro redefinition");
         }

         macros.add(name.str, std::move(newMacro));

         return true;
      }

      //
      // DefineDTBuf::GetArgs
      //
      Macro::Args DefineDTBuf::GetArgs(Core::TokenBuf &src)
      {
         std::vector<Core::String> args;

         if(src.peek().tok != Core::TOK_ParenC) for(;;)
         {
            if(src.peek().tok == Core::TOK_WSpace) src.get();

            if(src.peek().tok == Core::TOK_Identi)
            {
               args.emplace_back(src.get().str);

               if(src.peek().tok == Core::TOK_WSpace) src.get();

               if(src.peek().tok == Core::TOK_Comma) {src.get(); continue;}
               if(src.peek().tok == Core::TOK_ParenC) {src.get(); break;}

               throw Core::ParseExceptExpect(src.peek().pos, "',' or ')'",
                  src.peek().str, false);
            }

            if(src.peek().tok == Core::TOK_Dot3)
            {
               args.emplace_back((src.get(), nullptr));

               if(src.peek().tok == Core::TOK_WSpace) src.get();

               if(src.peek().tok == Core::TOK_ParenC) {src.get(); break;}

               throw Core::ParseExceptExpect(src.peek().pos, "')'",
                  src.peek().str, false);
            }

            throw Core::ParseExceptExpect(src.peek().pos,
              "identifier or '...'", src.peek().str, false);
         }
         else
            src.get();

         return {Core::Move, args.begin(), args.end()};
      }

      //
      // DefineDTBuf::GetList
      //
      Macro::List DefineDTBuf::GetList(Core::TokenBuf &src)
      {
         std::vector<Core::Token> list;

         // Skip leading whitespace.
         if(src.peek().tok == Core::TOK_WSpace) src.get();

         // Read replacement list.
         while(src.peek().tok != Core::TOK_LnEnd && src.peek().tok != Core::TOK_EOF)
            list.emplace_back(src.get());

         // Strip trailing whitespace.
         if(!list.empty() && list.back().tok == Core::TOK_WSpace)
            list.pop_back();

         return {Core::Move, list.begin(), list.end()};
      }

      //
      // LineDTBuf::directive
      //
      bool LineDTBuf::directive(Core::Token const &tok)
      {
         if(tok.tok != Core::TOK_Identi || tok.str != Core::STR_line)
            return false;

         // Clear directive name.
         src.get();

         // Read tokens.
         std::vector<Core::Token> toks;
         while(src.peek().tok != Core::TOK_LnEnd && src.peek().tok != Core::TOK_EOF)
            toks.emplace_back(src.get());

         // Build macro buffer.
         Core::ArrayTBuf abuf{toks.data(), toks.size()};
         MacroTBuf       mbuf{abuf, macros};

         while(mbuf.peek().tok == Core::TOK_WSpace) mbuf.get();

         if(mbuf.peek().tok != Core::TOK_Number)
            throw Core::ParseExceptExpect(mbuf.peek().pos, "digit-sequence",
               mbuf.peek().str, false);

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
            throw Core::ParseExceptExpect(mbuf.peek().pos, "digit-sequence",
               mbuf.peek().str, false);
         }

         macros.lineLine(num - numTok.pos.line);

         while(mbuf.peek().tok == Core::TOK_WSpace) mbuf.get();

         if(mbuf.peek().tok == Core::TOK_LnEnd || mbuf.peek().tok == Core::TOK_EOF)
            return true;

         if(mbuf.peek().tok != Core::TOK_String)
            throw Core::ParseExceptExpect(mbuf.peek().pos, "string",
               mbuf.peek().str, false);

         macros.lineFile(src.get().str);

         return true;
      }

      //
      // UndefDTBuf::directive
      //
      bool UndefDTBuf::directive(Core::Token const &tok)
      {
         if(tok.tok != Core::TOK_Identi || tok.str != Core::STR_undef)
            return false;

         src.get();

         if(src.peek().tok == Core::TOK_WSpace) src.get();

         if(src.peek().tok != Core::TOK_Identi)
            throw Core::ParseExceptExpect(src.peek().pos, "identifier",
               src.peek().str, false);

         macros.rem(src.get().str);

         if(src.peek().tok == Core::TOK_WSpace) src.get();

         if(src.peek().tok != Core::TOK_LnEnd && src.peek().tok != Core::TOK_EOF)
            throw Core::ParseExceptExpect(src.peek().pos, "end-line",
               src.peek().str, false);

         return true;
      }
   }
}

// EOf

