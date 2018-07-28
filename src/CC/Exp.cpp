//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp.hpp"

#include "CC/Exp/Bitwise.hpp"
#include "CC/Exp/Not.hpp"

#include "Core/Exception.hpp"

#include "IR/CodeSet/Bitwise.hpp"
#include "IR/CodeSet/Unary.hpp"
#include "IR/Exp.hpp"
#include "IR/Value.hpp"

#include "SR/Arg.hpp"
#include "SR/Exp/Binary.hpp"
#include "SR/Exp/Cnd.hpp"
#include "SR/Exp/Convert.hpp"
#include "SR/Exp/Logical.hpp"
#include "SR/Type.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC::CC
{
   Core::CounterRef<SR::Exp const> (*ExpPromo_Assign_Ptr)(SR::Type const *t,
      SR::Exp const *e, Core::Origin pos) = ExpPromo_Assign_Base;
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // GenAssoc constructor
   //
   GenAssoc::GenAssoc(SR::Type const *type_, SR::Exp const *exp_) :
      type{type_},
      exp {exp_}
   {
   }

   //
   // ExpConvert_Arith
   //
   SR::Exp::CRef ExpConvert_Arith(SR::Type const *t, SR::Exp const *e,
      Core::Origin pos)
   {
      return SR::Exp_ConvertArith::Create(t, e, pos);
   }

   //
   // ExpConvert_Bitfield
   //
   SR::Exp::CRef ExpConvert_Bitfield(SR::Exp const *e, Core::Origin pos)
   {
      return SR::Exp_ConvertBitfield::Create(e->getType()->getBaseType(), e, pos);
   }

   //
   // ExpConvert_Bool
   //
   SR::Exp::CRef ExpConvert_Bool(SR::Type const *t, SR::Exp const *e,
      Core::Origin pos)
   {
      return Exp_Not::Create(t, ExpCreate_Not(e, pos), pos);
   }

   //
   // ExpCreate_Array
   //
   SR::Exp::CRef ExpCreate_Array(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_Deref(ExpCreate_Add(l, r, pos), pos);
   }

   //
   // ExpCreate_BitAnd
   //
   SR::Exp::CRef ExpCreate_BitAnd(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_Bitwise<SR::Exp_BitAnd, IR::CodeSet_And>(l, r, pos);
   }

   //
   // ExpCreate_BitAndEq
   //
   SR::Exp::CRef ExpCreate_BitAndEq(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_BitwiseEq<SR::Exp_BitAnd, IR::CodeSet_And>(l, r, pos);
   }

   //
   // ExpCreate_BitOrI
   //
   SR::Exp::CRef ExpCreate_BitOrI(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_Bitwise<SR::Exp_BitOrI, IR::CodeSet_OrI>(l, r, pos);
   }

   //
   // ExpCreate_BitOrIEq
   //
   SR::Exp::CRef ExpCreate_BitOrIEq(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_BitwiseEq<SR::Exp_BitOrI, IR::CodeSet_OrI>(l, r, pos);
   }

   //
   // ExpCreate_BitOrX
   //
   SR::Exp::CRef ExpCreate_BitOrX(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_Bitwise<SR::Exp_BitOrX, IR::CodeSet_OrX>(l, r, pos);
   }

   //
   // ExpCreate_BitOrXEq
   //
   SR::Exp::CRef ExpCreate_BitOrXEq(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_BitwiseEq<SR::Exp_BitOrX, IR::CodeSet_OrX>(l, r, pos);
   }

   //
   // ExpCreate_Cnd
   //
   SR::Exp::CRef ExpCreate_Cnd(SR::Exp const *c, SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos)
   {
      SR::Type::CPtr type;
      SR::Exp::CRef expC{c}, expL{l}, expR{r};

      std::tie(type, expC, expL, expR) = ExpPromo_Cond(expC, expL, expR, pos);
      return SR::Exp_Cnd::Create(type, expC, expL, expR, pos);
   }

   //
   // ExpCreate_Comma
   //
   SR::Exp::CRef ExpCreate_Comma(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return SR::Exp_Pair::Create(l, ExpPromo_LValue(r, pos), pos);
   }

   //
   // ExpCreate_Cst
   //
   SR::Exp::CRef ExpCreate_Cst(SR::Type const *typeL, SR::Exp const *e)
   {
      return ExpCreate_Cst(typeL, e, e->pos);
   }

   //
   // ExpCreate_Cst
   //
   SR::Exp::CRef ExpCreate_Cst(SR::Type const *typeL, SR::Exp const *e,
      Core::Origin pos)
   {
      auto exp = ExpPromo_LValue(e, pos);

      auto typeR = exp->getType();

      if(typeL->getTypeQual() == typeR->getTypeQual())
         return static_cast<SR::Exp::CRef>(exp);

      if(typeL->isTypeVoid())
         return ExpConvert_Void(typeL, exp, pos);

      if(typeL->isTypeBoolean())
         return ExpConvert_Bool(typeL, exp, pos);

      if(typeL->isCTypeArith() && typeR->isCTypeArith())
         return ExpConvert_Arith(typeL, exp, pos);

      if(typeL->isCTypeArith() && typeR->isTypePointer())
         return ExpConvert_ArithPtr(typeL, exp, pos);

      if(typeL->isTypePointer() && typeR->isCTypeArith())
         return ExpConvert_PtrArith(typeL, exp, pos);

      if(typeL->isTypePointer() && typeR->isTypePointer())
         return ExpConvert_Pointer(typeL, exp, pos);

      Core::Error(pos, "unsupported cast");
   }

   //
   // ExpCreate_DecPre
   //
   SR::Exp::CRef ExpCreate_DecPre(SR::Exp const *e, Core::Origin pos)
   {
      return ExpCreate_SubEq(e, ExpCreate_LitInt(TypeIntegPrS, 1, pos), pos, false);
   }

   //
   // ExpCreate_DecPre
   //
   SR::Exp::CRef ExpCreate_DecSuf(SR::Exp const *e, Core::Origin pos)
   {
      return ExpCreate_SubEq(e, ExpCreate_LitInt(TypeIntegPrS, 1, pos), pos, true);
   }

   //
   // ExpCreate_IncPre
   //
   SR::Exp::CRef ExpCreate_IncPre(SR::Exp const *e, Core::Origin pos)
   {
      return ExpCreate_AddEq(e, ExpCreate_LitInt(TypeIntegPrS, 1, pos), pos, false);
   }

   //
   // ExpCreate_IncSuf
   //
   SR::Exp::CRef ExpCreate_IncSuf(SR::Exp const *e, Core::Origin pos)
   {
      return ExpCreate_AddEq(e, ExpCreate_LitInt(TypeIntegPrS, 1, pos), pos, true);
   }

   //
   // ExpCreate_Inv
   //
   SR::Exp::CRef ExpCreate_Inv(SR::Exp const *e, Core::Origin pos)
   {
      auto exp  = ExpPromo_Int(ExpPromo_LValue(e, pos), pos);
      auto type = exp->getType();

      if(!type->isCTypeInteg())
         Core::Error(pos, "expected integer type");

      auto code = SR::ExpCode_ArithInteg<IR::CodeSet_Inv>(type);

      if(code == IR::Code::None)
         Core::Error(pos, "unsupported operand size");

      return SR::Exp_UnaryCode<SR::Exp_Inv>::Create(code, type, exp, pos);
   }

   //
   // ExpCreate_LogAnd
   //
   SR::Exp::CRef ExpCreate_LogAnd(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return SR::Exp_LogAnd::Create(TypeIntegPrS,
         ExpPromo_Cond(l, pos), ExpPromo_Cond(r, pos), pos);
   }

   //
   // ExpCreate_LogOrI
   //
   SR::Exp::CRef ExpCreate_LogOrI(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return SR::Exp_LogOrI::Create(TypeIntegPrS,
         ExpPromo_Cond(l, pos), ExpPromo_Cond(r, pos), pos);
   }

   //
   // ExpCreate_ShL
   //
   SR::Exp::CRef ExpCreate_ShL(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_Shift<SR::Exp_ShL, IR::CodeSet_ShL>(l, r, pos);
   }

   //
   // ExpCreate_ShLEq
   //
   SR::Exp::CRef ExpCreate_ShLEq(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_ShiftEq<SR::Exp_ShL, IR::CodeSet_ShL>(l, r, pos);
   }

   //
   // ExpCreate_ShR
   //
   SR::Exp::CRef ExpCreate_ShR(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_Shift<SR::Exp_ShR, IR::CodeSet_ShR>(l, r, pos);
   }

   //
   // ExpCreate_ShREq
   //
   SR::Exp::CRef ExpCreate_ShREq(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_ShiftEq<SR::Exp_ShR, IR::CodeSet_ShR>(l, r, pos);
   }

   //
   // ExpCreate_SizeAlign
   //
   SR::Exp::CRef ExpCreate_SizeAlign(SR::Type const *t, Core::Origin pos)
   {
      if(!t->isCTypeObject() || !t->isTypeComplete())
         Core::Error(pos, "expected complete object type");

      return ExpCreate_LitInt(TypeIntegPrU, t->getSizeAlign(), pos);
   }

   //
   // ExpCreate_SizeBytes
   //
   SR::Exp::CRef ExpCreate_SizeBytes(SR::Exp const *e, Core::Origin pos)
   {
      return ExpCreate_SizeBytes(e->getType(), pos);
   }

   //
   // ExpCreate_SizeBytes
   //
   SR::Exp::CRef ExpCreate_SizeBytes(SR::Type const *t, Core::Origin pos)
   {
      if(!t->isCTypeObject() || !t->isTypeComplete())
         Core::Error(pos, "expected complete object type");

      if(t->isTypeBitfield())
         Core::Error(pos, "cannot sizeof bitfield");

      return ExpCreate_LitInt(TypeIntegPrU, t->getSizeBytes(), pos);
   }

   //
   // ExpToFastU
   //
   Core::FastU ExpToFastU(SR::Exp const *exp)
   {
      auto i = ExpToInteg(exp);

      // TODO: Check range?

      return Core::NumberCast<Core::FastU>(i);
   }

   //
   // ExpToInteg
   //
   Core::Integ ExpToInteg(SR::Exp const *exp)
   {
      auto val = exp->getIRExp()->getValue();

      switch(val.v)
      {
      case IR::ValueBase::Fixed:
         return val.vFixed.value >> val.vFixed.vtype.bitsF;

      case IR::ValueBase::Float:
         return Core::NumberCast<Core::Integ>(val.vFloat.value);

      default:
         Core::Error(exp->pos, "invalid integer constant");
      }
   }

   //
   // IsLValue
   //
   bool IsLValue(SR::Exp const *exp)
   {
      switch(exp->getArg().type->getQualAddr().base)
      {
      case IR::AddrBase::Cpy:
      case IR::AddrBase::Lit:
      case IR::AddrBase::Nul:
      case IR::AddrBase::Stk:
         return false;

      default:
         return exp->getType()->isCTypeObject();
      }
   }

   //
   // IsModLValue
   //
   bool IsModLValue(SR::Exp const *exp)
   {
      auto type = exp->getType();

      if(!IsLValue(exp))          return false;
      if(type->isTypeArray())     return false;
      if(!type->isTypeComplete()) return false;
      if(type->getQualCons())     return false;
      // TODO: Check struct/union members.

      return true;
   }
}

// EOF

