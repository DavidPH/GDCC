//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2022 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS glyph generation.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "IR/Program.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::ZDACS
{
   //
   // Info::gen
   //
   void Info::gen()
   {
      prog->genObjectBySpace();

      InfoBase::gen();

      for(auto &itr : prog->rangeSpaceGblArs()) genSpaceIniti(itr);
      for(auto &itr : prog->rangeSpaceHubArs()) genSpaceIniti(itr);
      for(auto &itr : prog->rangeSpaceModArs()) genSpaceIniti(itr);

      genSpaceIniti(prog->getSpaceModReg());
      genSpaceIniti(prog->getSpaceSta());

      genIniti();
   }

   //
   // Info::genDJump
   //
   void Info::genDJump()
   {
      if(djump->alloc)
         djump->allocValue(getAllocDJump());

      if(numChunkJUMP <= djump->value)
         numChunkJUMP = djump->value + 1;

      backGlyphDJump(djump->glyph, djump->value);
   }

   //
   // Info::genFunc
   //
   void Info::genFunc()
   {
      //
      // isScriptFlag
      //
      auto isScriptFlag = [](Core::Array<Core::String> const &stype)
      {
         for(auto const &st : stype)
         {
            if(auto flag = ScriptFlags.find(st)) if(*flag)
               return true;

            if(st == Core::STR_clientside || st == Core::STR_net)
               return true;
         }

         return false;
      };

      // Back label glyph.
      backGlyphWord(func->label, CodeBase() + numChunkCODE);

      // Gen function preamble.
      if(func->allocAut)
         numChunkCODE += 24;

      Core::FastU paramMax = GetParamMax(func->ctype);
      if(func->defin && func->param > paramMax)
         numChunkCODE += (func->param - paramMax) * 24;

      genBlock(func->block);

      switch(func->ctype)
      {
      case IR::CallType::StdCall:
      case IR::CallType::StkCall:
         if(func->alloc)
            func->allocValue(getAllocFunc(func->ctype));

         if(numChunkFUNC <= func->valueInt)
            numChunkFUNC = func->valueInt + 1;

         if(numChunkFNAM <= func->valueInt)
            numChunkFNAM = func->valueInt + 1;

         backGlyphFunc(func->glyph, func->valueInt, func->ctype);

         break;

      case IR::CallType::SScriptI:
      case IR::CallType::ScriptI:
         if(func->defin)
         {
            if(func->alloc)
               func->allocValue(getAllocFunc(func->ctype));

            ++numChunkSPTR;

            if(isScriptFlag(func->stype)) ++numChunkSFLG;
            if(func->getLocalReg() > 20)  ++numChunkSVCT;
         }

         if(!func->alloc)
            backGlyphWord(func->glyph, func->valueInt);

         break;

      case IR::CallType::SScriptS:
      case IR::CallType::ScriptS:
         if(func->defin)
         {
            if(func->alloc)
               func->allocValue(getAllocFunc(func->ctype));

            ++numChunkSPTR;

            if(isScriptFlag(func->stype)) ++numChunkSFLG;
            if(func->getLocalReg() > 20)  ++numChunkSVCT;

            if(numChunkSNAM <= func->valueInt)
               numChunkSNAM = func->valueInt + 1;
         }

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
      switch(obj->space.base)
      {
      case IR::AddrBase::GblArr:
      case IR::AddrBase::GblReg:
      case IR::AddrBase::HubArr:
      case IR::AddrBase::HubReg:
      case IR::AddrBase::Sta:
         if(obj->alloc)
            obj->allocValue(getAllocObj(obj->space));
         break;

      case IR::AddrBase::ModArr:
         if(obj->alloc)
            obj->allocValue(getAllocObj(obj->space));

         if(obj->defin)
            spaceUsed[prog->findSpace(obj->space)] = true;

         break;

      case IR::AddrBase::ModReg:
         if(obj->alloc)
         {
            obj->allocValue(getAllocObj(obj->space));
            getAllocSpace(IR::AddrBase::ModArr).allocAt(obj->words, obj->value);
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
   // Info::genStrEnt
   //
   void Info::genStrEnt()
   {
      if(!strent->defin) return;

      if(strent->alloc)
         strent->allocValue(*prog, getAllocStrEnt());

      // Back address glyph.
      backGlyphStrEnt(strent->glyph, strent->valueInt);

      if(numChunkSTRL <= strent->valueInt)
         numChunkSTRL = strent->valueInt + 1;
   }
}

// EOF

