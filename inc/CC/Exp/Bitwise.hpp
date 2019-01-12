//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C bitwise and shift expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Bitwise_H__
#define GDCC__CC__Exp__Bitwise_H__

#include "../../CC/Exp/Arith.hpp"

#include "../../CC/Factory.hpp"
#include "../../CC/Type.hpp"

#include "../../SR/Exp/Bitwise.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // ExpCreate_Bitwise
   //
   // Uses a base type and code set to generalize basic bitwise expression
   // creation.
   //
   template<typename Base, typename Codes>
   SR::Exp::CRef ExpCreate_Bitwise(Factory &fact, SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos)
   {
      auto expL = fact.expPromo_Int(fact.expPromo_LValue(l, pos), pos);
      auto expR = fact.expPromo_Int(fact.expPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // integer op integer
      if(typeL->isCTypeInteg() && typeR->isCTypeInteg())
      {
         auto type = SR::Type::None;
         std::tie(type, expL, expR) = fact.expPromo_Arith(expL, expR, pos);

         return ExpCreate_ArithInteg<Base, Codes>(fact, type, expL, expR, pos);
      }

      Core::Error(pos, "expected integer type");
   }
   //
   // ExpCreate_BitwiseEq
   //
   // Uses a base type and code set to generalize basic bitwise assignment
   // expression creation.
   //
   template<typename Base, typename Codes>
   SR::Exp::CRef ExpCreate_BitwiseEq(Factory &fact, SR::Exp const *expL,
      SR::Exp const *r, Core::Origin pos)
   {
      auto expR = fact.expPromo_Int(fact.expPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // integer op= integer
      if(typeL->isCTypeInteg() && typeR->isCTypeInteg())
      {
         SR::Type::CPtr evalT;
         std::tie(evalT, std::ignore, expR) = fact.expPromo_Arith(expL, expR, pos);

         return ExpCreate_ArithEqInteg<Base, Codes>(fact, evalT, typeL, expL, expR, pos);
      }

      Core::Error(pos, "expected integer type");
   }

   //
   // ExpCreate_Shift
   //
   // Uses a base type and code set to generalize basic shift expression
   // creation.
   //
   template<typename Base, typename Codes>
   SR::Exp::CRef ExpCreate_Shift(Factory &fact, SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos)
   {
      auto expL = fact.expPromo_Int(fact.expPromo_LValue(l, pos), pos);
      auto expR = fact.expPromo_Int(fact.expPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // Prepare shift-amount operand.
      if(!typeR->isCTypeInteg())
         Core::Error(pos, "expected integer type");

      // Convert to int rank, retaining sign.
      if(typeR->getSizeBitsS())
         expR = fact.expConvert_Arith(TypeIntegPrS, expR, pos);
      else
         expR = fact.expConvert_Arith(TypeIntegPrU, expR, pos);

      auto code = SR::ExpCode_Arith<Codes>(typeL);

      return SR::Exp_Arith<Base>::Create(code, typeL, expL, expR, pos);
   }

   //
   // ExpCreate_ShiftEq
   //
   // Uses a base type and code set to generalize basic shift expression
   // creation.
   //
   template<typename Base, typename Codes>
   SR::Exp::CRef ExpCreate_ShiftEq(Factory &fact, SR::Exp const *expL, SR::Exp const *r,
      Core::Origin pos)
   {
      auto expR = fact.expPromo_Int(fact.expPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // Prepare shift-amount operand.
      if(!typeR->isCTypeInteg())
         Core::Error(pos, "expected integer type");

      // Convert to int rank, retaining sign.
      if(typeR->getSizeBitsS())
         expR = fact.expConvert_Arith(TypeIntegPrS, expR, pos);
      else
         expR = fact.expConvert_Arith(TypeIntegPrU, expR, pos);

      auto code = SR::ExpCode_Arith<Codes>(typeL);

      return SR::Exp_ArithEq<Base>::Create(typeL, code, false, typeL, expL, expR, pos);
   }
}

#endif//GDCC__CC__Exp__Bitwise_H__

