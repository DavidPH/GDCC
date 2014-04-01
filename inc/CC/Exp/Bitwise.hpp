//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

#include "../../AST/Exp/Bitwise.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
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
      AST::Exp::CRef ExpCreate_Bitwise(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         auto expL = ExpPromo_Int(ExpPromo_LValue(l, pos), pos);
         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // integer op integer
         if(typeL->isCTypeInteg() && typeR->isCTypeInteg())
         {
            auto type = AST::Type::None;
            std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);

            return ExpCreate_ArithInteg<Base, Codes>(type, expL, expR, pos);
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
      AST::Exp::CRef ExpCreate_Shift(AST::Exp const *l, AST::Exp const *r,
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

         // Fixed-point types.
         if(typeL->isCTypeFixed())
            return ExpCreate_ArithFixed<Base, Codes>(typeL, expL, expR, pos);

         // Integer types.
         if(typeL->isCTypeInteg())
            return ExpCreate_ArithInteg<Base, Codes>(typeL, expL, expR, pos);

         throw Core::ExceptStr(pos, "expected integer or fixed-point type");
      }
   }
}

#endif//GDCC__CC__Exp__Bitwise_H__

