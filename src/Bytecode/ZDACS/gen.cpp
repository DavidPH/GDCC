//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS glyph generation.
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
      // Info::gen
      //
      void Info::gen()
      {
         InfoBase::gen();

         for(auto &itr : prog->rangeSpaceMapArs()) genSpaceIniti(itr);

         genSpaceIniti(prog->getSpaceMapReg());
      }

      //
      // Info::genFunc
      //
      void Info::genFunc()
      {
         static GDCC::IR::CallType const FuncTypes[] =
         {
            GDCC::IR::CallType::LangACS,
         };

         // Back label glyph.
         backGlyphWord(func->label, CodeBase() + numChunkCODE);

         genBlock(func->block);

         if(func->defin) switch(func->ctype)
         {
         case GDCC::IR::CallType::LangACS:
            if(func->alloc)
               func->allocValue(*prog, FuncTypes);

            if(numChunkFUNC <= func->valueInt)
               numChunkFUNC = func->valueInt + 1;

            if(numChunkFNAM <= func->valueInt)
               numChunkFNAM = func->valueInt + 1;

            backGlyphFunc(func->glyph, func->valueInt, func->ctype);

            break;

         case GDCC::IR::CallType::ScriptI:
            if(func->alloc)
               func->allocValue(*prog, GDCC::IR::CallType::ScriptI);

            ++numChunkSPTR;

            if(func->sflagClS || func->sflagNet) ++numChunkSFLG;
            if(func->localReg > 20) ++numChunkSVCT;

            backGlyphWord(func->glyph, func->valueInt);

            break;

         case GDCC::IR::CallType::ScriptS:
            if(func->alloc)
               func->allocValue(*prog, GDCC::IR::CallType::ScriptS);

            ++numChunkSPTR;

            if(func->sflagClS || func->sflagNet) ++numChunkSFLG;
            if(func->localReg > 20) ++numChunkSVCT;

            if(numChunkSNAM <= func->valueInt)
               numChunkSNAM = func->valueInt + 1;

            if(auto str = prog->findStrEntVal(func->valueStr))
               backGlyphGlyph(func->glyph, str->glyph);

            break;

         default: break;
         }
      }

      //
      // Info::genStr
      //
      void Info::genStr()
      {
         if(!str->defin) return;

         if(str->alloc)
            str->allocValue(*prog);

         // Back address glyph.
         backGlyphStr(str->glyph, str->valueInt);

         if(numChunkSTRL <= str->valueInt)
            numChunkSTRL = str->valueInt + 1;
      }
   }
}

// EOF
