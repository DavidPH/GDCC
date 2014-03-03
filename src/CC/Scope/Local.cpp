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

#include "AST/Attribute.hpp"
#include "AST/Object.hpp"

#include "Core/Exception.hpp"


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
         Scope{parent_}, global{global_}
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
         throw Core::ExceptStr(attr.namePos, "getObject stub");
      }
   }
}

//EOF

