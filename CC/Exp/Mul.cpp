//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C binary "operator *" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp.hpp"

#include "CC/Exp/Arith.hpp"
#include "CC/Exp/Mem.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Factory::expCreate_Mul
   //
   SR::Exp::CRef Factory::expCreate_Mul(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto expL = expPromo_Int(expPromo_LValue(l, pos), pos);
      auto expR = expPromo_Int(expPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // arithmetic * arithmetic
      if(typeL->isCTypeArith() && typeR->isCTypeArith())
      {
         SR::Type::CPtr type;
         std::tie(type, expL, expR) = expPromo_Arith(expL, expR, pos);

         // TODO: fixed * integer doesn't require conversion.

         return ExpCreate_Arith<SR::Exp_Mul>(
            *this, IR::CodeBase::Mul, type, expL, expR, pos);
      }

      Core::Error(pos, "invalid operands to 'operator *'");
   }

   //
   // Factory::expCreate_MulEq
   //
   SR::Exp::CRef Factory::expCreate_MulEq(SR::Exp const *expL, SR::Exp const *r,
      Core::Origin pos)
   {
      // Special check for structure property.
      if(auto l = dynamic_cast<Exp_MemProp const *>(expL))
         return l->createExp_MulEq(SR::Exp::CRef{r});

      if(!IsModLValue(expL))
         Core::Error(pos, "expected modifiable lvalue");

      auto expR = expPromo_Int(expPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // arithmetic *= arithmetic
      if(typeL->isCTypeArith() && typeR->isCTypeArith())
      {
         SR::Type::CPtr evalT;
         std::tie(evalT, std::ignore, expR) = expPromo_Arith(expL, expR, pos);

         // TODO: fixed *= integer doesn't require conversion.

         return ExpCreate_ArithEq<SR::Exp_Mul>(
            *this, IR::CodeBase::Mul, evalT, typeL, expL, expR, pos);
      }

      Core::Error(pos, "invalid operands to 'operator *='");
   }
}

// EOF

