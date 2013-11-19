//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS glyph preparation.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Program.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info::preFunc
      //
      void Info::preFunc()
      {
         if(func->defin) switch(func->ctype)
         {
         case GDCC::IR::CallType::LangACS:
            prog->getGlyphData(func->glyph).type = GDCC::IR::Type_Funct(func->ctype);
            break;

         case GDCC::IR::CallType::ScriptI:
            prog->getGlyphData(func->glyph).type = TypeWord;
            break;

         case GDCC::IR::CallType::ScriptS:
            prog->getGlyphData(func->glyph).type = GDCC::IR::Type_StrEn();
            break;

         default: break;
         }
      }

      //
      // Info::preStr
      //
      void Info::preStr()
      {
         prog->getGlyphData(str->glyph).type = GDCC::IR::Type_StrEn();
      }
   }
}

// EOF

