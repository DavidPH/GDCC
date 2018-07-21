//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__Exp_H__
#define GDCC__ACC__Exp_H__

#include "ACC/Exp.hpp"

#include "CC/Exp.hpp"
#include "CC/Type.hpp"

#include "IR/CallType.hpp"

#include "SR/Exp.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::ACC
{
   //
   // ExpPromo_Assign
   //
   // Allows additional implicit conversions over C.
   //
   SR::Exp::CRef ExpPromo_Assign(SR::Type const *typeL, SR::Exp const *e,
      Core::Origin pos)
   {
      auto exp   = CC::ExpPromo_LValue(e, pos);
      auto typeR = exp->getType();

      // integer = str
      if(typeL->isCTypeInteg() &&
         typeR->isTypePointer() && typeR->getBaseType()->isTypeStrEnt())
      {
         return CC::ExpConvert_ArithPtr(typeL, exp, pos);
      }

      // str = integer
      if(typeL->isTypePointer() && typeL->getBaseType()->isTypeStrEnt() &&
         typeR->isCTypeInteg())
      {
         return CC::ExpConvert_PtrArith(typeL, exp, pos);
      }

      // integer = special
      if(typeL->isCTypeInteg() &&
         typeR->isTypePointer() && typeR->getBaseType()->isTypeFunction() &&
            typeR->getBaseType()->getCallType() == IR::CallType::Special)
      {
         return CC::ExpConvert_ArithPtr(typeL, exp, pos);
      }

      // Defer to C rules.
      return CC::ExpPromo_Assign_Base(typeL, exp, pos);
   }
}

#endif//GDCC__ACC__Exp_H__

