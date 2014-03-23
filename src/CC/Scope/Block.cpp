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
      // Scope_Block::AllocAutoInfo::setMax
      //
      void Scope_Block::AllocAutoInfo::setMax(AllocAutoInfo const &alloc)
      {
         localArs = std::max(localArs, alloc.localArs);
         localReg = std::max(localReg, alloc.localReg);
      }

      //
      // Scope_Block constructor
      //
      Scope_Block::Scope_Block(Scope_Local *parent_, Scope_Function *fn_) :
         Scope_Local{parent_, fn_->global}, fn{fn_}
      {
      }

      //
      // Scope_Block destructor
      //
      Scope_Block::~Scope_Block()
      {
      }

      //
      // Scope_Block::allocAuto
      //
      Scope_Block::AllocAutoInfo Scope_Block::allocAuto(AllocAutoInfo const &base)
      {
         IR::Type_Fixed idxType{Bytecode::GetWordBits(), 0, 0, 0};

         // Allocate local objects.
         auto alloc = base;
         for(auto &itr : localObj)
         {
            auto &obj = itr.second;

            auto qual = obj->type->getQual();
            Core::FastU *idx;
            if(obj->refer)
            {
               idx = &alloc.localArs;
               if(qual.space.base == IR::AddrBase::Gen)
                  qual.space = IR::AddrBase::Loc;
            }
            else
            {
               idx = &alloc.localReg;
               if(qual.space.base == IR::AddrBase::Gen)
                  qual.space = IR::AddrBase::LocReg;
            }
            obj->type  = obj->type->getTypeQual(qual);
            obj->value = IR::ExpCreate_ValueRoot(
               IR::Value_Fixed(*idx, idxType), Core::Origin(Core::STRNULL, 0));
            *idx += obj->type->getSizeWords();
         }

         // Allocate sub-scopes.
         auto allocSub = alloc;
         for(auto &ctx : subScopes)
            allocSub.setMax(ctx->allocAuto(alloc));

         return allocSub;
      }

      //
      // Scope_Block::getFunction
      //
      AST::Function::Ref Scope_Block::getFunction()
      {
         return fn->fn;
      }

      //
      // Scope_Block::getScopeFunction
      //
      Scope_Function *Scope_Block::getScopeFunction()
      {
         return fn;
      }
   }
}

//EOF

