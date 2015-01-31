//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR Space glyph generation.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "IR/CallType.hpp"
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
            switch(space->space.base)
            {
            case IR::AddrBase::Sta:
               space->value = StaArray;
               backGlyphWord("___GDCC__Sta", space->value);
               break;

            case IR::AddrBase::LocArr:
               // Back address glyph.
               if(!space->alloc)
                  backGlyphWord(space->glyph, space->value);
               break;

            case IR::AddrBase::MapArr:
               // Even external arrays need an index.
               if(space->alloc)
               {
                  space->allocValue(*prog, [](IR::Program &prog, IR::Space &space)
                  {
                     for(auto const &itr : prog.rangeObject())
                     {
                        if(itr.space.base != IR::AddrBase::MapReg) continue;
                        if(itr.alloc) continue;

                        auto lo = itr.value;
                        auto hi = itr.words + lo;

                        if(lo <= space.value && space.value < hi)
                           return true;
                     }

                     return false;
                  });
               }

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
               // Even external arrays need an index.
               if(space->alloc)
                  space->allocValue(*prog, [](IR::Program &, IR::Space &space)
                     {return space.value == StaArray;});

               // Back address glyph.
               backGlyphWord(space->glyph, space->value);

               break;

            case IR::AddrBase::WldArr:
               // Even external arrays need an index.
               if(space->alloc)
                  space->allocValue(*prog);

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
            for(auto const &itr : prog->rangeObject())
            {
               if(!itr.defin || itr.space != space->space || !itr.initi)
                  continue;

               auto idx = itr.value;

               genSpaceInitiValue(ini, idx, itr.initi->getValue());

               if(ini.max < idx)
                  ini.max = idx;
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
            Core::FastU bits;

            switch(val.v)
            {
            case IR::ValueBase::Array:
               for(auto const &v : val.vArray.value)
                  genSpaceInitiValue(ini, itr, v);
               break;

            case IR::ValueBase::Assoc:
               for(auto const &v : val.vAssoc.value)
                  genSpaceInitiValue(ini, itr, v);
               break;

            case IR::ValueBase::Fixed:
               bits = val.vFixed.vtype.bitsI + val.vFixed.vtype.bitsF + val.vFixed.vtype.bitsS;
               for(Core::FastU w = 0, e = (bits + 31) / 32; w != e; ++w)
               {
                  iv = &ini.vals[itr++];
                  iv->tag = InitTag::Fixed;
                  iv->val = Core::NumberCast<Core::FastU>(val.vFixed.value >> (w * 32));
               }
               break;

            case IR::ValueBase::Float:
               bits = val.vFloat.vtype.bitsI + val.vFloat.vtype.bitsF + val.vFloat.vtype.bitsS;
               for(Core::FastU w = 0, e = (bits + 31) / 32; w != e; ++w)
               {
                  iv = &ini.vals[itr++];
                  iv->tag = InitTag::Fixed;
                  iv->val = GetWord_Float(val.vFloat, w);
               }
               break;

            case IR::ValueBase::Funct:
               iv = &ini.vals[itr++];
               if(IsScriptS(val.vFunct.vtype.callT))
               {
                  if((val.vFunct.value & 0xFFFFFFFF) == 0xFFFFFFFF)
                     iv->tag = InitTag::Fixed;
                  else
                     iv->tag = InitTag::StrEn;
               }
               else
               {
                  if((val.vFunct.value & 0xFFFFFFFF) == 0)
                     iv->tag = InitTag::Fixed;
                  else
                     iv->tag = InitTag::Funct;
               }
               iv->val = val.vFunct.value;
               break;

            case IR::ValueBase::Point:
               iv = &ini.vals[itr++];
               iv->tag = InitTag::Fixed;
               iv->val = val.vPoint.value;
               break;

            case IR::ValueBase::StrEn:
               iv = &ini.vals[itr++];
               if((val.vFunct.value & 0xFFFFFFFF) == 0xFFFFFFFF)
                  iv->tag = InitTag::Fixed;
               else
                  iv->tag = InitTag::StrEn;
               iv->val = val.vStrEn.value;
               break;

            case IR::ValueBase::Tuple:
               for(auto const &v : val.vTuple.value)
                  genSpaceInitiValue(ini, itr, v);
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

