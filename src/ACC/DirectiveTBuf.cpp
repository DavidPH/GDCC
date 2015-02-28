//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS preprocessor directive token buffers.
//
//-----------------------------------------------------------------------------

#include "ACC/DirectiveTBuf.hpp"

#include "ACC/Pragma.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
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
         case Core::STR_nocompact:
         case Core::STR_nowadauthor:
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

         if(src.peek().tok == Core::TOK_WSpace) src.get();

         if(src.peek().tok != Core::TOK_String)
            throw Core::ParseExceptExpect(src.peek(), "string-literal", false);

         prag.stateLibrary.emplace_back(src.get().str);

         if(src.peek().tok == Core::TOK_WSpace) src.get();

         if(src.peek().tok != Core::TOK_LnEnd && src.peek().tok != Core::TOK_EOF)
            throw Core::ParseExceptExpect(src.peek(), "end-line", false);

         return true;
      }
   }
}

// EOF

