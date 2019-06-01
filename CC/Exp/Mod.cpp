//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator %" expressions.
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
   // Factory::expCreate_Mod
   //
   SR::Exp::CRef Factory::expCreate_Mod(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto expL = expPromo_Int(expPromo_LValue(l, pos), pos);
      auto expR = expPromo_Int(expPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // arithmetic % arithmetic
      if(typeL->isCTypeArith() && typeR->isCTypeArith())
      {
         SR::Type::CPtr type;
         std::tie(type, expL, expR) = expPromo_Arith(expL, expR, pos);

         if(!type->isCTypeInteg() && !type->isCTypeFixed())
            Core::Error(pos, "expected integer or fixed-point");

         return ExpCreate_Arith<SR::Exp_Mod>(
            *this, IR::CodeBase::Mod, type, expL, expR, pos);
      }

      Core::Error(pos, "invalid operands to 'operator %'");
   }

   //
   // Factory::expCreate_ModEq
   //
   SR::Exp::CRef Factory::expCreate_ModEq(SR::Exp const *expL, SR::Exp const *r,
      Core::Origin pos)
   {
      // Special check for structure property.
      if(auto l = dynamic_cast<Exp_MemProp const *>(expL))
         return l->createExp_ModEq(SR::Exp::CRef{r});

      if(!IsModLValue(expL))
         Core::Error(pos, "expected modifiable lvalue");

      auto expR = expPromo_Int(expPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // arithmetic %= arithmetic
      if(typeL->isCTypeArith() && typeR->isCTypeArith())
      {
         SR::Type::CPtr evalT;
         std::tie(evalT, std::ignore, expR) = expPromo_Arith(expL, expR, pos);

         if(!evalT->isCTypeInteg() && !evalT->isCTypeFixed())
            Core::Error(pos, "expected integer or fixed-point");

         return ExpCreate_ArithEq<SR::Exp_Mod>(
            *this, IR::CodeBase::Mod, evalT, typeL, expL, expR, pos);
      }

      Core::Error(pos, "invalid operands to 'operator %='");
   }
}

// EOF

