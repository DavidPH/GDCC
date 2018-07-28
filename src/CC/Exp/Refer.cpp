//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C unary "operator &" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Refer.hpp"

#include "Core/Exception.hpp"

#include "IR/Exp.hpp"

#include "SR/Arg.hpp"
#include "SR/Function.hpp"
#include "SR/Object.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_Refer constructor
   //
   Exp_Refer::Exp_Refer(SR::Exp const *e, Core::Origin pos_) :
      Super{e->getType()->getTypePointer(), e, pos_}
   {
   }

   //
   // Exp_Refer::v_genStmnt
   //
   void Exp_Refer::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      auto arg = exp->getArg();
      if(!arg.data) Core::Error(pos, "expected arg data");
      arg.data->genStmnt(ctx, dst);
   }

   //
   // Exp_Refer::v_getFunction
   //
   SR::Function::Ref Exp_Refer::v_getFunction() const
   {
      return exp->getFunction();
   }

   //
   // Exp_Refer::v_getIRExp
   //
   IR::Exp::CRef Exp_Refer::v_getIRExp() const
   {
      auto arg = exp->getArg();
      if(!arg.data) Core::Error(pos, "expected arg data");
      return arg.data->getIRExp();
   }

   //
   // Exp_Refer::v_isEffect
   //
   bool Exp_Refer::v_isEffect() const
   {
      auto arg = exp->getArg();
      if(!arg.data) return false;
      return arg.data->isEffect();
   }

   //
   // Exp_Refer::v_isFunction
   //
   bool Exp_Refer::v_isFunction() const
   {
      return exp->isFunction();
   }

   //
   // Exp_Refer::v_isIRExp
   //
   bool Exp_Refer::v_isIRExp() const
   {
      auto arg = exp->getArg();
      if(!arg.data) return false;
      return arg.data->isIRExp();
   }

   //
   // ExpCreate_Refer
   //
   SR::Exp::CRef ExpCreate_Refer(SR::Exp const *e, Core::Origin pos)
   {
      auto type = e->getType();

      if(type->isCTypeFunction())
         return Exp_Refer::Create(e, pos);

      if(!IsLValue(e))
         Core::Error(pos, "expected function or lvalue");

      if(type->isTypeBitfield())
         Core::Error(pos, "cannot get address of bitfield");

      // Check if e is an object declared as register.
      if(e->isObject() && e->getObject()->noRef)
         Core::Error(pos, "cannot get address of register object");

      e->setRefer();

      return Exp_Refer::Create(e, pos);
   }
}

// EOF

