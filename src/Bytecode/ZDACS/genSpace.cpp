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

#include "Info.hpp"

#include "GDCC/IR/Exp.hpp"
#include "GDCC/IR/Object.hpp"

#include <iostream>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

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
         // genSpaceAlloc
         //
         auto genSpaceAlloc = [this]()
         {
            for(auto const &obj : space->obset)
            {
               if(obj->alloc)
                  obj->allocValue();

               // Back address glyph.
               BackGlyphWord(obj->glyph, obj->value);
            }
         };

         switch(space->space)
         {
         case GDCC::IR::AddrBase::GblReg:
         case GDCC::IR::AddrBase::LocArs:
         case GDCC::IR::AddrBase::WldReg:
            genSpaceAlloc();
            break;

         case GDCC::IR::AddrBase::MapReg:
            for(auto const &obj : space->obset)
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

            space->allocWords();

            break;

         case GDCC::IR::AddrBase::MapArr:
            // Allocate addresses for any sub-objects.
            genSpaceAlloc();

            // Even external arrays need an index.
            if(space->alloc)
               space->allocValue();

            space->allocWords();

            if(space->defin)
            {
               ++numChunkARAY;

               if(numChunkMEXP <= space->value)
                  numChunkMEXP = space->value + 1;
            }
            else
               ++numChunkAIMP;

            // Back address glyph.
            BackGlyphWord(space->glyph, space->value);

            break;

         case GDCC::IR::AddrBase::GblArr:
         case GDCC::IR::AddrBase::WldArr:
            // Allocate addresses for any sub-objects.
            genSpaceAlloc();

            // Even external arrays need an index.
            if(space->alloc)
               space->allocValue();

            space->allocWords();

            // Back address glyph.
            BackGlyphWord(space->glyph, space->value);

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
         GDCC::FastU size = 0;

         for(auto const &obj : space->obset) if(obj->defin)
         {
            auto max = obj->value + obj->words;
            if(size < max) size = max;
         }

         if(!size) return;

         auto &ini = init[space];
         ini.vals = GDCC::Array<InitVal>(size);

         // Generate init data.
         for(auto const &obj : space->obset) if(obj->defin)
         {
            auto data = &ini.vals[obj->value];

            // If the object has an initializer, use it.
            if(obj->initi)
               genSpaceInitiValue(data, ini.vals.end(), obj->initi->getValue());
            // Otherwise, mark the space as numeric.
            else
               for(auto i = obj->words; i--;) data++->tag = InitTag::Fixed;
         }

         if(space->space == GDCC::IR::AddrBase::MapArr)
         {
            ++numChunkAINI;

            for(auto const i : ini.vals) switch(i.tag)
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
         else if(space->space == GDCC::IR::AddrBase::MapReg)
         {
            for(auto const i : ini.vals)
            {
               if(i.val) ++numChunkMINI;

               if(i.tag == InitTag::StrEn)
                  ++numChunkMSTR;
            }
         }
      }

      //
      // Info::genSpaceIniti
      //
      void Info::genSpaceIniti(GDCC::IR::Space &space_)
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
      void Info::genSpaceInitiValue(InitVal *&data, InitVal const *end,
         GDCC::IR::Value const &val)
      {
         if(data == end) return;

         switch(val.v)
         {
         case GDCC::IR::ValueBase::Fixed:
            data->tag = InitTag::Fixed;
            data->val = number_cast<GDCC::FastU>(val.vFixed.value);
            break;

         case GDCC::IR::ValueBase::Funct:
            data->tag = InitTag::Funct;
            data->val = val.vStrEn.value;
            break;

         case GDCC::IR::ValueBase::Multi:
            for(auto const &v : val.vMulti.value)
               genSpaceInitiValue(data, end, v);
            break;

         case GDCC::IR::ValueBase::StrEn:
            data->tag = InitTag::StrEn;
            data->val = val.vStrEn.value;
            break;

         default:
            std::cerr << "bad genSpaceInitiValue\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

