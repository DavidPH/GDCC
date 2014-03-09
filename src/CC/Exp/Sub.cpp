//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator -" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Sub.hpp"

#include "CC/Type.hpp"

#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Code.hpp"
#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpCreate_Sub
      //
      AST::Exp::CRef ExpCreate_Sub(AST::Exp const *e, Core::Origin pos)
      {
         if(!e->getType()->isCTypeArith())
            throw Core::ExceptStr(pos, "expected arithmetic operand");

         throw Core::ExceptStr(pos, "unary - stub");
      }

      //
      // ExpCreate_Sub
      //
      AST::Exp::CRef ExpCreate_Sub(AST::Exp const *l, AST::Exp const *r,
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
               throw Core::ExceptStr(pos, "expected pointer to complete object");

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
               return static_cast<AST::Exp::CRef>(
                  new Exp_SubPtrInt(typeL, expL, expR, pos));
            }

            // pointer - pointer
            if(typeR->isTypePointer())
            {
               auto baseR = typeR->getBaseType();

               if(baseL->getTypeQual() != baseR->getTypeQual())
                  throw Core::ExceptStr(pos, "incompatible pointer types");

               switch(typeL->getSizeWords())
               {
               case 1:
                  return static_cast<AST::Exp::CRef>(
                     new Exp_SubPtrPtrW(TypeIntegPrS, expL, expR, pos));

               default:
                  throw Core::ExceptStr(pos, "unsupported pointer size");
               }
            }
         }

         // arithmetic - arithmetic
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
         {
            auto type = AST::Type::None;
            std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);
            typeL = expL->getType();
            typeR = expR->getType();

            if(type->isCTypeFloat())
               throw Core::ExceptStr(pos, "float - float stub");

            if(type->isCTypeFixed())
               throw Core::ExceptStr(pos, "fixed - fixed stub");

            if(type->isCTypeInteg())
            {
               if(type->getSizeWords() == 1)
               {
                  return AST::Exp_Arith<AST::Exp_Sub>::Create(
                     type->getSizeBitsS() ? IR::Code::SubI_W : IR::Code::SubU_W,
                     type, expL, expR, pos);
               }

               throw Core::ExceptStr(pos, "integer - integer stub");
            }
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator -'");
      }
   }
}

// EOF

