//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
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

#include "../../CC/Type.hpp"

#include "../../SR/Exp/Bitwise.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpCreate_Bitwise
      //
      // Uses a base type and code set to generalize basic bitwise expression
      // creation.
      //
      template<typename Base, typename Codes>
      SR::Exp::CRef ExpCreate_Bitwise(SR::Exp const *l, SR::Exp const *r,
         Core::Origin pos)
      {
         auto expL = ExpPromo_Int(ExpPromo_LValue(l, pos), pos);
         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // integer op integer
         if(typeL->isCTypeInteg() && typeR->isCTypeInteg())
         {
            auto type = SR::Type::None;
            std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);

            return ExpCreate_ArithInteg<Base, Codes>(type, expL, expR, pos);
         }

         throw Core::ExceptStr(pos, "expected integer type");
      }
      //
      // ExpCreate_BitwiseEq
      //
      // Uses a base type and code set to generalize basic bitwise assignment
      // expression creation.
      //
      template<typename Base, typename Codes>
      SR::Exp::CRef ExpCreate_BitwiseEq(SR::Exp const *expL,
         SR::Exp const *r, Core::Origin pos)
      {
         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // integer op= integer
         if(typeL->isCTypeInteg() && typeR->isCTypeInteg())
         {
            SR::Type::CPtr evalT;
            std::tie(evalT, std::ignore, expR) = ExpPromo_Arith(expL, expR, pos);

            return ExpCreate_ArithEqInteg<Base, Codes>(evalT, typeL, expL, expR, pos);
         }

         throw Core::ExceptStr(pos, "expected integer type");
      }

      //
      // ExpCreate_Shift
      //
      // Uses a base type and code set to generalize basic shift expression
      // creation.
      //
      template<typename Base, typename Codes>
      SR::Exp::CRef ExpCreate_Shift(SR::Exp const *l, SR::Exp const *r,
         Core::Origin pos)
      {
         auto expL = ExpPromo_Int(ExpPromo_LValue(l, pos), pos);
         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // Prepare shift-amount operand.
         if(!typeR->isCTypeInteg())
            throw Core::ExceptStr(pos, "expected integer type");

         // Convert to int rank, retaining sign.
         if(typeR->getSizeBitsS())
            expR = ExpConvert_Arith(TypeIntegPrS, expR, pos);
         else
            expR = ExpConvert_Arith(TypeIntegPrU, expR, pos);

         auto code = SR::ExpCode_Arith<Codes>(typeL).code;

         return SR::Exp_Arith<Base>::Create(code, typeL, expL, expR, pos);
      }

      //
      // ExpCreate_ShiftEq
      //
      // Uses a base type and code set to generalize basic shift expression
      // creation.
      //
      template<typename Base, typename Codes>
      SR::Exp::CRef ExpCreate_ShiftEq(SR::Exp const *expL, SR::Exp const *r,
         Core::Origin pos)
      {
         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // Prepare shift-amount operand.
         if(!typeR->isCTypeInteg())
            throw Core::ExceptStr(pos, "expected integer type");

         // Convert to int rank, retaining sign.
         if(typeR->getSizeBitsS())
            expR = ExpConvert_Arith(TypeIntegPrS, expR, pos);
         else
            expR = ExpConvert_Arith(TypeIntegPrU, expR, pos);

         auto code = SR::ExpCode_Arith<Codes>(typeL).code;

         return SR::Exp_ArithEq<Base>::Create(typeL, code, false, typeL, expL, expR, pos);
      }
   }
}

#endif//GDCC__CC__Exp__Bitwise_H__

