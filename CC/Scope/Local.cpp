//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2023 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C local scope handling.
//
//-----------------------------------------------------------------------------

#include "CC/Scope/Local.hpp"

#include "CC/Scope/Case.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Scope/Global.hpp"
#include "CC/Scope/Loop.hpp"

#include "Core/Exception.hpp"

#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"

#include "SR/Attribute.hpp"
#include "SR/Function.hpp"
#include "SR/Object.hpp"
#include "SR/Space.hpp"
#include "SR/Storage.hpp"
#include "SR/Type.hpp"

#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Scope_Local::AllocAutoInfo::setMax
   //
   void Scope_Local::AllocAutoInfo::setMax(AllocAutoInfo const &alloc)
   {
      for(auto const &itr : alloc.spaceMap)
         spaceMap[itr.first] = std::max(spaceMap[itr.first], itr.second);

      localArr = std::max(localArr, alloc.localArr);
      localAut = std::max(localAut, alloc.localAut);
      localReg = std::max(localReg, alloc.localReg);
   }

   //
   // Scope_Local constructor
   //
   Scope_Local::Scope_Local(Scope_Global &global_, Scope_Function &fn_) :
      Scope{&global_},

      fn    (fn_),
      global(global_),
      label {Core::STRNULL}
   {
   }

   //
   // Scope_Local constructor
   //
   Scope_Local::Scope_Local(Scope_Local &parent_) :
      Scope{&parent_},

      fn    (parent_.fn),
      global(parent_.global),
      label {Core::STRNULL}
   {
   }

   //
   // Scope_Local destructor
   //
   Scope_Local::~Scope_Local()
   {
   }

   //
   // Scope_Local::allocAutoObj
   //
   void Scope_Local::allocAutoObj(AllocAutoInfo &alloc, SR::Object *obj)
   {
      // This function only applies to automatic storage objects.
      if(obj->store != SR::Storage::Auto)
         return;

      IR::Type_Fixed idxType{Target::GetWordBits(), 0, 0, 0};

      auto objSpace = obj->type->getQualAddr();

      // If one is not specified, select an address space now.
      if(objSpace.base == IR::AddrBase::Gen)
      {
         if(obj->refer)
            objSpace.base = IR::AddrBase::Aut;
         else
            objSpace.base = IR::AddrBase::LocReg;

         obj->type = obj->type->getTypeArrayQualAddr(objSpace);
      }

      // Select type of local to allocate.
      Core::FastU *idx;
      switch(objSpace.base)
      {
      case IR::AddrBase::Aut:    idx = &alloc.localAut; break;
      case IR::AddrBase::LocReg: idx = &alloc.localReg; break;

      case IR::AddrBase::LocArr:
         idx = &alloc.spaceMap[getSpaceValue(objSpace.name)];
         break;

      default: return; // Any other address space is an error.
      }

      Core::FastU value = *idx * Target::GetWordBytes() / Target::GetWordShift();

      // Set object's value (index/address).
      obj->value = IR::ExpCreate_Value(IR::Value_Fixed(
         Core::NumberCast<Core::Integ>(value), idxType), {});

      // Update allocation info.
      *idx += obj->type->getSizeWords();
   }

   //
   // Scope_Local::allocAutoSpace
   //
   void Scope_Local::allocAutoSpace(AllocAutoInfo &alloc, SR::Space *space)
   {
      IR::Type_Fixed idxType{Target::GetWordBits(), 0, 0, 0};

      // Set space's value (index/address).
      space->value = IR::ExpCreate_Value(
         IR::Value_Fixed(Core::NumberCast<Core::Integ>(alloc.localArr), idxType), {});

      // Update allocation info.
      alloc.localArr += 1;
   }

   //
   // Scope_Local::createScopeBlock
   //
   Scope_Block &Scope_Local::createScopeBlock()
   {
      auto ctx = new Scope_Block(*this);
      subScopes.emplace_back(ctx);
      return *ctx;
   }

   //
   // Scope_Local::createScopeLoop
   //
   Scope_Block &Scope_Local::createScopeLoop()
   {
      auto ctx = new Scope_Loop(*this);
      subScopes.emplace_back(ctx);
      return *ctx;
   }

   //
   // Scope_Local::createScopeCase
   //
   Scope_Case &Scope_Local::createScopeCase()
   {
      auto ctx = new Scope_Case(*this);
      subScopes.emplace_back(ctx);
      return *ctx;
   }

   //
   // Scope_Local::genGlyphObj
   //
   Core::String Scope_Local::genGlyphObj(Core::String name, IR::Linkage linka)
   {
      // Actual linkages must go through global.
      if(linka != IR::Linkage::None)
         return global.genGlyphObj(name, linka);

      // Anonymous objects with no linkage need a fully generated name.
      if(!name) return fn.fn->genLabel();

      // Otherwise, base it on the scope's prefix (generating if needed).
      if(!label) label = fn.fn->genLabel();
      return label + name;
   }

   //
   // Scope_Local::genIR
   //
   void Scope_Local::genIR(IR::Program &prog)
   {
      for(auto &ctx : subScopes)
         ctx->genIR(prog);

      for(auto itr : localObj)
         itr.second->genObject(prog);

      for(auto itr : localSpace)
         itr.second->genSpace(prog);
   }

   //
   // Scope_Local::getLabel
   //
   Core::String Scope_Local::getLabel(Core::String name, bool define)
   {
      return fn.getLabel(name, define);
   }

   //
   // Scope_Local::getLabelBreak
   //
   Core::String Scope_Local::getLabelBreak()
   {
      return static_cast<Scope_Local *>(parent)->getLabelBreak();
   }

   //
   // Scope_Local::getLabelCase
   //
   Core::String Scope_Local::getLabelCase(Core::Integ const &n, bool define)
   {
      return static_cast<Scope_Local *>(parent)->getLabelCase(n, define);
   }

   //
   // Scope_Local::getLabelContinue
   //
   Core::String Scope_Local::getLabelContinue()
   {
      return static_cast<Scope_Local *>(parent)->getLabelContinue();
   }

   //
   // Scope_Local::getLabelDefault
   //
   Core::String Scope_Local::getLabelDefault(bool define)
   {
      return static_cast<Scope_Local *>(parent)->getLabelDefault(define);
   }

   //
   // Scope_Local::getObject
   //
   SR::Object::Ref Scope_Local::getObject(SR::Attribute const &attr)
   {
      if(attr.storeExt)
         return global.getObject(attr);

      auto glyph = genGlyphObj(attr.name, attr.linka);
      auto name  = attr.name ? attr.name : glyph;

      if(localObj.find(name) != localObj.end())
         Core::Error(attr.namePos, "local object redefined");

      auto obj = SR::Object::Create(attr.name, glyph);

      obj->linka    = attr.linka;
      obj->type     = attr.type;
      obj->warnUse  = attr.warnUse;

      if(attr.storeInt)
         obj->store = SR::Storage::Static;
      else
         obj->store = SR::Storage::Auto;

      // If declaration is explicitly auto, always make it addressable.
      obj->refer = attr.storeAuto;

      // If register is used, do not allow taking address.
      obj->noRef = attr.storeReg;

      localObj.emplace(attr.name ? attr.name : glyph, obj);

      return obj;
   }

   //
   // Scope_Local::getScopeFirst
   //
   Scope_Block &Scope_Local::getScopeFirst()
   {
      if(subScopes.empty())
         createScopeBlock();

      return *subScopes.front();
   }

   //
   // Scope_Local::getSpace
   //
   SR::Space::Ref Scope_Local::getSpace(SR::Attribute const &attr)
   {
      if(attr.storeExt)
         return global.getSpace(attr);

      auto glyph = genGlyphObj(attr.name, attr.linka);
      auto space = SR::Space::Create(attr.name, glyph);

      space->linka = attr.linka;
      space->space = attr.space.base;

      localSpace.emplace(attr.name ? attr.name : glyph, space);

      return space;
   }

   //
   // Scope_Local::getSpaceValue
   //
   Core::FastU Scope_Local::getSpaceValue(Core::String glyph) const
   {
      for(auto const &itr : localSpace)
      {
         if(itr.second->glyph == glyph)
         {
            if(itr.second->value)
               return itr.second->value->getValue().getFastU();

            return 0;
         }
      }

      if(auto *scope = dynamic_cast<Scope_Local *>(parent))
         return scope->getSpaceValue(glyph);

      return 0;
   }
}

// EOF

