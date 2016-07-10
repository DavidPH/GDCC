//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
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

namespace GDCC
{
   namespace CC
   {
      //
      // ExpCreate_Add
      //
      SR::Exp::CRef ExpCreate_Add(SR::Exp const *e, Core::Origin pos)
      {
         if(!e->getType()->isCTypeArith())
            throw Core::ExceptStr(pos, "expected arithmetic operand");

         return ExpPromo_Int(ExpPromo_LValue(e, pos), pos);
      }

      //
      // ExpCreate_Add
      //
      SR::Exp::CRef ExpCreate_Add(SR::Exp const *l, SR::Exp const *r,
         Core::Origin pos)
      {
         auto expL = ExpPromo_Int(ExpPromo_LValue(l, pos), pos);
         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // Try to put any pointer as the left operand.
         if(expR->getType()->isTypePointer())
            std::swap(expL, expR), std::swap(typeL, typeR);

         // pointer + integer
         if(typeL->isTypePointer())
         {
            if(!typeR->isCTypeInteg())
               throw Core::ExceptStr(pos, "expected integer operand");

            // Convert integer to int rank, retaining sign.
            if(typeR->getSizeBitsS())
               expR = ExpConvert_Arith(TypeIntegPrS, expR, pos);
            else
               expR = ExpConvert_Arith(TypeIntegPrU, expR, pos);

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
               throw Core::ExceptStr(pos, "expected pointer to complete object");

            // As of the time of this comment, multi-word pointers only need
            // their high word modified to be added to, and therefore do not
            // require a different handling than a single-word pointer.
            return Exp_AddPoint::Create(typeL, expL, expR, pos);
         }

         // arithmetic + arithmetic
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
         {
            auto type = SR::Type::None;
            std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);

            return ExpCreate_Arith<SR::Exp_Add, IR::CodeSet_Add>(type, expL, expR, pos);
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator +'");
      }

      // ExpCreate_AddEq
      SR::Exp::CRef ExpCreate_AddEq(SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
         {return ExpCreate_AddEq(l, r, pos, false);}

      //
      // ExpCreate_AddEq
      //
      SR::Exp::CRef ExpCreate_AddEq(SR::Exp const *expL, SR::Exp const *r,
         Core::Origin pos, bool post)
      {
         if(!IsModLValue(expL))
            throw Core::ExceptStr(pos, "expected modifiable lvalue");

         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // pointer += integer
         if(typeL->isTypePointer())
         {
            if(!typeR->isCTypeInteg())
               throw Core::ExceptStr(pos, "expected integer operand");

            auto baseL = typeL->getBaseType();

            if(!baseL->isTypeComplete() || !baseL->isCTypeObject())
               throw Core::ExceptStr(pos, "expected pointer to complete object");

            // Convert integer to int rank, retaining sign.
            if(typeR->getSizeBitsS())
               expR = ExpConvert_Arith(TypeIntegPrS, expR, pos);
            else
               expR = ExpConvert_Arith(TypeIntegPrU, expR, pos);

            return Exp_AddPointEq::Create(post, typeL, expL, expR, pos);
         }

         // arithmetic += arithmetic
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
         {
            SR::Type::CPtr evalT;
            std::tie(evalT, std::ignore, expR) = ExpPromo_Arith(expL, expR, pos);

            return ExpCreate_ArithEq<SR::Exp_Add, IR::CodeSet_Add>(
               evalT, typeL, expL, expR, pos, post);
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator +='");
      }
   }
}

// EOF

