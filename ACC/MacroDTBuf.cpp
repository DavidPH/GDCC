//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Macro preprocessor directive token buffer.
//
//-----------------------------------------------------------------------------

#include "ACC/MacroDTBuf.hpp"

#include "ACC/Macro.hpp"
#include "ACC/Pragma.hpp"

#include "CPP/MacroDTBuf.hpp"
#include "CPP/MacroTBuf.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // DefineDTBuf constructor
   //
   DefineDTBuf::DefineDTBuf(Core::TokenBuf &src_, MacroMap &macros_,
      PragmaData &pragd_, bool importing_) :
      CPP::DirectiveTBuf{src_},
      macros(macros_),
      pragd(pragd_),
      importing{importing_}
   {
   }

   //
   // DefineDTBuf::directive
   //
   bool DefineDTBuf::directive(Core::Token const &tok)
   {
      if(tok.tok != Core::TOK_Identi)
         return false;

      bool isTemp;

      if(tok.str == Core::STR_define)
         isTemp = importing;

      else if(tok.str == Core::STR_libdefine)
         isTemp = false;

      else
         return false;

      src.get();

      while(src.peek().tok == Core::TOK_WSpace) src.get();

      if(src.peek().tok != Core::TOK_Identi)
         Core::ErrorExpect("identifier", src.peek());

      Core::Token name = src.get();

      CPP::Macro newMacro = CPP::DefineDTBuf::GetMacro(src);

      // If not a function-like macro, then pre-expand and add parentheses.
      if(!newMacro.func && !pragd.stateDefineRaw)
      {
         std::vector<Core::Token> list;
         list.reserve(newMacro.list.size() + 2);

         list.emplace_back(name.pos, Core::STR_TOK_ParenO, Core::TOK_ParenO);

         Core::ArrayTBuf abuf{newMacro.list.data(), newMacro.list.size()};
         CPP::MacroTBuf  mbuf{abuf, macros, Core::STR___va_args__};

         while(mbuf.peek().tok != Core::TOK_EOF)
            list.emplace_back(mbuf.get());

         list.emplace_back(name.pos, Core::STR_TOK_ParenC, Core::TOK_ParenC);

         // If expanded to a single token, do not include parentheses.
         if(list.size() == 3)
            newMacro.list = {&list[1], &list[2]};
         else
            newMacro.list = {list.begin(), list.end()};
      }

      // Check against existing macro.
      if(auto oldMacro = macros.find(name)) if(*oldMacro != newMacro)
         Core::Error(name.pos, "incompatible macro redefinition");

      macros.add(name.str, std::move(newMacro));
      if(isTemp) macros.tempAdd(name.str);

      return true;
   }
}

// EOF

