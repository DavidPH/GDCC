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

#include "CC/Exp/Arith.hpp"
#include "CC/Type.hpp"

#include "AST/Exp/Unary.hpp"

#include "IR/CodeSet/Arith.hpp"
#include "IR/CodeSet/Unary.hpp"


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
         auto exp  = ExpPromo_Int(ExpPromo_LValue(e, pos), pos);
         auto type = exp->getType();

         if(!type->isCTypeArith())
            throw Core::ExceptStr(pos, "expected arithmetic operand");

         auto code = AST::ExpCode_Arith<IR::CodeSet_Neg>(type);

         if(code == IR::Code::None)
            throw Core::ExceptStr(pos, "unsupported operand size");

         return AST::Exp_UnaryCode<AST::Exp_Neg>::Create(code, type, exp, pos);
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
               return Exp_SubPtrInt::Create(typeL, expL, expR, pos);
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
                  return Exp_SubPtrPtrW::Create(TypeIntegPrS, expL, expR, pos);

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

            return ExpCreate_Arith<AST::Exp_Sub, IR::CodeSet_Sub>(type, expL, expR, pos);
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator -'");
      }

      // ExpCreate_SubEq
      AST::Exp::CRef ExpCreate_SubEq(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
         {return ExpCreate_SubEq(l, r, pos, false);}

      //
      // ExpCreate_SubEq
      //
      AST::Exp::CRef ExpCreate_SubEq(AST::Exp const *expL, AST::Exp const *r,
         Core::Origin pos, bool post)
      {
         if(!IsModLValue(expL))
            throw Core::ExceptStr(pos, "expected modifiable lvalue");

         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // pointer -= integer
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

            return Exp_SubPtrIntEq::Create(post, typeL, expL, expR, pos);
         }

         // arithmetic -= arithmetic
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
         {
            AST::Type::CPtr evalT;
            std::tie(evalT, std::ignore, expR) = ExpPromo_Arith(expL, expR, pos);

            return ExpCreate_ArithEq<AST::Exp_Sub, IR::CodeSet_Sub>(
               evalT, typeL, expL, expR, pos, post);
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator -='");
      }
   }
}

// EOF

