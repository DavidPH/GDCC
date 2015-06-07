//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
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

#include "CPP/Macro.hpp"
#include "CPP/MacroDTBuf.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // DefineDTBuf constructor
      //
      DefineDTBuf::DefineDTBuf(Core::TokenBuf &src_, MacroMap &macros_,
         bool importing_) :
         CPP::DirectiveTBuf{src_},
         macros(macros_),
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

         if(src.peek().tok == Core::TOK_WSpace) src.get();

         if(src.peek().tok != Core::TOK_Identi)
            throw Core::ParseExceptExpect(src.peek(), "identifier", false);

         Core::Token name = src.get();

         CPP::Macro newMacro = CPP::DefineDTBuf::GetMacro(src);

         // Check against existing macro.
         if(auto oldMacro = macros.find(name)) if(*oldMacro != newMacro)
         {
            throw Core::ParseExceptStr(name.pos,
               "incompatible macro redefinition");
         }

         macros.add(name.str, std::move(newMacro));
         if(isTemp) macros.tempAdd(name.str);

         return true;
      }
   }
}

// EOF

