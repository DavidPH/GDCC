//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
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

#include "SR/Exp/Arg.hpp"
#include "SR/Exp/Binary.hpp"
#include "SR/Exp/Cnd.hpp"
#include "SR/Exp/Convert.hpp"
#include "SR/Exp/IRExp.hpp"
#include "SR/Exp/Logical.hpp"
#include "SR/Type.hpp"

#include <vector>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
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

   //
   // Factory::expCreate_Arg
   //
   SR::Exp::CRef Factory::expCreate_Arg(SR::Arg const &arg, Core::Origin pos)
   {
      return SR::Exp_Arg::Create(arg, pos);
   }

   //
   // Factory::expConvert_Arith
   //
   SR::Exp::CRef Factory::expConvert_Arith(SR::Type const *t, SR::Exp const *e,
      Core::Origin pos)
   {
      return SR::Exp_ConvertArith::Create(t, e, pos);
   }

   //
   // Factory::expConvert_Bitfield
   //
   SR::Exp::CRef Factory::expConvert_Bitfield(SR::Exp const *e, Core::Origin pos)
   {
      return SR::Exp_ConvertBitfield::Create(e->getType()->getBaseType(), e, pos);
   }

   //
   // Factory::expConvert_Bool
   //
   SR::Exp::CRef Factory::expConvert_Bool(SR::Type const *t, SR::Exp const *e,
      Core::Origin pos)
   {
      return Exp_Not::Create(t, expCreate_Not(e, pos), pos);
   }

   //
   // Factory::expCreate_Array
   //
   SR::Exp::CRef Factory::expCreate_Array(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return expCreate_Deref(expCreate_Add(l, r, pos), pos);
   }

   //
   // Factory::expCreate_BitAnd
   //
   SR::Exp::CRef Factory::expCreate_BitAnd(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_Bitwise<SR::Exp_BitAnd, IR::CodeSet_And>(*this, l, r, pos);
   }

   //
   // Factory::expCreate_BitAndEq
   //
   SR::Exp::CRef Factory::expCreate_BitAndEq(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_BitwiseEq<SR::Exp_BitAnd, IR::CodeSet_And>(*this, l, r, pos);
   }

   //
   // Factory::expCreate_BitOrI
   //
   SR::Exp::CRef Factory::expCreate_BitOrI(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_Bitwise<SR::Exp_BitOrI, IR::CodeSet_OrI>(*this, l, r, pos);
   }

   //
   // Factory::expCreate_BitOrIEq
   //
   SR::Exp::CRef Factory::expCreate_BitOrIEq(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_BitwiseEq<SR::Exp_BitOrI, IR::CodeSet_OrI>(*this, l, r, pos);
   }

   //
   // Factory::expCreate_BitOrX
   //
   SR::Exp::CRef Factory::expCreate_BitOrX(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_Bitwise<SR::Exp_BitOrX, IR::CodeSet_OrX>(*this, l, r, pos);
   }

   //
   // Factory::expCreate_BitOrXEq
   //
   SR::Exp::CRef Factory::expCreate_BitOrXEq(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_BitwiseEq<SR::Exp_BitOrX, IR::CodeSet_OrX>(*this, l, r, pos);
   }

   //
   // Factory::expCreate_Cnd
   //
   SR::Exp::CRef Factory::expCreate_Cnd(SR::Exp const *c, SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos)
   {
      auto [type, expC, expL, expR] = expPromo_Cond(c, l, r, pos);
      return SR::Exp_Cnd::Create(type, expC, expL, expR, pos);
   }

   //
   // Factory::expCreate_Comma
   //
   SR::Exp::CRef Factory::expCreate_Comma(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return SR::Exp_Pair::Create(l, expPromo_LValue(r, pos), pos);
   }

   //
   // Factory::expCreate_Cst
   //
   SR::Exp::CRef Factory::expCreate_Cst(SR::Type const *typeL, SR::Exp const *e)
   {
      return expCreate_Cst(typeL, e, e->pos);
   }

   //
   // Factory::expCreate_Cst
   //
   SR::Exp::CRef Factory::expCreate_Cst(SR::Type const *typeL, SR::Exp const *e,
      Core::Origin pos)
   {
      auto exp = expPromo_LValue(e, pos);

      auto typeR = exp->getType();

      if(typeL->getTypeQual() == typeR->getTypeQual())
         return static_cast<SR::Exp::CRef>(exp);

      if(typeL->isTypeVoid())
         return expConvert_Void(typeL, exp, pos);

      if(typeL->isTypeBoolean())
         return expConvert_Bool(typeL, exp, pos);

      if(typeL->isCTypeArith() && typeR->isCTypeArith())
         return expConvert_Arith(typeL, exp, pos);

      if(typeL->isCTypeArith() && typeR->isTypePointer())
         return expConvert_ArithPtr(typeL, exp, pos);

      if(typeL->isTypePointer() && typeR->isCTypeArith())
         return expConvert_PtrArith(typeL, exp, pos);

      if(typeL->isTypePointer() && typeR->isTypePointer())
         return expConvert_Pointer(typeL, exp, pos);

      Core::Error(pos, "unsupported cast");
   }

   //
   // Factory::expCreate_DecPre
   //
   SR::Exp::CRef Factory::expCreate_DecPre(SR::Exp const *e, Core::Origin pos)
   {
      return expCreate_SubEq(e, expCreate_LitInt(TypeIntegPrS, 1, pos), pos, false);
   }

   //
   // Factory::expCreate_DecSuf
   //
   SR::Exp::CRef Factory::expCreate_DecSuf(SR::Exp const *e, Core::Origin pos)
   {
      return expCreate_SubEq(e, expCreate_LitInt(TypeIntegPrS, 1, pos), pos, true);
   }

   //
   // Factory::expCreate_IRExp
   //
   SR::Exp::CRef Factory::expCreate_IRExp(IR::Exp const *exp, SR::Type const *type)
   {
      return expCreate_IRExp(exp, type, exp->pos);
   }

   //
   // Factory::expCreate_IRExp
   //
   SR::Exp::CRef Factory::expCreate_IRExp(IR::Exp const *exp, SR::Type const *type,
      Core::Origin pos)
   {
      return SR::Exp_IRExp::Create(exp, type, pos);
   }

   //
   // Factory::expCreate_IncPre
   //
   SR::Exp::CRef Factory::expCreate_IncPre(SR::Exp const *e, Core::Origin pos)
   {
      return expCreate_AddEq(e, expCreate_LitInt(TypeIntegPrS, 1, pos), pos, false);
   }

   //
   // Factory::expCreate_IncSuf
   //
   SR::Exp::CRef Factory::expCreate_IncSuf(SR::Exp const *e, Core::Origin pos)
   {
      return expCreate_AddEq(e, expCreate_LitInt(TypeIntegPrS, 1, pos), pos, true);
   }

   //
   // Factory::expCreate_Inv
   //
   SR::Exp::CRef Factory::expCreate_Inv(SR::Exp const *e, Core::Origin pos)
   {
      auto exp  = expPromo_Int(expPromo_LValue(e, pos), pos);
      auto type = exp->getType();

      if(!type->isCTypeInteg())
         Core::Error(pos, "expected integer type");

      auto code = SR::ExpCode_ArithInteg<IR::CodeSet_Inv>(type);

      if(code == IR::Code::None)
         Core::Error(pos, "unsupported operand size");

      return SR::Exp_UnaryCode<SR::Exp_Inv>::Create(code, type, exp, pos);
   }

   //
   // Factory::expCreate_LogAnd
   //
   SR::Exp::CRef Factory::expCreate_LogAnd(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return SR::Exp_LogAnd::Create(TypeIntegPrS,
         expPromo_Cond(l, pos), expPromo_Cond(r, pos), pos);
   }

   //
   // Factory::expCreate_LogOrI
   //
   SR::Exp::CRef Factory::expCreate_LogOrI(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return SR::Exp_LogOrI::Create(TypeIntegPrS,
         expPromo_Cond(l, pos), expPromo_Cond(r, pos), pos);
   }

   //
   // Factory::expCreate_ShL
   //
   SR::Exp::CRef Factory::expCreate_ShL(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_Shift<SR::Exp_ShL, IR::CodeSet_ShL>(*this, l, r, pos);
   }

   //
   // Factory::expCreate_ShLEq
   //
   SR::Exp::CRef Factory::expCreate_ShLEq(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_ShiftEq<SR::Exp_ShL, IR::CodeSet_ShL>(*this, l, r, pos);
   }

   //
   // Factory::expCreate_ShR
   //
   SR::Exp::CRef Factory::expCreate_ShR(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_Shift<SR::Exp_ShR, IR::CodeSet_ShR>(*this, l, r, pos);
   }

   //
   // Factory::expCreate_ShREq
   //
   SR::Exp::CRef Factory::expCreate_ShREq(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      return ExpCreate_ShiftEq<SR::Exp_ShR, IR::CodeSet_ShR>(*this, l, r, pos);
   }

   //
   // Factory::expCreate_SizeAlign
   //
   SR::Exp::CRef Factory::expCreate_SizeAlign(SR::Type const *t, Core::Origin pos)
   {
      if(!t->isCTypeObject() || !t->isTypeComplete())
         Core::Error(pos, "expected complete object type");

      return expCreate_LitInt(TypeIntegPrU,
         Core::NumberCast<Core::Integ>(t->getSizeAlign()), pos);
   }

   //
   // Factory::expCreate_SizeBytes
   //
   SR::Exp::CRef Factory::expCreate_SizeBytes(SR::Exp const *e, Core::Origin pos)
   {
      return expCreate_SizeBytes(e->getType(), pos);
   }

   //
   // Factory::expCreate_SizeBytes
   //
   SR::Exp::CRef Factory::expCreate_SizeBytes(SR::Type const *t, Core::Origin pos)
   {
      if(!t->isCTypeObject() || !t->isTypeComplete())
         Core::Error(pos, "expected complete object type");

      if(t->isTypeBitfield())
         Core::Error(pos, "cannot sizeof bitfield");

      return expCreate_LitInt(TypeIntegPrU,
         Core::NumberCast<Core::Integ>(t->getSizeBytes()), pos);
   }

   //
   // GenAssoc constructor
   //
   GenAssoc::GenAssoc(SR::Type const *type_, SR::Exp const *exp_) :
      type{type_},
      exp {exp_}
   {
   }
}

// EOF

