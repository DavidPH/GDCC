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
      // Scope_Local constructor
      //
      Scope_Local::Scope_Local(Scope *parent_, Scope_Global *global_) :
         Scope{parent_},

         global{global_},
         label {Core::STRNULL}
      {
      }

      //
      // Scope_Local destructor
      //
      Scope_Local::~Scope_Local()
      {
         for(auto &ctx : subScopes)
            delete ctx;
      }

      //
      // Scope_Local::createScopeBlock
      //
      Scope_Block *Scope_Local::createScopeBlock()
      {
         auto ctx = new Scope_Block(this, getScopeFunction());
         subScopes.emplace_back(ctx);
         return ctx;
      }

      //
      // Scope_Local::createScopeLoop
      //
      Scope_Block *Scope_Local::createScopeLoop()
      {
         // FIXME
         auto ctx = new Scope_Block(this, getScopeFunction());
         subScopes.emplace_back(ctx);
         return ctx;
      }

      //
      // Scope_Local::createScopeSwitch
      //
      Scope_Block *Scope_Local::createScopeSwitch()
      {
         // FIXME
         auto ctx = new Scope_Block(this, getScopeFunction());
         subScopes.emplace_back(ctx);
         return ctx;
      }

      //
      // Scope_Local::genGlyphObj
      //
      Core::String Scope_Local::genGlyphObj(Core::String name, IR::Linkage linka)
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
      // Scope_Local::genIR
      //
      void Scope_Local::genIR(IR::Program &prog)
      {
         for(auto ctx : subScopes)
            ctx->genIR(prog);

         for(auto itr : localObj)
            itr.second->genObject(prog);
      }

      //
      // Scope_Local::getObject
      //
      AST::Object::Ref Scope_Local::getObject(AST::Attribute const &attr)
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

