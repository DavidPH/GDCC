//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

#include "AST/Arg.hpp"
#include "AST/Exp/Binary.hpp"
#include "AST/Exp/Cnd.hpp"
#include "AST/Exp/Convert.hpp"
#include "AST/Exp/Logical.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/CodeSet/Bitwise.hpp"
#include "IR/CodeSet/Unary.hpp"
#include "IR/Exp.hpp"
#include "IR/Value.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // GenAssoc constructor
      //
      GenAssoc::GenAssoc(AST::Type const *type_, AST::Exp const *exp_) :
         type{type_},
         exp {exp_}
      {
      }

      //
      // ExpConvert_Arith
      //
      AST::Exp::CRef ExpConvert_Arith(AST::Type const *t, AST::Exp const *e,
         Core::Origin pos)
      {
         return AST::Exp_ConvertArith::Create(t, e, pos);
      }

      //
      // ExpConvert_Bool
      //
      AST::Exp::CRef ExpConvert_Bool(AST::Type const *t, AST::Exp const *e,
         Core::Origin pos)
      {
         return Exp_Not::Create(t, ExpCreate_Not(e, pos), pos);
      }

      //
      // ExpCreate_Array
      //
      AST::Exp::CRef ExpCreate_Array(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_Deref(ExpCreate_Add(l, r, pos), pos);
      }

      //
      // ExpCreate_BitAnd
      //
      AST::Exp::CRef ExpCreate_BitAnd(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_Bitwise<AST::Exp_BitAnd, IR::CodeSet_And>(l, r, pos);
      }

      //
      // ExpCreate_BitAndEq
      //
      AST::Exp::CRef ExpCreate_BitAndEq(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_BitwiseEq<AST::Exp_BitAnd, IR::CodeSet_And>(l, r, pos);
      }

      //
      // ExpCreate_BitOrI
      //
      AST::Exp::CRef ExpCreate_BitOrI(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_Bitwise<AST::Exp_BitOrI, IR::CodeSet_OrI>(l, r, pos);
      }

      //
      // ExpCreate_BitOrIEq
      //
      AST::Exp::CRef ExpCreate_BitOrIEq(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_BitwiseEq<AST::Exp_BitOrI, IR::CodeSet_OrI>(l, r, pos);
      }

      //
      // ExpCreate_BitOrX
      //
      AST::Exp::CRef ExpCreate_BitOrX(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_Bitwise<AST::Exp_BitOrX, IR::CodeSet_OrX>(l, r, pos);
      }

      //
      // ExpCreate_BitOrXEq
      //
      AST::Exp::CRef ExpCreate_BitOrXEq(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_BitwiseEq<AST::Exp_BitOrX, IR::CodeSet_OrX>(l, r, pos);
      }

      //
      // ExpCreate_Cnd
      //
      AST::Exp::CRef ExpCreate_Cnd(AST::Exp const *c, AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos)
      {
         AST::Type::CPtr type;
         AST::Exp::CRef expC{c}, expL{l}, expR{r};

         std::tie(type, expC, expL, expR) = ExpPromo_Cond(expC, expL, expR, pos);
         return AST::Exp_Cnd::Create(type, expC, expL, expR, pos);
      }

      //
      // ExpCreate_Comma
      //
      AST::Exp::CRef ExpCreate_Comma(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return AST::Exp_Pair::Create(l, r, pos);
      }

      //
      // ExpCreate_Cst
      //
      AST::Exp::CRef ExpCreate_Cst(AST::Type const *typeL, AST::Exp const *e,
         Core::Origin pos)
      {
         auto exp = ExpPromo_LValue(e, pos);

         auto typeR = exp->getType();

         if(typeL->getTypeQual() == typeR->getTypeQual())
            return static_cast<AST::Exp::CRef>(exp);

         if(typeL->isTypeVoid())
            return ExpConvert_Void(typeL, exp, pos);

         if(typeL->isTypeBoolean())
            return ExpConvert_Bool(typeL, exp, pos);

         if(typeL->isCTypeArith() && typeR->isCTypeArith())
            return ExpConvert_Arith(typeL, exp, pos);

         if(typeL->isTypePointer() && typeR->isCTypeArith())
            return ExpConvert_PtrArith(typeL, exp, pos);

         if(typeL->isTypePointer() && typeR->isTypePointer())
            return ExpConvert_Pointer(typeL, exp, pos);

         throw Core::ExceptStr(pos, "unsupported cast");
      }

      //
      // ExpCreate_DecPre
      //
      AST::Exp::CRef ExpCreate_DecPre(AST::Exp const *e, Core::Origin pos)
      {
         return ExpCreate_SubEq(e, ExpCreate_LitInt(TypeIntegPrS, 1, pos), pos, false);
      }

      //
      // ExpCreate_DecPre
      //
      AST::Exp::CRef ExpCreate_DecSuf(AST::Exp const *e, Core::Origin pos)
      {
         return ExpCreate_SubEq(e, ExpCreate_LitInt(TypeIntegPrS, 1, pos), pos, true);
      }

      //
      // ExpCreate_IncPre
      //
      AST::Exp::CRef ExpCreate_IncPre(AST::Exp const *e, Core::Origin pos)
      {
         return ExpCreate_AddEq(e, ExpCreate_LitInt(TypeIntegPrS, 1, pos), pos, false);
      }

      //
      // ExpCreate_IncSuf
      //
      AST::Exp::CRef ExpCreate_IncSuf(AST::Exp const *e, Core::Origin pos)
      {
         return ExpCreate_AddEq(e, ExpCreate_LitInt(TypeIntegPrS, 1, pos), pos, true);
      }

      //
      // ExpCreate_Inv
      //
      AST::Exp::CRef ExpCreate_Inv(AST::Exp const *e, Core::Origin pos)
      {
         auto exp  = ExpPromo_Int(ExpPromo_LValue(e, pos), pos);
         auto type = exp->getType();

         if(!type->isCTypeInteg())
            throw Core::ExceptStr(pos, "expected integer type");

         auto code = AST::ExpCode_ArithInteg<IR::CodeSet_Inv>(type);

         if(code == IR::Code::None)
            throw Core::ExceptStr(pos, "unsupported operand size");

         return AST::Exp_UnaryCode<AST::Exp_Inv>::Create(code, type, exp, pos);
      }

      //
      // ExpCreate_LogAnd
      //
      AST::Exp::CRef ExpCreate_LogAnd(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return AST::Exp_LogAnd::Create(TypeIntegPrS,
            ExpPromo_Cond(l, pos), ExpPromo_Cond(r, pos), pos);
      }

      //
      // ExpCreate_LogOrI
      //
      AST::Exp::CRef ExpCreate_LogOrI(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return AST::Exp_LogOrI::Create(TypeIntegPrS,
            ExpPromo_Cond(l, pos), ExpPromo_Cond(r, pos), pos);
      }

      //
      // ExpCreate_ShL
      //
      AST::Exp::CRef ExpCreate_ShL(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_Shift<AST::Exp_ShL, IR::CodeSet_ShL>(l, r, pos);
      }

      //
      // ExpCreate_ShLEq
      //
      AST::Exp::CRef ExpCreate_ShLEq(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_ShiftEq<AST::Exp_ShL, IR::CodeSet_ShL>(l, r, pos);
      }

      //
      // ExpCreate_ShR
      //
      AST::Exp::CRef ExpCreate_ShR(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_Shift<AST::Exp_ShR, IR::CodeSet_ShR>(l, r, pos);
      }

      //
      // ExpCreate_ShREq
      //
      AST::Exp::CRef ExpCreate_ShREq(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_ShiftEq<AST::Exp_ShR, IR::CodeSet_ShR>(l, r, pos);
      }

      //
      // ExpCreate_SizeAlign
      //
      AST::Exp::CRef ExpCreate_SizeAlign(AST::Type const *t, Core::Origin pos)
      {
         if(!t->isCTypeObject() || !t->isTypeComplete())
            throw Core::ExceptStr(pos, "expected complete object type");

         return ExpCreate_LitInt(TypeIntegPrU, t->getSizeAlign(), pos);
      }

      //
      // ExpCreate_SizeBytes
      //
      AST::Exp::CRef ExpCreate_SizeBytes(AST::Exp const *e, Core::Origin pos)
      {
         return ExpCreate_SizeBytes(e->getType(), pos);
      }

      //
      // ExpCreate_SizeBytes
      //
      AST::Exp::CRef ExpCreate_SizeBytes(AST::Type const *t, Core::Origin pos)
      {
         if(!t->isCTypeObject() || !t->isTypeComplete())
            throw Core::ExceptStr(pos, "expected complete object type");

         if(t->isTypeBitfield())
            throw Core::ExceptStr(pos, "cannot sizeof bitfield");

         return ExpCreate_LitInt(TypeIntegPrU, t->getSizeBytes(), pos);
      }

      //
      // ExpToFastU
      //
      Core::FastU ExpToFastU(AST::Exp const *exp)
      {
         auto i = ExpToInteg(exp);

         // TODO: Check range?

         return Core::NumberCast<Core::FastU>(i);
      }

      //
      // ExpToInteg
      //
      Core::Integ ExpToInteg(AST::Exp const *exp)
      {
         auto val = exp->getIRExp()->getValue();

         switch(val.v)
         {
         case IR::ValueBase::Fixed:
            return val.vFixed.value >> val.vFixed.vtype.bitsF;

         case IR::ValueBase::Float:
            return Core::NumberCast<Core::Integ>(val.vFloat.value);

         default:
            throw Core::ExceptStr(exp->pos, "invalid integer constant");
         }
      }

      //
      // IsLValue
      //
      bool IsLValue(AST::Exp const *exp)
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
      bool IsModLValue(AST::Exp const *exp)
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
}

// EOF

