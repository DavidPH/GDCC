//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator -" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Sub.hpp"

#include "CC/Exp/Arith.hpp"
#include "CC/Type.hpp"

#include "SR/Exp/Unary.hpp"

#include "IR/CodeSet/Arith.hpp"
#include "IR/CodeSet/Unary.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // ExpCreate_Sub
   //
   SR::Exp::CRef ExpCreate_Sub(SR::Exp const *e, Core::Origin pos)
   {
      auto exp  = ExpPromo_Int(ExpPromo_LValue(e, pos), pos);
      auto type = exp->getType();

      if(!type->isCTypeArith())
         Core::Error(pos, "expected arithmetic operand");

      auto code = SR::ExpCode_Arith<IR::CodeSet_Neg>(type);

      return SR::Exp_UnaryCode<SR::Exp_Neg>::Create(code, type, exp, pos);
   }

   //
   // ExpCreate_Sub
   //
   SR::Exp::CRef ExpCreate_Sub(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto expL = ExpPromo_Int(ExpPromo_LValue(l, pos), pos);
      auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // pointer -
      if(typeL->isTypePointer())
      {
         auto baseL = typeL->getBaseType();

         if(!baseL->isTypeComplete() || !baseL->isCTypeObject())
            Core::Error(pos, "expected pointer to complete object");

         // pointer - integer
         if(typeR->isCTypeInteg())
         {
            // Convert integer to int rank, retaining sign.
            if(typeR->getSizeBitsS())
               expR = ExpConvert_Arith(TypeIntegPrS, expR, pos);
            else
               expR = ExpConvert_Arith(TypeIntegPrU, expR, pos);

            // As of the time of this comment, multi-word pointers only need
            // their high word modified to be added to, and therefore do not
            // require a different handling than a single-word pointer.
            return Exp_SubPtrInt::Create(typeL, expL, expR, pos);
         }

         // pointer - pointer
         if(typeR->isTypePointer())
         {
            auto baseR = typeR->getBaseType();

            if(baseL->getTypeQual() != baseR->getTypeQual())
               Core::Error(pos, "incompatible pointer types");

            // Similar to the above, although in this case there is some
            // minor extra codegen by the Exp_SubPtrPtrW class.
            return Exp_SubPtrPtrW::Create(TypeIntegPrS, expL, expR, pos);
         }
      }

      // arithmetic - arithmetic
      if(typeL->isCTypeArith() && typeR->isCTypeArith())
      {
         auto type = SR::Type::None;
         std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);

         return ExpCreate_Arith<SR::Exp_Sub, IR::CodeSet_Sub>(type, expL, expR, pos);
      }

      Core::Error(pos, "invalid operands to 'operator -'");
   }

   // ExpCreate_SubEq
   SR::Exp::CRef ExpCreate_SubEq(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
      {return ExpCreate_SubEq(l, r, pos, false);}

   //
   // ExpCreate_SubEq
   //
   SR::Exp::CRef ExpCreate_SubEq(SR::Exp const *expL, SR::Exp const *r,
      Core::Origin pos, bool post)
   {
      if(!IsModLValue(expL))
         Core::Error(pos, "expected modifiable lvalue");

      auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // pointer -= integer
      if(typeL->isTypePointer())
      {
         if(!typeR->isCTypeInteg())
            Core::Error(pos, "expected integer operand");

         auto baseL = typeL->getBaseType();

         if(!baseL->isTypeComplete() || !baseL->isCTypeObject())
            Core::Error(pos, "expected pointer to complete object");

         // Convert integer to int rank, retaining sign.
         if(typeR->getSizeBitsS())
            expR = ExpConvert_Arith(TypeIntegPrS, expR, pos);
         else
            expR = ExpConvert_Arith(TypeIntegPrU, expR, pos);

         return Exp_SubPtrIntEq::Create(post, typeL, expL, expR, pos);
      }

      // arithmetic -= arithmetic
      if(typeL->isCTypeArith() && typeR->isCTypeArith())
      {
         SR::Type::CPtr evalT;
         std::tie(evalT, std::ignore, expR) = ExpPromo_Arith(expL, expR, pos);

         return ExpCreate_ArithEq<SR::Exp_Sub, IR::CodeSet_Sub>(
            evalT, typeL, expL, expR, pos, post);
      }

      Core::Error(pos, "invalid operands to 'operator -='");
   }
}

// EOF

