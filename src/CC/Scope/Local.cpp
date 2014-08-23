//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

#include "AST/Attribute.hpp"
#include "AST/Function.hpp"
#include "AST/Object.hpp"
#include "AST/Storage.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Scope_Local::AllocAutoInfo::setMax
      //
      void Scope_Local::AllocAutoInfo::setMax(AllocAutoInfo const &alloc)
      {
         localArs = std::max(localArs, alloc.localArs);
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
      void Scope_Local::allocAutoObj(AllocAutoInfo &alloc, AST::Object *obj)
      {
         IR::Type_Fixed idxType{Platform::GetWordBits(), 0, 0, 0};

         // If one is not specified, select an address space now.
         if(obj->type->getQualAddr().base == IR::AddrBase::Gen)
         {
            if(obj->refer)
               obj->type = obj->type->getTypeArrayQualAddr(IR::AddrBase::Loc);
            else
               obj->type = obj->type->getTypeArrayQualAddr(IR::AddrBase::LocReg);
         }

         // Select type of local to allocate.
         Core::FastU *idx;
         switch(obj->type->getQualAddr().base)
         {
         case IR::AddrBase::Loc:    idx = &alloc.localArs; break;
         case IR::AddrBase::LocReg: idx = &alloc.localReg; break;
         default: return; // Any other address space is an error.
         }

         // Set object's value (index/address).
         obj->value = IR::ExpCreate_Value(
            IR::Value_Fixed(*idx, idxType), Core::Origin(Core::STRNULL, 0));

         // Update allocation info.
         *idx += obj->type->getSizeWords();
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
      AST::Object::Ref Scope_Local::getObject(AST::Attribute const &attr)
      {
         if(attr.storeExt || attr.storeInt)
            return global.getObject(attr);

         auto glyph = genGlyphObj(attr.name, attr.linka);
         auto obj   = AST::Object::Create(attr.name, glyph);

         obj->linka = attr.linka;
         obj->store = AST::Storage::Auto;
         obj->type  = attr.type;

         // If declaration is explicitly auto, always make it addressable.
         obj->refer = attr.storeAuto;

         // If register is used, do not allow taking address.
         obj->noRef = attr.storeReg;

         localObj.emplace(attr.name, obj);

         return obj;
      }
   }
}

//EOF

