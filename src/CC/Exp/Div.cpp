//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator /" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp.hpp"

#include "CC/Exp/Arith.hpp"
#include "CC/Type/Struct.hpp"

#include "IR/CodeSet/Arith.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // ExpCreate_Div
   //
   SR::Exp::CRef ExpCreate_Div(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto expL = ExpPromo_Int(ExpPromo_LValue(l, pos), pos);
      auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // arithmetic / arithmetic
      if(typeL->isCTypeArith() && typeR->isCTypeArith())
      {
         auto type = SR::Type::None;
         std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);

         // TODO: fixed / integer doesn't require conversion.

         return ExpCreate_Arith<SR::Exp_Div, IR::CodeSet_Div>(type, expL, expR, pos);
      }

      Core::Error(pos, "invalid operands to 'operator /'");
   }

   //
   // ExpCreate_DivEq
   //
   SR::Exp::CRef ExpCreate_DivEq(SR::Exp const *expL, SR::Exp const *r,
      Core::Origin pos)
   {
      if(!IsModLValue(expL))
         Core::Error(pos, "expected modifiable lvalue");

      auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // arithmetic /= arithmetic
      if(typeL->isCTypeArith() && typeR->isCTypeArith())
      {
         SR::Type::CPtr evalT;
         std::tie(evalT, std::ignore, expR) = ExpPromo_Arith(expL, expR, pos);

         // TODO: fixed /= integer doesn't require conversion.

         return ExpCreate_ArithEq<SR::Exp_Div, IR::CodeSet_Div>(
            evalT, typeL, expL, expR, pos);
      }

      Core::Error(pos, "invalid operands to 'operator /='");
   }

   //
   // ExpCreate_DivEx
   //
   SR::Exp::CRef ExpCreate_DivEx(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto expL = ExpPromo_Int(ExpPromo_LValue(l, pos), pos);
      auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // __div(integer, integer)
      if(typeL->isCTypeInteg() && typeR->isCTypeInteg())
      {
         auto type = SR::Type::None;
         std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);

         auto code = SR::ExpCode_ArithInteg<IR::CodeSet_DiX>(type);

         return SR::Exp_Arith<SR::Exp_DivEx>::Create(
            code, Type_Div::Get(type), expL, expR, pos);
      }

      Core::Error(pos, "invalid operands to __div");
   }
}

// EOF

