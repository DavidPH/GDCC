//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS preprocessor directive token buffers.
//
//-----------------------------------------------------------------------------

#include "ACC/DirectiveTBuf.hpp"

#include "ACC/TSource.hpp"
#include "ACC/Parse.hpp"
#include "ACC/Pragma.hpp"

#include "Core/Exception.hpp"
#include "Core/Parse.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // IgnoreDTBuf::directive
   //
   bool IgnoreDTBuf::directive(Core::Token const &tok)
   {
      if(tok.tok != Core::TOK_Identi) return false;

      switch(tok.str)
      {
      case Core::STR_encryptstrings:
      case Core::STR_endregion:
      case Core::STR_nocompact:
      case Core::STR_nowadauthor:
      case Core::STR_region:
      case Core::STR_wadauthor:
         return true;

      case Core::STR_define:
      case Core::STR_import:
      case Core::STR_include:
         return importing;

      case Core::STR_library:
         return !importing;

      default:
         return false;
      }
   }

   //
   // LibraryDTBuf::directive
   //
   bool LibraryDTBuf::directive(Core::Token const &tok)
   {
      if(tok.tok != Core::TOK_Identi || tok.str != Core::STR_library)
         return false;

      src.get();

      while(src.peek().tok == Core::TOK_WSpace) src.get();

      if(src.peek().tok != Core::TOK_String)
         Core::ErrorExpect("string-literal", src.peek());

      prag.stateLibrary.emplace_back(
         Core::ParseStringC(src.get().str, 0, '"', ParseEscape));

      while(src.peek().tok == Core::TOK_WSpace) src.get();

      if(src.peek().tok != Core::TOK_LnEnd && src.peek().tok != Core::TOK_EOF)
         Core::ErrorExpect("end-line", src.peek());

      return true;
   }
}

// EOF

