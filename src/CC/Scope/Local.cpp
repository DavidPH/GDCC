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

#include "CC/Scope/Block.hpp"
#include "CC/Scope/Global.hpp"

#include "AST/Attribute.hpp"
#include "AST/Function.hpp"
#include "AST/Object.hpp"
#include "AST/Storage.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Linkage.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // LocalScope constructor
      //
      LocalScope::LocalScope(Scope *parent_, GlobalScope *global_) :
         Scope{parent_},

         global{global_},
         label {Core::STRNULL}
      {
      }

      //
      // LocalScope destructor
      //
      LocalScope::~LocalScope()
      {
         for(auto &ctx : subScopes)
            delete ctx;
      }

      //
      // LocalScope::createScopeBlock
      //
      BlockScope *LocalScope::createScopeBlock()
      {
         auto ctx = new BlockScope(this, getScopeFunction());
         subScopes.emplace_back(ctx);
         return ctx;
      }

      //
      // LocalScope::createScopeLoop
      //
      BlockScope *LocalScope::createScopeLoop()
      {
         // FIXME
         auto ctx = new BlockScope(this, getScopeFunction());
         subScopes.emplace_back(ctx);
         return ctx;
      }

      //
      // LocalScope::createScopeSwitch
      //
      BlockScope *LocalScope::createScopeSwitch()
      {
         // FIXME
         auto ctx = new BlockScope(this, getScopeFunction());
         subScopes.emplace_back(ctx);
         return ctx;
      }

      //
      // LocalScope::genGlyphObj
      //
      Core::String LocalScope::genGlyphObj(Core::String name, IR::Linkage linka)
      {
         // Actual linkages must go through global.
         if(linka != IR::Linkage::None)
            return global->genGlyphObj(name, linka);

         auto fn = getFunction();

         // Anonymous objects with no linkage need a fully generated name.
         if(!name) return getFunction()->genLabel();

         // Otherwise, base it on the scope's prefix (generating if needed).
         if(!label) label = getFunction()->genLabel();
         return label + name;
      }

      //
      // LocalScope::genIR
      //
      void LocalScope::genIR(IR::Program &prog)
      {
         for(auto ctx : subScopes)
            ctx->genIR(prog);

         for(auto itr : localObj)
            itr.second->getIRObject(prog);
      }

      //
      // LocalScope::getObject
      //
      AST::Object::Ref LocalScope::getObject(AST::Attribute const &attr)
      {
         if(attr.storeExt || attr.storeInt)
            return global->getObject(attr);

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

