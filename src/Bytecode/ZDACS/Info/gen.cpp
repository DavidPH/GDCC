//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS glyph generation.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "IR/CallType.hpp"
#include "IR/Program.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
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

            for(auto &itr : prog->rangeSpaceGblArs()) genSpaceIniti(itr);
            for(auto &itr : prog->rangeSpaceMapArs()) genSpaceIniti(itr);
            for(auto &itr : prog->rangeSpaceWldArs()) genSpaceIniti(itr);

            genSpaceIniti(prog->getSpaceLocArs());
            genSpaceIniti(prog->getSpaceMapReg());

            genIniti();
         }

         //
         // Info::genFunc
         //
         void Info::genFunc()
         {
            static IR::CallType const FuncTypes[] =
            {
               IR::CallType::StdCall,
               IR::CallType::StkCall,
            };

            // Back label glyph.
            backGlyphWord(func->label, CodeBase() + numChunkCODE);

            // Gen function preamble.
            switch(func->ctype)
            {
            case IR::CallType::ScriptI:
            case IR::CallType::ScriptS:
               if(!func->defin) break;
               if(func->param <= 4) break;

               numChunkCODE += (func->param - 4) * 24;
               break;

            default:
               break;
            }

            genBlock(func->block);

            switch(func->ctype)
            {
            case IR::CallType::StdCall:
            case IR::CallType::StkCall:
               if(func->alloc)
                  func->allocValue(*prog, FuncTypes);

               if(numChunkFUNC <= func->valueInt)
                  numChunkFUNC = func->valueInt + 1;

               if(numChunkFNAM <= func->valueInt)
                  numChunkFNAM = func->valueInt + 1;

               backGlyphFunc(func->glyph, func->valueInt, func->ctype);

               break;

            case IR::CallType::ScriptI:
               if(!func->defin) break;

               if(func->alloc)
                  func->allocValue(*prog, IR::CallType::ScriptI);

               ++numChunkSPTR;

               if(func->sflagClS || func->sflagNet) ++numChunkSFLG;
               if(func->getLocalReg() > 20) ++numChunkSVCT;

               backGlyphWord(func->glyph, func->valueInt);

               break;

            case IR::CallType::ScriptS:
               if(!func->defin) break;

               if(func->alloc)
                  func->allocValue(*prog, IR::CallType::ScriptS);

               ++numChunkSPTR;

               if(func->sflagClS || func->sflagNet) ++numChunkSFLG;
               if(func->getLocalReg() > 20) ++numChunkSVCT;

               if(numChunkSNAM <= func->valueInt)
                  numChunkSNAM = func->valueInt + 1;

               if(auto s = prog->findStrEntVal(func->valueStr))
                  backGlyphGlyph(func->glyph, s->glyph);

               break;

            default:
               // Back glyph if already allocated.
               if(!func->alloc)
                  backGlyphFunc(func->glyph, func->valueInt, func->ctype);

               break;
            }
         }

         //
         // Info::genObj
         //
         void Info::genObj()
         {
            // HUGE HACK: Set null address space name to empty string.
            // A proper fix will be to normalize usage at higher levels.
            if(!obj->space.name)
               obj->space.name = Core::STR_;

            switch(obj->space.base)
            {
            case IR::AddrBase::GblArr:
            case IR::AddrBase::GblReg:
            case IR::AddrBase::LocArs:
            case IR::AddrBase::MapArr:
            case IR::AddrBase::WldArr:
            case IR::AddrBase::WldReg:
               if(obj->alloc)
                  obj->allocValue(*prog);
               break;

            case IR::AddrBase::MapReg:
               if(obj->alloc)
               {
                  obj->allocValue(*prog, [](IR::Program &prog, IR::Object &obj)
                  {
                     auto lo = obj.value;
                     auto hi = obj.words + lo;

                     for(auto const &itr : prog.rangeSpaceMapArs())
                     {
                        if(lo <= itr.value && itr.value < hi)
                           return true;
                     }

                     return false;
                  });
               }

               if(!obj->defin)
                  ++numChunkMIMP;

               if(obj->defin && numChunkMEXP <= obj->value)
                  numChunkMEXP = obj->value + 1;

               break;

            default:
               break;
            }

            // Generic actions to take for allocated objects.
            if(!obj->alloc)
            {
               // Back address glyph.
               backGlyphObj(obj->glyph, obj->value);

               // Recheck space size.
               auto words = obj->value + obj->words;
               if(auto sp = prog->findSpace(obj->space))
                  if(sp->words < words) sp->words = words;
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
}

// EOF

