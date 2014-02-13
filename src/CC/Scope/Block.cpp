//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C block scope handling.
//
//-----------------------------------------------------------------------------

#include "CC/Scope/Block.hpp"

#include "CC/Scope/Function.hpp"

#include "AST/Function.hpp"
#include "AST/Object.hpp"
#include "AST/Type.hpp"

#include "Bytecode/Platform.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // BlockScope::AllocAutoInfo::setMax
      //
      void BlockScope::AllocAutoInfo::setMax(AllocAutoInfo const &alloc)
      {
         localArs = std::max(localArs, alloc.localArs);
         localReg = std::max(localReg, alloc.localReg);
      }

      //
      // BlockScope constructor
      //
      BlockScope::BlockScope(LocalScope *parent_, FunctionScope *fn_) :
         LocalScope{parent_, fn_->global}, fn{fn_}
      {
      }

      //
      // BlockScope destructor
      //
      BlockScope::~BlockScope()
      {
      }

      //
      // BlockScope::allocAuto
      //
      BlockScope::AllocAutoInfo BlockScope::allocAuto(AllocAutoInfo const &base)
      {
         IR::Type_Fixed idxType{Bytecode::GetWordBits(), 0, 0, 0};

         // Allocate local objects.
         auto alloc = base;
         for(auto &itr : localObj)
         {
            auto &obj = itr.second;

            auto &idx = obj->point ? alloc.localArs : alloc.localReg;
            obj->value = IR::ExpCreate_ValueRoot(
               IR::Value_Fixed(idx, idxType), Core::Origin(Core::STRNULL, 0));
            idx += obj->type->getSizeWords();
         }

         // Allocate sub-scopes.
         auto allocSub = alloc;
         for(auto &ctx : subScopes)
            allocSub.setMax(ctx->allocAuto(alloc));

         return allocSub;
      }

      //
      // BlockScope::getFunction
      //
      AST::Function::Ref BlockScope::getFunction()
      {
         return fn->fn;
      }

      //
      // BlockScope::getScopeFunction
      //
      FunctionScope *BlockScope::getScopeFunction()
      {
         return fn;
      }
   }
}

//EOF

