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

#include "GDCC/IR/Function.hpp"
#include "GDCC/IR/Object.hpp"
#include "GDCC/IR/StrEnt.hpp"


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

         for(auto &itr : GDCC::IR::Space::MapArs) genSpaceIniti(itr.second);

         genSpaceIniti(GDCC::IR::Space::MapReg);
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
         BackGlyphWord(func->label, CodeBase() + numChunkCODE);

         genBlock(func->block);

         if(func->defin) switch(func->ctype)
         {
         case GDCC::IR::CallType::LangACS:
            if(func->alloc)
               func->allocValue(FuncTypes);

            if(numChunkFUNC <= func->valueInt)
               numChunkFUNC = func->valueInt + 1;

            if(numChunkFNAM <= func->valueInt)
               numChunkFNAM = func->valueInt + 1;

            BackGlyphFunc(func->glyph, func->valueInt, func->ctype);

            break;

         case GDCC::IR::CallType::ScriptI:
            if(func->alloc)
               func->allocValue(GDCC::IR::CallType::ScriptI);

            ++numChunkSPTR;

            if(func->sflagClS || func->sflagNet) ++numChunkSFLG;
            if(func->localReg > 20) ++numChunkSVCT;

            BackGlyphWord(func->glyph, func->valueInt);

            break;

         case GDCC::IR::CallType::ScriptS:
            if(func->alloc)
               func->allocValue(GDCC::IR::CallType::ScriptS);

            ++numChunkSPTR;

            if(func->sflagClS || func->sflagNet) ++numChunkSFLG;
            if(func->localReg > 20) ++numChunkSVCT;

            if(numChunkSNAM <= func->valueInt)
               numChunkSNAM = func->valueInt + 1;

            if(auto str = GDCC::IR::StrEnt::FindValue(func->valueStr))
               BackGlyphGlyph(func->glyph, str->glyph);

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
            str->allocValue();

         // Back address glyph.
         BackGlyphStr(str->glyph, str->valueInt);

         if(numChunkSTRL <= str->valueInt)
            numChunkSTRL = str->valueInt + 1;
      }
   }
}

// EOF

