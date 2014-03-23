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
      // Scope_Function constructor
      //
      Scope_Function::Scope_Function(Scope_Global *parent_, AST::Function *fn_,
         Core::Array<AST::Object::Ref> &&params_) :
         Scope_Local{parent_, parent_}, params{std::move(params_)}, fn{fn_}
      {
         for(auto const &param : params)
         {
            if(param->name) add(param->name, param);
            localObj.emplace(param->glyph, param);
         }
      }

      //
      // Scope_Function destructor
      //
      Scope_Function::~Scope_Function()
      {
      }

      //
      // Scope_Function::allocAuto
      //
      void Scope_Function::allocAuto()
      {
         IR::Type_Fixed idxType{Bytecode::GetWordBits(), 0, 0, 0};

         // Allocate parameter objects.
         Scope_Block::AllocAutoInfo alloc;
         for(auto &obj : params)
         {
            auto &idx = obj->refer ? alloc.localArs : alloc.localReg;
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
      // Scope_Function::getFunction
      //
      AST::Function::Ref Scope_Function::getFunction()
      {
         return fn;
      }

      //
      // Scope_Function::getScopeFunction
      //
      Scope_Function *Scope_Function::getScopeFunction()
      {
         return this;
      }
   }
}

//EOF

