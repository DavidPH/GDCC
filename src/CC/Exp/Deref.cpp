//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C unary "operator *" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Deref.hpp"

#include "Core/Exception.hpp"

#include "IR/Addr.hpp"

#include "SR/Arg.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
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
         return type->getQualVola();
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
      // ExpCreate_Deref
      //
      SR::Exp::CRef ExpCreate_Deref(SR::Exp const *e, Core::Origin pos)
      {
         auto exp  = ExpPromo_LValue(e, pos);
         auto type = exp->getType();

         if(!type->isTypePointer())
            throw Core::ExceptStr(pos, "expected pointer");

         type = type->getBaseType();

         return Exp_Deref::Create(type, exp, pos);
      }
   }
}

// EOF

