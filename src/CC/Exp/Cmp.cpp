//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C relational and equality expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Cmp.hpp"

#include "CC/Factory.hpp"
#include "CC/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Factory::expCreate_CmpEQ
   //
   SR::Exp::CRef Factory::expCreate_CmpEQ(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto [type, expL, expR] = expPromo_CmpEqu(l, r, pos);

      if(type->isCTypeArith() || type->isTypePointer())
         return ExpCreate_CmpArith<SR::Exp_CmpEQ>(
            *this, IR::CodeBase::CmpEQ, TypeIntegPrS, type, expL, expR, pos);

      Core::Error(pos, "invalid operands to 'operator =='");
   }

   //
   // Factory::expCreate_CmpGE
   //
   SR::Exp::CRef Factory::expCreate_CmpGE(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto [type, expL, expR] = expPromo_CmpRel(l, r, pos);

      if(type->isCTypeArith() || type->isTypePointer())
         return ExpCreate_CmpArith<SR::Exp_CmpGE>(
            *this, IR::CodeBase::CmpGE, TypeIntegPrS, type, expL, expR, pos);

      Core::Error(pos, "invalid operands to 'operator >='");
   }

   //
   // Factory::expCreate_CmpGT
   //
   SR::Exp::CRef Factory::expCreate_CmpGT(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto [type, expL, expR] = expPromo_CmpRel(l, r, pos);

      if(type->isCTypeArith() || type->isTypePointer())
         return ExpCreate_CmpArith<SR::Exp_CmpGT>(
            *this, IR::CodeBase::CmpGT, TypeIntegPrS, type, expL, expR, pos);

      Core::Error(pos, "invalid operands to 'operator >='");
   }

   //
   // Factory::expCreate_CmpLE
   //
   SR::Exp::CRef Factory::expCreate_CmpLE(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto [type, expL, expR] = expPromo_CmpRel(l, r, pos);

      if(type->isCTypeArith() || type->isTypePointer())
         return ExpCreate_CmpArith<SR::Exp_CmpLE>(
            *this, IR::CodeBase::CmpLE, TypeIntegPrS, type, expL, expR, pos);

      Core::Error(pos, "invalid operands to 'operator >='");
   }

   //
   // Factory::expCreate_CmpLT
   //
   SR::Exp::CRef Factory::expCreate_CmpLT(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto [type, expL, expR] = expPromo_CmpRel(l, r, pos);

      if(type->isCTypeArith() || type->isTypePointer())
         return ExpCreate_CmpArith<SR::Exp_CmpLT>(
            *this, IR::CodeBase::CmpLT, TypeIntegPrS, type, expL, expR, pos);

      Core::Error(pos, "invalid operands to 'operator >='");
   }

   //
   // Factory::expCreate_CmpNE
   //
   SR::Exp::CRef Factory::expCreate_CmpNE(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto [type, expL, expR] = expPromo_CmpEqu(l, r, pos);

      if(type->isCTypeArith() || type->isTypePointer())
         return ExpCreate_CmpArith<SR::Exp_CmpNE>(
            *this, IR::CodeBase::CmpNE, TypeIntegPrS, type, expL, expR, pos);

      Core::Error(pos, "invalid operands to 'operator !='");
   }
}

// EOF

