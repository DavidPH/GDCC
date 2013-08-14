//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS IR Space translation.
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
      // Info::trSpaceIniti
      //
      void Info::trSpaceIniti(GDCC::IR::Space &space)
      {
         GDCC::FastU size = 0;

         for(auto const &obj : space.obset) if(obj->defin)
         {
            auto max = obj->value + obj->words;
            if(size < max) size = max;
         }

         if(!size) return;

         auto &ini = init[&space];
         ini.vals = GDCC::Array<InitVal>(size);

         // Generate init data.
         for(auto const &obj : space.obset) if(obj->defin)
         {
            auto data = &ini.vals[obj->value];

            // If the object has an initializer, use it.
            if(obj->initi)
               trSpaceInitiValue(data, ini.vals.end(), obj->initi->getValue());
            // Otherwise, mark the space as numeric.
            else
               for(auto i = obj->words; i--;) data++->tag = InitTag::Fixed;
         }

         if(space.space == GDCC::IR::AddrBase::MapArr)
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
         else if(space.space == GDCC::IR::AddrBase::MapReg)
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
      // Info::trSpaceInitiValue
      //
      void Info::trSpaceInitiValue(InitVal *&data, InitVal const *end,
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
               trSpaceInitiValue(data, end, v);
            break;

         case GDCC::IR::ValueBase::StrEn:
            data->tag = InitTag::StrEn;
            data->val = val.vStrEn.value;
            break;

         default:
            std::cerr << "bad trSpaceInitiValue\n";
            throw EXIT_FAILURE;
         }
      }
   }
}

// EOF

