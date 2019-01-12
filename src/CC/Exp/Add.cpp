//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator +" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Add.hpp"

#include "CC/Exp/Arith.hpp"
#include "CC/Type.hpp"

#include "IR/CodeSet/Arith.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Factory::expCreate_Add
   //
   SR::Exp::CRef Factory::expCreate_Add(SR::Exp const *e, Core::Origin pos)
   {
      if(!e->getType()->isCTypeArith())
         Core::Error(pos, "expected arithmetic operand");

      return expPromo_Int(expPromo_LValue(e, pos), pos);
   }

   //
   // Factory::ExpCreate_Add
   //
   SR::Exp::CRef Factory::expCreate_Add(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto expL = expPromo_Int(expPromo_LValue(l, pos), pos);
      auto expR = expPromo_Int(expPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // Try to put any pointer as the left operand.
      if(expR->getType()->isTypePointer())
         std::swap(expL, expR), std::swap(typeL, typeR);

      // pointer + integer
      if(typeL->isTypePointer())
      {
         if(!typeR->isCTypeInteg())
            Core::Error(pos, "expected integer operand");

         // Convert integer to int rank, retaining sign.
         if(typeR->getSizeBitsS())
            expR = expConvert_Arith(TypeIntegPrS, expR, pos);
         else
            expR = expConvert_Arith(TypeIntegPrU, expR, pos);

         auto baseL = typeL->getBaseType();

         // __str_ent* + int = char const __str_ars*
         if(baseL->isTypeStrEnt())
         {
            SR::TypeQual qual = {{IR::AddrBase::StrArs, Core::STR_}};
            qual.aCons = true;
            auto type = TypeChar->getTypeQual(qual)->getTypePointer();
            return Exp_AddStrEntInt::Create(type, expL, expR, pos);
         }

         if(!baseL->isTypeComplete() || !baseL->isCTypeObject())
            Core::Error(pos, "expected pointer to complete object");

         // As of the time of this comment, multi-word pointers only need
         // their high word modified to be added to, and therefore do not
         // require a different handling than a single-word pointer.
         return Exp_AddPoint::Create(typeL, expL, expR, pos);
      }

      // arithmetic + arithmetic
      if(typeL->isCTypeArith() && typeR->isCTypeArith())
      {
         SR::Type::CPtr type;
         std::tie(type, expL, expR) = expPromo_Arith(expL, expR, pos);

         return ExpCreate_Arith<SR::Exp_Add, IR::CodeSet_Add>(*this, type, expL, expR, pos);
      }

      Core::Error(pos, "invalid operands to 'operator +'");
   }

   //
   // Factory::expCreate_AddEq
   //
   SR::Exp::CRef Factory::expCreate_AddEq(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return expCreate_AddEq(l, r, pos, false);
   }

   //
   // Factory::expCreate_AddEq
   //
   SR::Exp::CRef Factory::expCreate_AddEq(SR::Exp const *expL, SR::Exp const *r,
      Core::Origin pos, bool post)
   {
      if(!IsModLValue(expL))
         Core::Error(pos, "expected modifiable lvalue");

      auto expR = expPromo_Int(expPromo_LValue(r, pos), pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // pointer += integer
      if(typeL->isTypePointer())
      {
         if(!typeR->isCTypeInteg())
            Core::Error(pos, "expected integer operand");

         auto baseL = typeL->getBaseType();

         if(!baseL->isTypeComplete() || !baseL->isCTypeObject())
            Core::Error(pos, "expected pointer to complete object");

         // Convert integer to int rank, retaining sign.
         if(typeR->getSizeBitsS())
            expR = expConvert_Arith(TypeIntegPrS, expR, pos);
         else
            expR = expConvert_Arith(TypeIntegPrU, expR, pos);

         return Exp_AddPointEq::Create(post, typeL, expL, expR, pos);
      }

      // arithmetic += arithmetic
      if(typeL->isCTypeArith() && typeR->isCTypeArith())
      {
         SR::Type::CPtr evalT;
         std::tie(evalT, std::ignore, expR) = expPromo_Arith(expL, expR, pos);

         return ExpCreate_ArithEq<SR::Exp_Add, IR::CodeSet_Add>(
            *this, evalT, typeL, expL, expR, pos, post);
      }

      Core::Error(pos, "invalid operands to 'operator +='");
   }
}

// EOF

