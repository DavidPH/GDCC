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

#include "CC/Type.hpp"

#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Add constructor
      //
      Exp_Add::Exp_Add(AST::Type const *t, AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos_) :
         Super{l, r, pos_}, type{t}
      {
      }

      //
      // Exp_Add destructor
      //
      Exp_Add::~Exp_Add()
      {
      }

      //
      // Exp_Add::v_getIRExp
      //
      IR::Exp::CRef Exp_Add::v_getIRExp() const
      {
         return IR::ExpCreate_BinaryAdd(expL->getIRExp(), expR->getIRExp(), pos);
      }

      //
      // Exp_Add::v_getType
      //
      AST::Type::CRef Exp_Add::v_getType() const
      {
         return type;
      }

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
            typeL = expL->getType();
            typeR = expR->getType();

            if(type->isCTypeFloat())
               throw Core::ExceptStr(pos, "float + float stub");

            if(type->isCTypeFixed())
               throw Core::ExceptStr(pos, "fixed + fixed stub");

            if(type->isCTypeInteg())
            {
               return static_cast<AST::Exp::CRef>(
                  new Exp_AddInteg(type, expL, expR, pos));
            }
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator +'");
      }
   }
}

// EOF

