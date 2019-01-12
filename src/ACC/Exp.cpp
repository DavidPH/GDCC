//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS expressions.
//
//-----------------------------------------------------------------------------

#include "ACC/Factory.hpp"

#include "SR/Exp.hpp"
#include "SR/Type.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // Factory::expPromo_Assign
   //
   // Allows additional implicit conversions over C.
   //
   SR::Exp::CRef Factory::expPromo_Assign(SR::Type const *typeL, SR::Exp const *e,
      Core::Origin pos)
   {
      auto exp   = expPromo_LValue(e, pos);
      auto typeR = exp->getType();

      // integer = str
      if(typeL->isCTypeInteg() &&
         typeR->isTypePointer() && typeR->getBaseType()->isTypeStrEnt())
      {
         return expConvert_ArithPtr(typeL, exp, pos);
      }

      // str = integer
      if(typeL->isTypePointer() && typeL->getBaseType()->isTypeStrEnt() &&
         typeR->isCTypeInteg())
      {
         return expConvert_PtrArith(typeL, exp, pos);
      }

      // integer = special
      if(typeL->isCTypeInteg() &&
         typeR->isTypePointer() && typeR->getBaseType()->isTypeFunction() &&
            typeR->getBaseType()->getCallType() == IR::CallType::Special)
      {
         return expConvert_ArithPtr(typeL, exp, pos);
      }

      // Defer to C rules.
      return CC::Factory::expPromo_Assign(typeL, exp, pos);
   }
}

// EOF

