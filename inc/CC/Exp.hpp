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

#ifndef GDCC__CC__Exp_H__
#define GDCC__CC__Exp_H__

#include "../CC/Types.hpp"

#include "../Core/Counter.hpp"
#include "../Core/Number.hpp"

#include <tuple>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // GenAssoc
   //
   class GenAssoc
   {
   public:
      GenAssoc(SR::Type const *type, SR::Exp const *exp);

      Core::CounterRef<SR::Type const> type;
      Core::CounterRef<SR::Exp const>  exp;
   };
}


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

namespace GDCC::CC
{
   extern Core::CounterRef<SR::Exp const> (*ExpPromo_Assign_Ptr)(
      SR::Type const *t, SR::Exp const *e, Core::Origin pos);
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   Core::CounterRef<SR::Exp const> ExpConvert_Arith(SR::Type const *t,
      SR::Exp const *e, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpConvert_ArithPtr(SR::Type const *t,
      SR::Exp const *e, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpConvert_Bitfield(SR::Exp const *e,
      Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpConvert_Bool(SR::Type const *t,
      SR::Exp const *e, Core::Origin pos);

   // Converts the expression to a soft boolean for conditional branching.
   // Will result in a hard boolean type if doing so is no more difficult.
   Core::CounterRef<SR::Exp const> ExpConvert_BoolSoft(SR::Exp const *e,
      Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpConvert_Pointer(SR::Type const *t,
      SR::Exp const *e, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpConvert_PtrArith(SR::Type const *t,
      SR::Exp const *e, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpConvert_Void(SR::Type const *t,
      SR::Exp const *e, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Add(SR::Exp const *e,
      Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Add(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_AddEq(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_AddEq(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos, bool post);

   Core::CounterRef<SR::Exp const> ExpCreate_Array(SR::Exp const *exp,
      SR::Exp const *idx, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Assign(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_BitAnd(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_BitAndEq(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_BitOrI(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_BitOrIEq(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_BitOrX(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_BitOrXEq(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Call(SR::Exp const *e,
      Core::Array<Core::CounterRef<SR::Exp const>> const &args,
      Scope &scope, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_Call(SR::Exp const *e,
      Core::Array<Core::CounterRef<SR::Exp const>>      &&args,
      Scope &scope, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_CmpEQ(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_CmpGE(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_CmpGT(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_CmpLE(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_CmpLT(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_CmpNE(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Cnd(SR::Exp const *c,
      SR::Exp const *l, SR::Exp const *r, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Comma(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Cst(SR::Type const *t,
      SR::Exp const *e);
   Core::CounterRef<SR::Exp const> ExpCreate_Cst(SR::Type const *t,
      SR::Exp const *e, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_DecPre(SR::Exp const *e,
      Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_DecSuf(SR::Exp const *e,
      Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Deref(SR::Exp const *e,
      Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Div(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_DivEq(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_DivEx(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Func(IR::Program &prog,
      SR::Function *fn, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_GenSel(SR::Exp const *exp,
      SR::Exp const *def, Core::Array<GenAssoc> const &a, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_GenSel(SR::Exp const *exp,
      SR::Exp const *def, Core::Array<GenAssoc>      &&a, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_IncPre(SR::Exp const *e,
      Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_IncSuf(SR::Exp const *e,
      Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Inv(SR::Exp const *e,
      Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_JmpLng(Scope &scope,
      SR::Exp const *env, SR::Exp const *val, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_JmpSet(SR::Exp const *env,
      Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_LitInt(SR::Type const *type,
      Core::Integ const &lit, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_LitInt(SR::Type const *type,
      Core::Integ      &&lit, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_LitNul(SR::Type const *type,
      Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_LogAnd(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_LogOrI(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Mem(SR::Exp const *l,
      Core::String r, Core::Origin pos, Scope &scope);

   Core::CounterRef<SR::Exp const> ExpCreate_MemPt(SR::Exp const *l,
      Core::String r, Core::Origin pos, Scope &scope);

   Core::CounterRef<SR::Exp const> ExpCreate_Mod(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_ModEq(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Mul(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_MulEq(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Not(SR::Exp const *e,
      Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Obj(IR::Program &prog,
      SR::Object *obj, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Refer(SR::Exp const *e,
      Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_ShL(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_ShLEq(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_ShR(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_ShREq(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_SizeAlign(
      SR::Type const *type, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_SizeBytes(
      SR::Exp const *type, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_SizeBytes(
      SR::Type const *type, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_StrIdx(IR::Program &prog,
      Scope &scope, Core::String lit, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_StrU08(IR::Program &prog,
      Scope &scope, Core::String lit, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_StrU16(IR::Program &prog,
      Scope &scope, Core::String lit, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_StrU32(IR::Program &prog,
      Scope &scope, Core::String lit, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_String(IR::Program &prog,
      Scope &scope, Core::String lit, Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Sub(SR::Exp const *e,
      Core::Origin pos);

   Core::CounterRef<SR::Exp const> ExpCreate_Sub(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_SubEq(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpCreate_SubEq(SR::Exp const *l,
      SR::Exp const *r, Core::Origin pos, bool post);

   Core::CounterRef<SR::Exp const> ExpCreate_VaArg(SR::Type const *type,
      SR::Exp const *exp, Core::Origin pos);

   // Default argument promotions.
   Core::CounterRef<SR::Exp const> ExpPromo_Arg(SR::Exp const *e,
      Core::Origin pos);

   // Usual arithmtic conversions.
   std::tuple<
      Core::CounterRef<SR::Type const> /*type*/,
      Core::CounterRef<SR::Exp  const> /*expL*/,
      Core::CounterRef<SR::Exp  const> /*expR*/>
   ExpPromo_Arith(SR::Exp const *l, SR::Exp const *r, Core::Origin pos);

   // Usual arithmetic conversions, always promoted to common type.
   std::tuple<
      Core::CounterRef<SR::Type const> /*type*/,
      Core::CounterRef<SR::Exp  const> /*expL*/,
      Core::CounterRef<SR::Exp  const> /*expR*/>
   ExpPromo_ArithAlways(SR::Exp const *l, SR::Exp const *r, Core::Origin pos);

   // Conversion as if by assignment.
   Core::CounterRef<SR::Exp const> ExpPromo_Assign(SR::Type const *t,
      SR::Exp const *e);
   Core::CounterRef<SR::Exp const> ExpPromo_Assign(SR::Type const *t,
      SR::Exp const *e, Core::Origin pos);
   Core::CounterRef<SR::Exp const> ExpPromo_Assign_Base(SR::Type const *t,
      SR::Exp const *e, Core::Origin pos);

   // Equality expression contraints and promotion.
   std::tuple<
      Core::CounterRef<SR::Type const> /*type*/,
      Core::CounterRef<SR::Exp  const> /*expL*/,
      Core::CounterRef<SR::Exp  const> /*expR*/>
   ExpPromo_CmpEqu(SR::Exp const *l, SR::Exp const *r, Core::Origin pos);

   // Relational expression contraints and promotion.
   std::tuple<
      Core::CounterRef<SR::Type const> /*type*/,
      Core::CounterRef<SR::Exp  const> /*expL*/,
      Core::CounterRef<SR::Exp  const> /*expR*/>
   ExpPromo_CmpRel(SR::Exp const *l, SR::Exp const *r, Core::Origin pos);

   // Promotes a branch condition to a soft boolean.
   Core::CounterRef<SR::Exp const> ExpPromo_Cond(SR::Exp const *e,
      Core::Origin pos);

   // Conditional expression contraints and promotion. (Center and right.)
   std::tuple<
      Core::CounterRef<SR::Type const> /*type*/,
      Core::CounterRef<SR::Exp  const> /*expL*/,
      Core::CounterRef<SR::Exp  const> /*expR*/>
   ExpPromo_Cond(SR::Exp const *l, SR::Exp const *r, Core::Origin pos);

   // Conditional expression contraints and promotion.
   std::tuple<
      Core::CounterRef<SR::Type const> /*type*/,
      Core::CounterRef<SR::Exp  const> /*expC*/,
      Core::CounterRef<SR::Exp  const> /*expL*/,
      Core::CounterRef<SR::Exp  const> /*expR*/>
   ExpPromo_Cond(SR::Exp const *c, SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos);

   // Integer promotion.
   Core::CounterRef<SR::Exp const> ExpPromo_Int(SR::Exp const *e,
      Core::Origin pos);

   // Lvalues, arrays, and function designators.
   Core::CounterRef<SR::Exp const> ExpPromo_LValue(SR::Exp const *e);
   Core::CounterRef<SR::Exp const> ExpPromo_LValue(SR::Exp const *e,
      Core::Origin pos);

   // Equality expression contraints and promotion for pointers.
   std::tuple<
      Core::CounterRef<SR::Type const> /*type*/,
      Core::CounterRef<SR::Exp  const> /*expL*/,
      Core::CounterRef<SR::Exp  const> /*expR*/>
   ExpPromo_PtrEqu(SR::Exp const *l, SR::Exp const *r, Core::Origin pos);

   // Relational expression contraints and promotion for pointers.
   std::tuple<
      Core::CounterRef<SR::Type const> /*type*/,
      Core::CounterRef<SR::Exp  const> /*expL*/,
      Core::CounterRef<SR::Exp  const> /*expR*/>
   ExpPromo_PtrRel(SR::Exp const *l, SR::Exp const *r, Core::Origin pos);

   Core::FastU ExpToFastU(SR::Exp const *exp);
   Core::Integ ExpToInteg(SR::Exp const *exp);

   // Is exp an lvalue?
   bool IsLValue(SR::Exp const *exp);

   // Is exp a modifiable lvalue?
   bool IsModLValue(SR::Exp const *exp);
}

#endif//GDCC__CC__Exp_H__

