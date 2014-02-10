//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C function scope handling.
//
//-----------------------------------------------------------------------------

#include "CC/Scope/Function.hpp"

#include "CC/Scope/Block.hpp"
#include "CC/Scope/Global.hpp"

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
      // FunctionScope constructor
      //
      FunctionScope::FunctionScope(GlobalScope *parent_, AST::Function *fn_,
         Core::Array<AST::Object::Ref> &&params_) :
         LocalScope{parent_, parent_}, params{std::move(params_)}, fn{fn_}
      {
         for(auto const &param : params)
            if(param->name) add(param->name, param);
      }

      //
      // FunctionScope destructor
      //
      FunctionScope::~FunctionScope()
      {
      }

      //
      // FunctionScope::allocAuto
      //
      void FunctionScope::allocAuto()
      {
         IR::Type_Fixed idxType{Bytecode::GetWordBits(), 0, 0, 0};

         // Allocate parameter objects.
         BlockScope::AllocAutoInfo alloc;
         for(auto &obj : params)
         {
            auto &idx = obj->point ? alloc.localArs : alloc.localReg;
            obj->value = IR::ExpCreate_ValueRoot(
               IR::Value_Fixed(idx, idxType), Core::Origin(Core::STRNULL, 0));
            idx += obj->type->getSizeWords();
         }

         // Allocate sub-scopes.
         auto allocSub = alloc;
         for(auto &ctx : subScopes)
            allocSub.setMax(ctx->allocAuto(alloc));

         // Set function's local counts.
         fn->localArs = allocSub.localArs;
         fn->localReg = allocSub.localReg;
      }

      //
      // FunctionScope::createScope
      //
      BlockScope *FunctionScope::createScope()
      {
         auto ctx = new BlockScope(this, this);
         subScopes.emplace_back(ctx);
         return ctx;
      }
   }
}

//EOF

