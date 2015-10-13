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

#include "Core/Exception.hpp"

#include "IR/CallType.hpp"
#include "IR/Exp.hpp"
#include "IR/Program.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
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

            case IR::AddrBase::GblArr:
            case IR::AddrBase::HubArr:
               // Even external arrays need an index.
               if(space->alloc)
                  space->allocValue(getAllocSpace(space->space.base));

               // Back address glyph.
               backGlyphWord(space->glyph, space->value);

               break;

            case IR::AddrBase::LocArr:
               // Back address glyph.
               if(!space->alloc)
                  backGlyphWord(space->glyph, space->value);
               break;

            case IR::AddrBase::ModArr:
               // Even external arrays need an index.
               if(space->alloc)
               {
                  space->allocValue(getAllocSpace(space->space.base));
                  getAllocObj({IR::AddrBase::ModReg, Core::STR_}).allocAt(1, space->value);
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

               try
               {
                  genSpaceInitiValue(ini, idx, itr.initi->getValue());
               }
               catch(Core::Exception &e)
               {
                  e.setOrigin(itr.initi->pos);
                  throw;
               }

               if(ini.max < idx)
                  ini.max = idx;
            }

            // If no initializers needed, skip remaining parts.
            if(ini.vals.empty()) return;

            if(space->space.base == IR::AddrBase::ModArr)
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
            else if(space->space.base == IR::AddrBase::ModReg)
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

            case IR::ValueBase::DJump:
               iv = &ini.vals[itr++];
               iv->tag = InitTag::Fixed;
               iv->val = val.vDJump.value;
               break;

            case IR::ValueBase::Empty:
               throw Core::ExceptStr({nullptr}, "bad genSpaceInitiValue: Empty");

            case IR::ValueBase::Fixed:
               bits = val.vFixed.vtype.bitsI + val.vFixed.vtype.bitsF + val.vFixed.vtype.bitsS;
               for(Core::FastU w = 0, e = (bits + 31) / 32; w != e; ++w)
               {
                  iv = &ini.vals[itr++];
                  iv->tag = InitTag::Fixed;
                  iv->val = GetWord_Fixed(val.vFixed, w);
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
               if(!IsNull(val.vFunct))
               {
                  if(IsScriptS(val.vFunct.vtype.callT))
                     iv->tag = InitTag::StrEn;
                  else
                     iv->tag = InitTag::Funct;
               }
               else
                  iv->tag = InitTag::Fixed;
               iv->val = val.vFunct.value;
               break;

            case IR::ValueBase::Point:
               iv = &ini.vals[itr++];
               iv->tag = InitTag::Fixed;
               iv->val = val.vPoint.value;
               break;

            case IR::ValueBase::StrEn:
               iv = &ini.vals[itr++];
               if(IsNull(val.vStrEn))
                  iv->tag = InitTag::Fixed;
               else
                  iv->tag = InitTag::StrEn;
               iv->val = val.vStrEn.value;
               break;

            case IR::ValueBase::Tuple:
               for(auto const &v : val.vTuple.value)
                  genSpaceInitiValue(ini, itr, v);
               break;

            case IR::ValueBase::Union:
               {
                  Core::FastU next = itr + getSpaceInitiSize(val.vUnion.vtype);
                  genSpaceInitiValue(ini, itr, *val.vUnion.value);
                  itr = next;
               }
               break;
            }
         }

         //
         // Info::getSpaceInitiSize
         //
         Core::FastU Info::getSpaceInitiSize(IR::Type const &type)
         {
            Core::FastU n;

            switch(type.t)
            {
            case IR::TypeBase::Array:
               return getSpaceInitiSize(*type.tArray.elemT) * type.tArray.elemC;

            case IR::TypeBase::Assoc:
               n = 0;
               for(auto const &t : type.tAssoc.assoc)
                  n = std::max(n, getSpaceInitiSize(t.type) + t.addr);
               return n;

            case IR::TypeBase::DJump:
               return 1;

            case IR::TypeBase::Empty:
               return 0;

            case IR::TypeBase::Fixed:
               return (type.tFixed.bitsI + type.tFixed.bitsF + type.tFixed.bitsS + 31) / 32;

            case IR::TypeBase::Float:
               return (type.tFixed.bitsI + type.tFixed.bitsF + type.tFixed.bitsS + 31) / 32;

            case IR::TypeBase::Funct:
               return 1;

            case IR::TypeBase::Point:
               return 1;

            case IR::TypeBase::StrEn:
               return 1;

            case IR::TypeBase::Tuple:
               n = 0;
               for(auto const &v : type.tTuple.types)
                  n += getSpaceInitiSize(v);
               return n;

            case IR::TypeBase::Union:
               n = 0;
               for(auto const &t : type.tUnion.types)
                  n = std::max(n, getSpaceInitiSize(t));
               return n;
            }
         }
      }
   }
}

// EOF

