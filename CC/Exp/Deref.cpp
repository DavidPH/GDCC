//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C unary "operator *" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Deref.hpp"

#include "CC/Factory.hpp"

#include "Core/Exception.hpp"

#include "Target/Addr.hpp"

#include "SR/Arg.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_Deref::v_genStmnt
   //
   void Exp_Deref::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      GenStmnt_Move(this, ctx, dst, getArg());
   }

   //
   // Exp_Deref::v_getArg
   //
   SR::Arg Exp_Deref::v_getArg() const
   {
      return SR::Arg(type, exp);
   }

   //
   // Exp_Deref::v_isEffect
   //
   bool Exp_Deref::v_isEffect() const
   {
      return type->getQualVola() || Super::v_isEffect();
   }

   //
   // Exp_Deref::v_isIRExp
   //
   bool Exp_Deref::v_isIRExp() const
   {
      return false;
   }

   //
   // Exp_Deref::v_isNoAuto
   //
   bool Exp_Deref::v_isNoAuto() const
   {
      return type->getQualAddr().base != IR::AddrBase::Aut && exp->isNoAuto();
   }

   //
   // Factory::expCreate_Deref
   //
   SR::Exp::CRef Factory::expCreate_Deref(SR::Exp const *e, Core::Origin pos)
   {
      auto exp  = expPromo_LValue(e, pos);
      auto type = exp->getType();

      if(!type->isTypePointer())
         Core::Error(pos, "expected pointer");

      type = type->getBaseType();

      return Exp_Deref::Create(type, exp, pos);
   }
}

// EOF

