//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS glyph generation.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Module.hpp"

#include "Core/Exception.hpp"

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
      module.reset(new Module);

      prog->genObjectBySpace();

      // Back asm-func glyphs before generating codes.
      for(auto &fn : prog->rangeFunction())
      {
         if(fn.alloc || fn.ctype != IR::CallType::AsmFunc) continue;

         func = &fn;
         backGlyphFunc(func->glyph, func->valueInt, func->ctype);
         func = nullptr;
      }

      InfoBase::gen();

      for(auto const &import : prog->rangeImport())
         module->chunkLOAD.add(import.glyph);

      for(auto &itr : prog->rangeSpaceGblArs()) genSpaceIniti(itr);
      for(auto &itr : prog->rangeSpaceHubArs()) genSpaceIniti(itr);
      for(auto &itr : prog->rangeSpaceModArs()) genSpaceIniti(itr);

      genSpaceIniti(prog->getSpaceModReg());
      genSpaceIniti(prog->getSpaceSta());

      genIniti();

      // TODO 2024-12-31: ACS0 constraint checks.
   }

   //
   // Info::genCode
   //
   void Info::genCode(Code code)
   {
      genCode(code, ElemArgs{});
   }

   //
   // Info::genCode
   //
   void Info::genCode(Code code, int arg0)
   {
      genCode(code, ElemArgs{ElemArg{static_cast<Core::FastU>(arg0)}});
   }

   //
   // Info::genCode
   //
   void Info::genCode(Code code, unsigned int arg0)
   {
      genCode(code, ElemArgs{ElemArg{arg0}});
   }

   //
   // Info::genCode
   //
   void Info::genCode(Code code, Core::FastU arg0)
   {
      genCode(code, ElemArgs{ElemArg{arg0}});
   }

   //
   // Info::genCode
   //
   void Info::genCode(Code code, Core::FastU arg0, int arg1)
   {
      genCode(code, ElemArgs{ElemArg{arg0}, ElemArg{static_cast<Core::FastU>(arg1)}});
   }

   //
   // Info::genCode
   //
   void Info::genCode(Code code, Core::FastU arg0, Core::FastU arg1)
   {
      genCode(code, ElemArgs{ElemArg{arg0}, ElemArg{arg1}});
   }

   //
   // Info::genCode
   //
   void Info::genCode(Code code, Core::FastU arg0, Core::String arg1)
   {
      genCode(code, ElemArgs{ElemArg{arg0}, ElemArg{getExpGlyph(arg1)}});
   }

   //
   // Info::genCode
   //
   void Info::genCode(Code code, Core::FastU arg0, IR::Exp const *arg1)
   {
      genCode(code, ElemArgs{ElemArg{arg0}, ElemArg{arg1}});
   }

   //
   // Info::genCode
   //
   void Info::genCode(Code code, Core::String arg0)
   {
      genCode(code, ElemArgs{ElemArg{getExpGlyph(arg0)}});
   }

   //
   // Info::genCode
   //
   void Info::genCode(Code code, ElemArg const &arg0)
   {
      genCode(code, ElemArgs{ElemArg{arg0}});
   }

   //
   // Info::genCode
   //
   void Info::genCode(Code code, IR::Exp const *arg0)
   {
      genCode(code, ElemArgs{ElemArg{arg0}});
   }

   //
   // Info::genCode
   //
   void Info::genCode(Code code, IR::Exp const *arg0, IR::Exp const *arg1)
   {
      genCode(code, ElemArgs{ElemArg{arg0}, ElemArg{arg1}});
   }

   //
   // Info::genCode
   //
   void Info::genCode(Code code, ElemArgs &&args)
   {
      Core::FastU cpos = module->chunkCODE.getPos();
      Core::FastU size = 4;

      switch(code)
      {
         // TODO 2024-12-30: Add padding to Jcnd_Lit for word alignment before
         //    implementing any codes with sub-word arguments. (Or ACSe.)

      default:
         size += args.size() * 4;
         break;
      }

      module->chunkCODE.add(cpos, size, static_cast<Core::FastU>(code), std::move(args));
   }

   //
   // Info::genDJump
   //
   void Info::genDJump()
   {
      if(djump->alloc)
         djump->allocValue(getAllocDJump());

      module->chunkJUMP[djump->value] = ElemJUMP{ElemArg{getExpGlyph(djump->label)}};

      backGlyphDJump(djump->glyph, djump->value);
   }

   //
   // Info::genFunc
   //
   void Info::genFunc()
   {
      //
      // getSizes
      //
      auto getSizes = [](IR::Function *func)
      {
         Core::Array<Core::FastU> sizes{Core::Size, func->localArr.size()};
         for(auto i = sizes.size(); i--;)
            sizes[i] = func->localArr[i];
         return sizes;
      };

      // Back label glyph.
      backGlyphWord(func->label, getCodePos());

      // Gen function preamble.
      if(func->defin && func->allocAut)
      {
         genCode(Code::Push_Lit,    func->allocAut);
         genCode(Code::Call_Lit,    "___GDCC__Plsa");
         genCode(Code::Drop_LocReg, getStkPtrIdx());
      }

      Core::FastU paramMax = GetParamMax(func->ctype);
      if(func->defin && func->param > paramMax)
      {
         for(Core::FastU i = paramMax; i != func->param; ++i)
         {
            genCode(Code::Push_Lit,    ~(i - paramMax));
            genCode(Code::Push_GblArr, StaArray);
            genCode(Code::Drop_LocReg, i);
         }
      }

      genBlock(func->block);

      switch(func->ctype)
      {
      case IR::CallType::StdCall:
      case IR::CallType::StkCall:
         if(func->alloc)
            func->allocValue(getAllocFunc(func->ctype));

         module->chunkFUNC[func->valueInt] = ElemFUNC
         {
            ElemArg{func->defin ? &*getExpGlyph(func->label) : nullptr},
            std::min(func->param, GetParamMax(func->ctype)),
            std::max(func->param, func->getLocalReg()),
            !!func->retrn
         };

         if(module->chunkFUNC[func->valueInt].local > 255)
            Core::Error(func->getOrigin(), "too many registers");

         module->chunkFNAM[func->valueInt] = ElemFNAM{func->glyph};

         backGlyphFunc(func->glyph, func->valueInt, func->ctype);

         if(func->defin && !func->localArr.empty())
            module->chunkFARY.add(func->valueInt, getSizes(func));

         break;

      case IR::CallType::SScriptI:
      case IR::CallType::SScriptS:
      case IR::CallType::ScriptI:
      case IR::CallType::ScriptS:
         if(func->defin)
         {
            if(func->alloc)
               func->allocValue(getAllocFunc(func->ctype));

            Core::FastU value = GetScriptValue(func);

            module->chunkSPTR.add(ElemArg{&*getExpGlyph(func->label)}, value,
               GetScriptType(func), std::min(func->param, GetParamMax(func->ctype)));

            if(!func->localArr.empty())
               module->chunkSARY.add(value, getSizes(func));

            if(auto flags = GetScriptFlag(func))
               module->chunkSFLG.add(value, flags);

            if(func->ctype == IR::CallType::SScriptS || func->ctype == IR::CallType::ScriptS)
               module->chunkSNAM[func->valueInt] = ElemSNAM{func->valueStr};

            // TODO 2024-12-25: The implicit register count should be an option.
            if(func->getLocalReg() > 20)
               module->chunkSVCT.add(value, func->getLocalReg());

            if(func->getLocalReg() > 65535)
               Core::Error(func->getOrigin(), "too many registers");
         }

         if(func->ctype == IR::CallType::SScriptS || func->ctype == IR::CallType::ScriptS)
         {
            if(auto s = prog->findStrEntVal(func->valueStr))
               backGlyphGlyph(func->glyph, s->glyph);
         }
         else
         {
            if(!func->alloc)
               backGlyphWord(func->glyph, func->valueInt);
         }

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

         // TODO 2024-12-25: This needs to account for multi-word objects.
         if(obj->defin)
            module->chunkMEXP[obj->value] = obj->glyph;
         else
            module->chunkMIMP.add(obj->glyph, obj->value);

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

      if(obj->initi)
         init[prog->findSpace(obj->space)].used = true;
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

      module->chunkSTRL[strent->valueInt] = ElemSTRL{strent->valueStr};
   }
}

// EOF

