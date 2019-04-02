//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C __va_arg expression.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/VaArg.hpp"

#include "CC/Factory.hpp"

#include "SR/Arg.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_VaArg::v_genStmnt
   //
   void Exp_VaArg::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      GenStmnt_Move(this, ctx, dst, getArg());
   }

   //
   // Exp_VaArg::v_getArg
   //
   SR::Arg Exp_VaArg::v_getArg() const
   {
      // *--*(type __sta *__adr_cpy *)&exp

      auto typeArg = type->getTypeQual({{IR::AddrBase::Sta, Core::STR_}});
      auto typeCst = typeArg->getTypePointer()
         ->getTypeQual({exp->getType()->getQualAddr()})->getTypePointer();

      auto expPtr = fact.expConvert_Pointer(typeCst, exp->getArgDup().data, pos);

      return {typeArg, fact.expCreate_DecPre(fact.expCreate_Deref(expPtr, pos), pos)};
   }

   //
   // Exp_VaArg::v_isIRExp
   //
   bool Exp_VaArg::v_isIRExp() const
   {
      return false;
   }

   //
   // Factory::expCreate_VaArg
   //
   SR::Exp::CRef Factory::expCreate_VaArg(SR::Type const *type,
      SR::Exp const *exp, Core::Origin pos)
   {
      return Exp_VaArg::Create(type, exp, pos, *this);
   }
}

// EOF

