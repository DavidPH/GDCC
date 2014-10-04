//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR Space glyph generation.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "IR/Exp.hpp"
#include "IR/Program.hpp"

#include <iostream>


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
         // Info::genSpace
         //
         void Info::genSpace()
         {
            //
            // allocMapReg
            //
            // Handles the unusual rules for allocating map registers.
            //
            auto allocMapReg = [this](IR::Object &self)
            {
               for(;; ++self.value)
               {
                  auto lo = self.value;
                  auto hi = self.words + lo;

                  for(auto const &obj : prog->rangeObject())
                  {
                     if(obj.alloc || &obj == &self || obj.space != self.space)
                        continue;

                     auto objLo = obj.value;
                     auto objHi = obj.words + objLo;

                     if((objLo <= lo && lo < objHi) || (objLo < hi && hi < objHi))
                        goto nextValue;

                     if((lo <= objLo && objLo < hi) || (lo < objHi && objHi < hi))
                        goto nextValue;
                  }

                  for(auto const &itr : prog->rangeSpaceMapArs())
                  {
                     if(lo <= itr.value && itr.value < hi)
                        goto nextValue;
                  }

                  break;

               nextValue:;
               }

               self.alloc = false;
            };

            //
            // genSpaceAlloc
            //
            auto genSpaceAlloc = [this]()
            {
               for(auto &obj : prog->rangeObject())
               {
                  if(obj.space != space->space)
                     continue;

                  if(obj.alloc)
                     obj.allocValue(*prog);

                  // Back address glyph.
                  backGlyphObj(obj.glyph, obj.value);
               }
            };

            switch(space->space.base)
            {
            case IR::AddrBase::LocArs:
               space->value = LocArsArray;

               space->allocWords(*prog);
               break;

            case IR::AddrBase::MapReg:
               for(auto &obj : prog->rangeObject())
               {
                  if(obj.space.base != IR::AddrBase::MapReg)
                     continue;

                  if(obj.alloc)
                     allocMapReg(obj);

                  // Back address glyph.
                  backGlyphObj(obj.glyph, obj.value);

                  if(!obj.defin)
                     ++numChunkMIMP;

                  if(obj.defin && numChunkMEXP <= obj.value)
                     numChunkMEXP = obj.value + 1;
               }

               space->allocWords(*prog);

               break;

            case IR::AddrBase::MapArr:
               // Allocate addresses for any sub-objects.
               genSpaceAlloc();

               // Even external arrays need an index.
               if(space->alloc)
                  space->allocValue(*prog);

               space->allocWords(*prog);

               if(space->defin)
               {
                  ++numChunkARAY;

                  if(numChunkMEXP <= space->value)
                     numChunkMEXP = space->value + 1;
               }
               else
                  ++numChunkAIMP;

               // Back address glyph.
               backGlyphWord(space->glyph, space->value);

               break;

            case IR::AddrBase::GblArr:
            case IR::AddrBase::WldArr:
               // Allocate addresses for any sub-objects.
               genSpaceAlloc();

               // Even external arrays need an index.
               if(space->alloc)
                  space->allocValue(*prog);

               space->allocWords(*prog);

               // Back address glyph.
               backGlyphWord(space->glyph, space->value);

               break;

            default:
               break;
            }
         }

         //
         // Info::genSpaceIniti
         //
         void Info::genSpaceIniti()
         {
            auto &ini = init[space];

            // Generate init data.
            for(auto const &obj : prog->rangeObject())
            {
               if(!obj.defin || obj.space != space->space || !obj.initi)
                  continue;

               auto itr = obj.value;

               genSpaceInitiValue(ini, itr, obj.initi->getValue());

               if(ini.max < itr)
                  ini.max = itr;
            }

            // If no initializers needed, skip remaining parts.
            if(ini.vals.empty()) return;

            if(space->space.base == IR::AddrBase::MapArr)
            {
               ++numChunkAINI;

               for(auto const &i : ini.vals) switch(i.second.tag)
               {
               case InitTag::Empty:
                  break;

               case InitTag::Fixed:
                  ini.onlyStr = false;
                  break;

               case InitTag::Funct:
                  ini.needTag = true;
                  ini.onlyStr = false;
                  break;

               case InitTag::StrEn:
                  ini.needTag = true;
                  break;
               }

               if(ini.needTag)
               {
                  if(ini.onlyStr)
                     ++numChunkASTR;
                  else
                     ++numChunkATAG;
               }
            }
            else if(space->space.base == IR::AddrBase::MapReg)
            {
               for(auto const &i : ini.vals)
               {
                  if(i.second.val) ++numChunkMINI;

                  if(i.second.tag == InitTag::StrEn)
                     ++numChunkMSTR;
               }
            }
         }

         //
         // Info::genSpaceIniti
         //
         void Info::genSpaceIniti(IR::Space &space_)
         {
            try
            {
               space = &space_;
               genSpaceIniti();
               space = nullptr;
            }
            catch(...)
            {
               space = nullptr;
               throw;
            }
         }

         //
         // Info::genSpaceInitiValue
         //
         void Info::genSpaceInitiValue(InitData &ini, Core::FastU &itr, IR::Value const &val)
         {
            InitVal *iv;

            switch(val.v)
            {
            case IR::ValueBase::Fixed:
               iv = &ini.vals[itr++];
               iv->tag = InitTag::Fixed;
               iv->val = Core::NumberCast<Core::FastU>(val.vFixed.value);
               break;

            case IR::ValueBase::Funct:
               iv = &ini.vals[itr++];
               iv->tag = InitTag::Funct;
               iv->val = val.vStrEn.value;
               break;

            case IR::ValueBase::Multi:
               for(auto const &v : val.vMulti.value)
                  genSpaceInitiValue(ini, itr, v);
               break;

            case IR::ValueBase::StrEn:
               iv = &ini.vals[itr++];
               iv->tag = InitTag::StrEn;
               iv->val = val.vStrEn.value;
               break;

            default:
               std::cerr << "bad genSpaceInitiValue\n";
               throw EXIT_FAILURE;
            }
         }
      }
   }
}

// EOF

