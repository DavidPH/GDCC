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

#include "AST/Object.hpp"


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
   }
}

//EOF

