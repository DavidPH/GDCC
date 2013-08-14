//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR code translation.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Function.hpp"
#include "GDCC/IR/Object.hpp"
#include "GDCC/IR/StrEnt.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info::tr
      //
      void Info::tr()
      {
         InfoBase::tr();

         for(auto &itr : GDCC::IR::Space::MapArs) trSpaceIniti(itr.second);

         trSpaceIniti(GDCC::IR::Space::MapReg);
      }

      //
      // Info::trFunc
      //
      void Info::trFunc(GDCC::IR::Function &func_)
      {
         static GDCC::IR::CallType const FuncTypes[] =
         {
            GDCC::IR::CallType::LangACS,
         };

         func = &func_;

         // Back label glyph.
         BackGlyphWord(func->label, jumpPos);

         trBlock(func->block);

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

         case GDCC::IR::CallType::Script:
            func->ctype = GDCC::IR::CallType::ScriptI;

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

         func = nullptr;
      }

      //
      // Info::trSpace
      //
      void Info::trSpace(GDCC::IR::Space &space)
      {
         //
         // allocMapReg
         //
         // Handles the unusual rules for allocating map registers.
         //
         auto allocMapReg = [](GDCC::IR::Object *self)
         {
            if(!self->space) return;

            for(;; ++self->value)
            {
               auto lo = self->value;
               auto hi = self->words + lo;

               for(auto const &obj : self->space->obset)
               {
                  if(obj->alloc || obj == self)
                     continue;

                  auto objLo = obj->value;
                  auto objHi = obj->words + objLo;

                  if((objLo <= lo && lo < objHi) || (objLo < hi && hi < objHi))
                     goto nextValue;

                  if((lo <= objLo && objLo < hi) || (lo < objHi && objHi < hi))
                     goto nextValue;
               }

               for(auto const &itr : GDCC::IR::Space::MapArs)
               {
                  if(lo <= itr.second.value && itr.second.value < hi)
                     goto nextValue;
               }

               break;

            nextValue:;
            }

            self->alloc = false;
         };

         //
         // trSpaceAlloc
         //
         auto trSpaceAlloc = [](GDCC::IR::Space &sp)
         {
            for(auto const &obj : sp.obset)
            {
               if(obj->alloc)
                  obj->allocValue();

               // Back address glyph.
               BackGlyphWord(obj->glyph, obj->value);
            }
         };

         switch(space.space)
         {
         case GDCC::IR::AddrBase::GblReg:
         case GDCC::IR::AddrBase::LocArs:
         case GDCC::IR::AddrBase::WldReg:
            trSpaceAlloc(space);
            break;

         case GDCC::IR::AddrBase::MapReg:
            for(auto const &obj : space.obset)
            {
               if(obj->alloc)
                  allocMapReg(obj);

               // Back address glyph.
               BackGlyphWord(obj->glyph, obj->value);

               if(!obj->defin)
                  ++numChunkMIMP;

               if(obj->defin && numChunkMEXP <= obj->value)
                  numChunkMEXP = obj->value + 1;
            }

            space.allocWords();

            break;

         case GDCC::IR::AddrBase::MapArr:
            // Allocate addresses for any sub-objects.
            trSpaceAlloc(space);

            // Even external arrays need an index.
            if(space.alloc)
               space.allocValue();

            space.allocWords();

            if(space.defin)
            {
               ++numChunkARAY;

               if(numChunkMEXP <= space.value)
                  numChunkMEXP = space.value + 1;
            }
            else
               ++numChunkAIMP;

            // Back address glyph.
            BackGlyphWord(space.glyph, space.value);

            break;

         case GDCC::IR::AddrBase::GblArr:
         case GDCC::IR::AddrBase::WldArr:
            // Allocate addresses for any sub-objects.
            trSpaceAlloc(space);

            // Even external arrays need an index.
            if(space.alloc)
               space.allocValue();

            space.allocWords();

            // Back address glyph.
            BackGlyphWord(space.glyph, space.value);

            break;

         default:
            break;
         }
      }

      //
      // Info::trStr
      //
      void Info::trStr(GDCC::IR::StrEnt &str)
      {
         if(!str.defin) return;

         if(str.alloc)
            str.allocValue();

         // Back address glyph.
         BackGlyphStr(str.glyph, str.valueInt);

         if(numChunkSTRL <= str.valueInt)
            numChunkSTRL = str.valueInt + 1;
      }
   }
}

// EOF

