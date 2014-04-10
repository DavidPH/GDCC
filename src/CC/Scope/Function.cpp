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

#include "AST/Function.hpp"
#include "AST/Object.hpp"


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
      Scope_Function::Scope_Function(Scope_Global &parent_, AST::Function *fn_,
         Core::Array<AST::Object::Ref> &&params_) :
         Scope_Local{parent_, *this}, params{std::move(params_)}, fn{fn_}
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
         // Allocate parameter objects.
         AllocAutoInfo alloc;
         for(auto &obj : params)
            allocAutoObj(alloc, obj);

         // Allocate sub-scopes.
         auto allocSub = alloc;
         for(auto &ctx : subScopes)
            allocSub.setMax(ctx->allocAuto(alloc));

         // Set function's local counts.
         fn->localArs = allocSub.localArs;
         fn->localReg = allocSub.localReg;
      }

      //
      // Scope_Function::getLabel
      //
      Core::String Scope_Function::getLabel(Core::String name, bool)
      {
         // FIXME: TODO: Check for redefinition.
         return fn->label + name;
      }

      //
      // Scope_Function::getLabelBreak
      //
      Core::String Scope_Function::getLabelBreak()
      {
         return Core::STRNULL;
      }

      //
      // Scope_Function::getLabelCase
      //
      Core::String Scope_Function::getLabelCase(Core::Integ const &, bool)
      {
         return Core::STRNULL;
      }

      //
      // Scope_Function::getLabelContinue
      //
      Core::String Scope_Function::getLabelContinue()
      {
         return Core::STRNULL;
      }

      //
      // Scope_Function::getLabelDefault
      //
      Core::String Scope_Function::getLabelDefault(bool)
      {
         return Core::STRNULL;
      }
   }
}

//EOF

