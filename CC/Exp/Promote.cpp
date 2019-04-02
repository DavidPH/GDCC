//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C type promotion rules.
//
//-----------------------------------------------------------------------------

#include "CC/Exp.hpp"

#include "CC/Exp/Mem.hpp"
#include "CC/Factory.hpp"
#include "CC/Type.hpp"

#include "Core/Exception.hpp"

#include "SR/Exp.hpp"
#include "SR/Type.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Factory::expPromo_Arg
   //
   SR::Exp::CRef Factory::expPromo_Arg(SR::Exp const *e, Core::Origin pos)
   {
      auto exp = expPromo_Int(expPromo_LValue(e, pos), pos);

      auto type = exp->getType();

      // Promote float to double.
      if(type->getTypeQual() == TypeFloatRS)
         return expConvert_Arith(TypeFloatRSL, exp, pos);

      // Promote Aut* to Sta*.
      if(type->isTypePointer() &&
         type->getBaseType()->getQualAddr().base == IR::AddrBase::Aut)
      {
         return expConvert_Pointer(type->getBaseType()
            ->getTypeQualAddr({IR::AddrBase::Sta, Core::STR_})
            ->getTypePointer(), exp, pos);
      }

      return exp;
   }

   //
   // Factory::expPromo_Assign
   //
   SR::Exp::CRef Factory::expPromo_Assign(SR::Type const *t, SR::Exp const *e)
   {
      return expPromo_Assign(t, e, e->pos);
   }

   //
   // Factory::expPromo_Assign
   //
   SR::Exp::CRef Factory::expPromo_Assign(SR::Type const *typeL,
      SR::Exp const *e, Core::Origin pos)
   {
      auto exp   = expPromo_LValue(e, pos);
      auto typeR = exp->getType();

      // If assigning to same type, no conversion is needed.
      if(typeL->getTypeQual() == typeR->getTypeQual())
         return exp;

      // bool = pointer
      // bool = arithmetic
      if(typeL->isTypeBoolean() && (typeR->isTypePointer() || typeR->isCTypeArith()))
      {
         return expConvert_Bool(typeL, exp, pos);
      }

      // integer = line-special
      if(typeL->isCTypeInteg() && typeR->isTypePointer() &&
         typeR->getBaseType()->isTypeFunction() &&
         typeR->getBaseType()->getCallType() == IR::CallType::Special)
      {
         return expConvert_ArithPtr(typeL, exp, pos);
      }

      // arithmetic = arithmetic
      if(typeL->isCTypeArith())
      {
         if(!typeR->isCTypeArith())
            Core::Error(pos, "cannot implicitly convert to "
               "arithmetic type from non-arithmetic type");

         return expConvert_Arith(typeL, exp, pos);
      }

      // struct = struct
      if(typeL->isCTypeStruct() || typeL->isCTypeUnion())
      {
         if(typeL->getTypeQual() != typeR->getTypeQual())
            Core::Error(pos, "cannot implicitly convert to "
               "incompatible structure or union type");

         return exp;
      }

      // pointer = pointer
      if(typeL->isTypePointer())
      {
         // Check for exp being a null pointer constant.
         if(typeR->isCTypeInteg() && exp->isZero())
            return expConvert_PtrArith(typeL, exp, pos);

         if(!typeR->isTypePointer())
            Core::Error(pos, "cannot implicitly convert to "
               "pointer type from non-pointer type");

         auto baseL = typeL->getBaseType();
         auto baseR = typeR->getBaseType();

         // Treat __str_ent* as char const __str_ars* for these checks.
         if(baseR->isTypeStrEnt())
         {
            SR::TypeQual qual = {{IR::AddrBase::StrArs, Core::STR_}};
            qual.aCons = true;
            baseR = TypeChar->getTypeQual(qual);
         }

         // Check underlying type compatibility.
         if(!baseL->isTypeVoid() && !baseR->isTypeVoid() &&
            baseL->getTypeArrayQual() != baseR->getTypeArrayQual())
            Core::Error(pos, "cannot implicitly convert to "
               "incompatible pointer type");

         auto qualL = baseL->getQual();
         auto qualR = baseR->getQual();

         // Check address space compatibility.
         if(!Target::IsAddrEnclosed(qualL.space, qualR.space))
            Core::Error(pos, "cannot implicitly convert to "
               "pointer to disjoint address space");

         // Check for discarded qualifiers.
         if((!qualL.aAtom && qualR.aAtom) || (!qualL.aCons && qualR.aCons) ||
            (!qualL.aRest && qualR.aRest) || (!qualL.aVola && qualR.aVola))
            Core::Error(pos, "cannot implicitly discard qualifiers");

         return expConvert_Pointer(typeL, exp, pos);
      }

      Core::Error(pos, "cannot implicitly convert");
   }

   //
   // Factory::expPromo_CmpEqu
   //
   std::tuple<SR::Type::CRef, SR::Exp::CRef, SR::Exp::CRef>
   Factory::expPromo_CmpEqu(SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
   {
      auto expL = expPromo_LValue(l, pos);
      auto expR = expPromo_LValue(r, pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // Both operands have arithmetic types.
      if(typeL->isCTypeArith() && typeR->isCTypeArith())
         return expPromo_ArithAlways(expL, expR, pos);

      // Otherwise, they must be pointers.
      return expPromo_PtrEqu(expL, expR, pos);
   }

   //
   // Factory::expPromo_CmpRel
   //
   std::tuple<SR::Type::CRef, SR::Exp::CRef, SR::Exp::CRef>
   Factory::expPromo_CmpRel(SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
   {
      auto expL = expPromo_LValue(l, pos);
      auto expR = expPromo_LValue(r, pos);

      auto typeL = expL->getType();
      auto typeR = expR->getType();

      // C requires relational-expression operands to have real types.
      if(typeL->isCTypeReal() && typeR->isCTypeReal())
         return expPromo_ArithAlways(expL, expR, pos);

      // Otherwise, they must be pointers.
      return expPromo_PtrRel(expL, expR, pos);
   }

   //
   // Factory::expPromo_Cond
   //
   SR::Exp::CRef Factory::expPromo_Cond(SR::Exp const *e, Core::Origin pos)
   {
      SR::Exp::CRef exp = expPromo_LValue(e, pos);

      if(!exp->getType()->isCTypeScalar())
         Core::Error(pos, "expected scalar type");

      return expConvert_BoolSoft(exp, pos);
   }

   //
   // Factory::expPromo_Cond
   //
   std::tuple<SR::Type::CRef, SR::Exp::CRef, SR::Exp::CRef>
   Factory::expPromo_Cond(SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
   {
      auto expL = expPromo_LValue(l, pos);
      auto expR = expPromo_LValue(r, pos);

      auto typeL = expL->getType()->getTypeQual();
      auto typeR = expR->getType()->getTypeQual();

      // Both operands have arithmetic type.
      if(typeL->isCTypeArith() && typeR->isCTypeArith())
      {
         // Use normal arithmetic promotion.
         SR::Type::CRef type = SR::Type::None;
         std::tie(type, expL, expR) = expPromo_Arith(expL, expR, pos);

         // But always convert to the result type.
         expL = expConvert_Arith(type, expL, pos);
         expR = expConvert_Arith(type, expR, pos);

         return std::make_tuple(type, expL, expR);
      }

      // Both operands have the same structure or union type.
      if(typeL->isCTypeStruct() || typeL->isCTypeUnion() ||
         typeR->isCTypeStruct() || typeR->isCTypeUnion())
      {
         if(typeL != typeR)
            Core::Error(pos, "non-matching structure types");

         return std::make_tuple(typeL, expL, expR);
      }

      // Both operands have void type.
      if(typeL->isTypeVoid() && typeR->isTypeVoid())
         return std::make_tuple(SR::Type::Void, expL, expR);

      // Remaining constraints all concern pointers.
      return expPromo_PtrEqu(expL, expR, pos);
   }

   //
   // Factory::expPromo_Cond
   //
   std::tuple<SR::Type::CRef, SR::Exp::CRef, SR::Exp::CRef, SR::Exp::CRef>
   Factory::expPromo_Cond(SR::Exp const *c, SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      auto [type, expL, expR] = expPromo_Cond(l, r, pos);

      return std::make_tuple(type, expPromo_Cond(c, pos), expL, expR);
   }

   //
   // Factory::expPromo_Int
   //
   SR::Exp::CRef Factory::expPromo_Int(SR::Exp const *e, Core::Origin pos)
   {
      SR::Exp::CRef exp{e};
      auto          type = exp->getType()->getTypeQual();

      // Non integers are unaffected.
      if(!type->isCTypeInteg())
         return exp;

      // Bitfield of T -> T.
      if(type->isTypeBitfield())
         type = (exp = expConvert_Bitfield(exp, pos))->getType();

      // Types with a rank higher than int are unaffected.
      try
      {
         if(type->getRankC() > SR::TypeRankC::Integ)
            return exp;
      }
      catch(SR::TypeError const &)
      {
         Core::Error(pos, "unranked integer type");
      }

      // Int and unsigned int are unaffected.
      if(type == TypeIntegPrS || type == TypeIntegPrU)
         return exp;

      // If the original type can be represented in int, convert to that.
      if(type->getSizeBitsI() <= TypeIntegPrS->getSizeBitsI())
         return expConvert_Arith(TypeIntegPrS, exp, pos);
      // Otherwise, unsigned int.
      else
         return expConvert_Arith(TypeIntegPrU, exp, pos);
   }

   //
   // Factory::expPromo_LValue
   //
   SR::Exp::CRef Factory::expPromo_LValue(SR::Exp const *exp)
   {
      return expPromo_LValue(exp, exp->pos);
   }

   //
   // Factory::expPromo_LValue
   //
   SR::Exp::CRef Factory::expPromo_LValue(SR::Exp const *e, Core::Origin pos)
   {
      // Special check for structure property.
      if(auto exp = dynamic_cast<Exp_MemProp const *>(e))
         return exp->createExp_get();

      SR::Exp::CRef exp{e};
      auto          type = exp->getType();

      // Bitfield of T -> T.
      if(type->isTypeBitfield())
         type = (exp = expConvert_Bitfield(exp, pos))->getType();

      // Array of T -> pointer to T.
      if(type->isTypeArray())
      {
         // Mark exp as referred and then reacquire its type.
         auto ref = expCreate_Refer(exp, pos);
         type = exp->getType()->getBaseType()->getTypePointer();
         return expCreate_Cst(type, ref, pos);
      }

      // Function returning T -> pointer to function returning T.
      if(type->isCTypeFunction())
         return expCreate_Refer(exp, pos);

      // Conversion of lvalue to rvalue is internally implicit.
      return exp;
   }
}

// EOF

