//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C type promotion rules.
//
//-----------------------------------------------------------------------------

#include "CC/Exp.hpp"

#include "CC/Type.hpp"

#include "AST/Exp.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpPromo_Arg
      //
      AST::Exp::CRef ExpPromo_Arg(AST::Exp const *e, Core::Origin pos)
      {
         auto exp = ExpPromo_Int(e, pos);

         if(exp->getType()->getTypeQual() == TypeFloatRS)
            return ExpConvert_Arith(TypeFloatRSL, exp, pos);

         return exp;
      }

      //
      // ExpPromo_Assign
      //
      AST::Exp::CRef ExpPromo_Assign(AST::Type const *typeL, AST::Exp const *e,
         Core::Origin pos)
      {
         auto exp   = ExpPromo_LValue(e, pos);
         auto typeR = exp->getType();

         // If assigning to same type, no conversion is needed.
         if(typeL->getTypeQual() == typeR->getTypeQual())
            return exp;

         // bool = pointer
         if(typeL->isTypeBoolean() && typeR->isTypePointer())
         {
            return ExpConvert_Bool(typeL, exp, pos);
         }

         // arithmetic = arithmetic
         if(typeL->isCTypeArith())
         {
            if(!typeR->isCTypeArith())
               throw Core::ExceptStr(pos, "cannot implicitly convert to "
                  "arithmetic type from non-arithmetic type");

            return ExpConvert_Arith(typeL, exp, pos);
         }

         // struct = struct
         if(typeL->isCTypeStruct() || typeL->isCTypeUnion())
         {
            if(typeL->getTypeQual() != typeR->getTypeQual())
               throw Core::ExceptStr(pos, "cannot implicitly convert to "
                  "incompatible structure or union type");

            return exp;
         }

         // pointer = pointer
         if(typeL->isTypePointer())
         {
            // Check for exp being a null pointer constant.
            if(typeR->isCTypeInteg() && exp->isZero())
               return ExpConvert_PtrArith(typeL, exp, pos);

            if(!typeR->isTypePointer())
               throw Core::ExceptStr(pos, "cannot implicitly convert to "
                  "pointer type from non-pointer type");

            auto baseL = typeL->getBaseType();
            auto baseR = typeR->getBaseType();

            // Check underlying type compatibility.
            if(!baseL->isTypeVoid() && !baseR->isTypeVoid() &&
               baseL->getTypeQual() != baseR->getTypeQual())
               throw Core::ExceptStr(pos, "cannot implicitly convert to "
                  "incompatible pointer type");

            auto qualL = baseL->getQual();
            auto qualR = baseR->getQual();

            // Check address space compatibility.
            if(!IR::IsAddrEnclosed(qualL.space, qualR.space))
               throw Core::ExceptStr(pos, "cannot implicitly convert to "
                  "pointer to disjoint address space");

            // Check for discarded qualifiers.
            if((!qualL.aAtom && qualR.aAtom) || (!qualL.aCons && qualR.aCons) ||
               (!qualL.aRest && qualR.aRest) || (!qualL.aVola && qualR.aVola))
               throw Core::ExceptStr(pos, "cannot implicitly discard qualifiers");

            return ExpConvert_Pointer(typeL, exp, pos);
         }

         throw Core::ExceptStr(pos, "cannot implicitly convert");
      }

      //
      // ExpPromo_CmpEqu
      //
      std::tuple<AST::Type::CRef, AST::Exp::CRef, AST::Exp::CRef>
      ExpPromo_CmpEqu(AST::Exp const *l, AST::Exp const *r, Core::Origin pos)
      {
         auto expL = ExpPromo_LValue(l, pos);
         auto expR = ExpPromo_LValue(r, pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // Both operands have arithmetic types.
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
            return ExpPromo_Arith(expL, expR, pos);

         // Otherwise, they must be pointers.
         return ExpPromo_PtrEqu(expL, expR, pos);
      }

      //
      // ExpPromo_CmpRel
      //
      std::tuple<AST::Type::CRef, AST::Exp::CRef, AST::Exp::CRef>
      ExpPromo_CmpRel(AST::Exp const *l, AST::Exp const *r, Core::Origin pos)
      {
         auto expL = ExpPromo_LValue(l, pos);
         auto expR = ExpPromo_LValue(r, pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // C requires relational-expression operands to have real types.
         if(typeL->isCTypeReal() && typeR->isCTypeReal())
            return ExpPromo_Arith(expL, expR, pos);

         // Otherwise, they must be pointers.
         return ExpPromo_PtrRel(expL, expR, pos);
      }

      //
      // ExpPromo_Cond
      //
      AST::Exp::CRef ExpPromo_Cond(AST::Exp const *exp, Core::Origin pos)
      {
         if(!exp->getType()->isCTypeScalar())
            throw Core::ExceptStr(pos, "expected scalar type");

         return ExpConvert_BoolSoft(exp, pos);
      }

      //
      // ExpPromo_Cond
      //
      std::tuple<AST::Type::CRef, AST::Exp::CRef, AST::Exp::CRef>
      ExpPromo_Cond(AST::Exp const *l, AST::Exp const *r, Core::Origin pos)
      {
         auto expL = ExpPromo_LValue(l, pos);
         auto expR = ExpPromo_LValue(r, pos);

         auto typeL = expL->getType()->getTypeQual();
         auto typeR = expR->getType()->getTypeQual();

         // Both operands have arithmetic type.
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
         {
            // Use normal arithmetic promotion.
            AST::Type::CRef type = AST::Type::None;
            std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);

            // But always convert to the result type.
            expL = ExpConvert_Arith(type, expL, pos);
            expR = ExpConvert_Arith(type, expR, pos);

            return std::make_tuple(type, expL, expR);
         }

         // Both operands have the same structure or union type.
         if(typeL->isCTypeStruct() || typeL->isCTypeUnion() ||
            typeR->isCTypeStruct() || typeR->isCTypeUnion())
         {
            if(typeL != typeR)
               throw Core::ExceptStr(pos, "non-matching structure types");

            return std::make_tuple(typeL, expL, expR);
         }

         // Both operands have void type.
         if(typeL->isTypeVoid() && typeR->isTypeVoid())
            return std::make_tuple(AST::Type::Void, expL, expR);

         // Remaining constraints all concern pointers.
         return ExpPromo_PtrEqu(expL, expR, pos);
      }

      //
      // ExpPromo_Cond
      //
      std::tuple<AST::Type::CRef, AST::Exp::CRef, AST::Exp::CRef, AST::Exp::CRef>
      ExpPromo_Cond(AST::Exp const *c, AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         AST::Type::CRef type = AST::Type::None;
         AST::Exp::CRef expL{l}, expR{r};

         std::tie(type, expL, expR) = ExpPromo_Cond(expL, expR, pos);

         return std::make_tuple(type, ExpPromo_Cond(c, pos), expL, expR);
      }

      //
      // ExpPromo_Int
      //
      AST::Exp::CRef ExpPromo_Int(AST::Exp const *exp, Core::Origin pos)
      {
         auto type = exp->getType()->getTypeQual();

         // Non integers are unaffected.
         if(!type->isCTypeInteg())
            return static_cast<AST::Exp::CRef>(exp);

         auto rank = type->getRankC();

         // Types with a rank higher than int are unaffected.
         if(rank > AST::TypeRankC::Integ)
            return static_cast<AST::Exp::CRef>(exp);

         // Int and unsigned int are unaffected.
         if(type == TypeIntegPrS || type == TypeIntegPrU)
            return static_cast<AST::Exp::CRef>(exp);

         // If the original type can be represented in int, convert to that.
         if(type->getSizeBitsI() <= TypeIntegPrS->getSizeBitsI())
            return ExpConvert_Arith(TypeIntegPrS, exp, pos);
         // Otherwise, unsigned int.
         else
            return ExpConvert_Arith(TypeIntegPrU, exp, pos);
      }

      //
      // ExpPromo_LValue
      //
      AST::Exp::CRef ExpPromo_LValue(AST::Exp const *exp, Core::Origin pos)
      {
         auto type = exp->getType();

         // Array of T -> pointer to T.
         if(type->isTypeArray())
         {
            // Mark exp as referred and then reacquire its type.
            auto ref = ExpCreate_Refer(exp, pos);
            type = exp->getType()->getBaseType()->getTypePointer();
            return ExpCreate_Cst(type, ref, pos);
         }

         // Function returning T -> pointer to function returning T.
         if(type->isCTypeFunction())
            return ExpCreate_Refer(exp, pos);

         // Conversion of lvalue to rvalue is internally implicit.
         return static_cast<AST::Exp::CRef>(exp);
      }
   }
}

// EOF

