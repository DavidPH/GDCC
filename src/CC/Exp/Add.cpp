//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpCreate_Add
      //
      AST::Exp::CRef ExpCreate_Add(AST::Exp const *e, Core::Origin pos)
      {
         if(!e->getType()->isCTypeArith())
            throw Core::ExceptStr(pos, "expected arithmetic operand");

         return ExpPromo_Int(ExpPromo_LValue(e, pos), pos);
      }

      //
      // ExpCreate_Add
      //
      AST::Exp::CRef ExpCreate_Add(AST::Exp const *l, AST::Exp const *r,
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

            auto baseL = typeL->getBaseType();

            if(!baseL->isTypeComplete() || !baseL->isCTypeObject())
               throw Core::ExceptStr(pos, "expected pointer to complete object");

            // Convert integer to int rank, retaining sign.
            if(typeR->getSizeBitsS())
               expR = ExpConvert_Arith(TypeIntegPrS, expR, pos);
            else
               expR = ExpConvert_Arith(TypeIntegPrU, expR, pos);

            // As of the time of this comment, multi-word pointers only need
            // their high word modified to be added to, and therefore do not
            // require a different handling than a single-word pointer.
            return static_cast<AST::Exp::CRef>(
               new Exp_AddPoint(typeL, expL, expR, pos));
         }

         // arithmetic + arithmetic
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
         {
            auto type = AST::Type::None;
            std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);

            return ExpCreate_Arith<AST::Exp_Add, IR::CodeSet_Add>(type, expL, expR, pos);
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator +'");
      }
   }
}

// EOF

