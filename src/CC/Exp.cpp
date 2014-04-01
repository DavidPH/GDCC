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

#include "AST/Arg.hpp"
#include "AST/Exp/Binary.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/CodeSet/Bitwise.hpp"
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
      // ExpCreate_BitAnd
      //
      AST::Exp::CRef ExpCreate_BitAnd(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_Bitwise<AST::Exp_BitAnd, IR::CodeSet_And>(l, r, pos);
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
      // ExpCreate_BitOrX
      //
      AST::Exp::CRef ExpCreate_BitOrX(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_Bitwise<AST::Exp_BitOrX, IR::CodeSet_OrX>(l, r, pos);
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
         auto typeR = e->getType();

         if(typeL->getTypeQual() == typeR->getTypeQual())
            return static_cast<AST::Exp::CRef>(e);

         if(typeL->isCTypeArith() && typeR->isCTypeArith())
            return ExpConvert_Arith(typeL, e, pos);

         if(typeL->isTypePointer() && typeR->isCTypeArith())
            return ExpConvert_PtrArith(typeL, e, pos);

         if(typeL->isTypePointer() && typeR->isTypePointer())
            return ExpConvert_Pointer(typeL, e, pos);

         throw Core::ExceptStr(pos, "unsupported cast");
      }

      AST::Exp::CRef ExpCreate_ShL(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_Shift<AST::Exp_ShL, IR::CodeSet_ShL>(l, r, pos);
      }

      AST::Exp::CRef ExpCreate_ShR(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         return ExpCreate_Shift<AST::Exp_ShR, IR::CodeSet_ShR>(l, r, pos);
      }

      //
      // ExpToFastU
      //
      Core::FastU ExpToFastU(AST::Exp const *exp)
      {
         auto i = ExpToInteg(exp);

         // TODO: Check range?

         return number_cast<Core::FastU>(i);
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
            return number_cast<Core::Integ>(val.vFloat.value);

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

      // Stubs.

      AST::Exp::CRef ExpConvert_Arith(AST::Type const *t, AST::Exp const *e,
         Core::Origin pos)
      {
         if(t->getTypeQual() == e->getType()->getTypeQual())
            return static_cast<AST::Exp::CRef>(e);
         throw Core::ExceptStr(pos, "convert arith stub");
      }

      Core::CounterRef<AST::Exp const> ExpConvert_Bool(AST::Type const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "convert bool stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_AddEq(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_Array(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_BitAndEq(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_BitOrIEq(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_BitOrXEq(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_CmpEQ(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}
      Core::CounterRef<AST::Exp const> ExpCreate_CmpGE(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}
      Core::CounterRef<AST::Exp const> ExpCreate_CmpGT(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}
      Core::CounterRef<AST::Exp const> ExpCreate_CmpLE(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}
      Core::CounterRef<AST::Exp const> ExpCreate_CmpLT(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}
      Core::CounterRef<AST::Exp const> ExpCreate_CmpNE(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_Cnd(AST::Exp const *,
         AST::Exp const *, AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_DecPre(AST::Exp const *,
         Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}
      Core::CounterRef<AST::Exp const> ExpCreate_DecSuf(AST::Exp const *,
         Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_DivEq(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_GenSel(AST::Exp const *,
         AST::Exp const *, Core::Array<GenAssoc> const &, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}
      Core::CounterRef<AST::Exp const> ExpCreate_GenSel(AST::Exp const *,
         AST::Exp const *, Core::Array<GenAssoc>      &&, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_IncPre(AST::Exp const *,
         Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}
      Core::CounterRef<AST::Exp const> ExpCreate_IncSuf(AST::Exp const *,
         Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_Inv(AST::Exp const *,
         Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_LogAnd(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_LogOrI(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_Mem(AST::Exp const *,
         Core::String, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_MemPt(AST::Exp const *,
         Core::String, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_ModEq(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_MulEq(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_Not(AST::Exp const *,
         Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_ShLEq(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_ShREq(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_SizeAlign(
         AST::Type const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_SizeBytes(
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}
      Core::CounterRef<AST::Exp const> ExpCreate_SizeBytes(
         AST::Type const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}

      Core::CounterRef<AST::Exp const> ExpCreate_SubEq(AST::Exp const *,
         AST::Exp const *, Core::Origin pos)
         {throw Core::ExceptStr(pos, "stub");}
   }
}

// EOF

