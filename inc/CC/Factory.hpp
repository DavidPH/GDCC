//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C expression/statement creation.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Factory_H__
#define GDCC__CC__Factory_H__

#include "../CC/Types.hpp"

#include "../Core/Number.hpp"

#include <tuple>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Factory
   //
   class Factory
   {
   protected:
      using Exp    = SR::Exp;
      using ExpRef = Core::CounterRef<Exp const>;

      using Labels = Core::Array<Core::String>;

      using Origin = Core::Origin;

      using St    = SR::Statement;
      using StRef = Core::CounterRef<St const>;

      using Type    = SR::Type;
      using TypeRef = Core::CounterRef<Type const>;

   public:
      virtual ExpRef expCreate_Arg(SR::Arg const &arg, Origin pos);

      virtual ExpRef expConvert_Arith(Type const *t, Exp const *e, Origin pos);

      virtual ExpRef expConvert_ArithPtr(Type const *t, Exp const *e, Origin pos);

      virtual ExpRef expConvert_Bitfield(Exp const *e, Origin pos);

      virtual ExpRef expConvert_Bool(Type const *t, Exp const *e, Origin pos);

      // Converts the expression to a soft boolean for conditional branching.
      // Will result in a hard boolean type if doing so is no more difficult.
      virtual ExpRef expConvert_BoolSoft(Exp const *e, Origin pos);

      virtual ExpRef expConvert_Pointer(Type const *t, Exp const *e, Origin pos);

      virtual ExpRef expConvert_PtrArith(Type const *t, Exp const *e, Origin pos);

      virtual ExpRef expConvert_Void(Type const *t, Exp const *e, Origin pos);

      virtual ExpRef expCreate_Add(Exp const *e, Origin pos);

      virtual ExpRef expCreate_Add(Exp const *l, Exp const *r, Origin pos);
      ExpRef expCreate_AddEq(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_AddEq(Exp const *l, Exp const *r, Origin pos, bool post);

      virtual ExpRef expCreate_Array(Exp const *exp, Exp const *idx, Origin pos);

      virtual ExpRef expCreate_Assign(Exp const *l, Exp const *r, Origin pos);

      virtual ExpRef expCreate_BitAnd(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_BitAndEq(Exp const *l, Exp const *r, Origin pos);

      virtual ExpRef expCreate_BitOrI(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_BitOrIEq(Exp const *l, Exp const *r, Origin pos);

      virtual ExpRef expCreate_BitOrX(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_BitOrXEq(Exp const *l, Exp const *r, Origin pos);

      virtual ExpRef expCreate_Call(Exp const *e,
         Core::Array<ExpRef> &&args, Scope &scope, Origin pos);

      virtual ExpRef expCreate_CmpEQ(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_CmpGE(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_CmpGT(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_CmpLE(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_CmpLT(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_CmpNE(Exp const *l, Exp const *r, Origin pos);

      virtual ExpRef expCreate_Cnd(Exp const *c, Exp const *l, Exp const *r, Origin pos);

      virtual ExpRef expCreate_Comma(Exp const *l, Exp const *r, Origin pos);

      ExpRef expCreate_Cst(Type const *t, Exp const *e);
      virtual ExpRef expCreate_Cst(Type const *t, Exp const *e, Origin pos);

      virtual ExpRef expCreate_DecPre(Exp const *e, Origin pos);
      virtual ExpRef expCreate_DecSuf(Exp const *e, Origin pos);

      virtual ExpRef expCreate_Deref(Exp const *e, Origin pos);

      virtual ExpRef expCreate_Div(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_DivEq(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_DivEx(Exp const *l, Exp const *r, Origin pos);

      virtual ExpRef expCreate_Func(IR::Program &prog, SR::Function *fn, Origin pos);

      virtual ExpRef expCreate_GenSel(Exp const *exp, Exp const *def,
         Core::Array<GenAssoc> &&a, Origin pos);

      ExpRef expCreate_IRExp(IR::Exp const *exp, Type const *type);
      virtual ExpRef expCreate_IRExp(IR::Exp const *exp, Type const *type, Origin pos);

      virtual ExpRef expCreate_IncPre(Exp const *e, Origin pos);
      virtual ExpRef expCreate_IncSuf(Exp const *e, Origin pos);

      virtual ExpRef expCreate_Inv(Exp const *e, Origin pos);

      virtual ExpRef expCreate_JmpLng(Scope &scope, Exp const *env,
         Exp const *val, Origin pos);
      virtual ExpRef expCreate_JmpSet(Exp const *env, Origin pos);

      virtual ExpRef expCreate_LitInt(Type const *type, Core::Integ const &lit, Origin pos);
      virtual ExpRef expCreate_LitInt(Type const *type, Core::Integ &&lit, Origin pos);

      virtual ExpRef expCreate_LitNul(Type const *type, Origin pos);

      virtual ExpRef expCreate_LogAnd(Exp const *l, Exp const *r, Origin pos);

      virtual ExpRef expCreate_LogOrI(Exp const *l, Exp const *r, Origin pos);

      virtual ExpRef expCreate_Mem(Exp const *l, Core::String r, Origin pos,
         Scope &scope);

      virtual ExpRef expCreate_MemPt(Exp const *l, Core::String r, Origin pos,
         Scope &scope);

      virtual ExpRef expCreate_Mod(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_ModEq(Exp const *l, Exp const *r, Origin pos);

      virtual ExpRef expCreate_Mul(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_MulEq(Exp const *l, Exp const *r, Origin pos);

      virtual ExpRef expCreate_Not(Exp const *e, Origin pos);

      virtual ExpRef expCreate_Obj(IR::Program &prog, SR::Object *obj, Origin pos);

      virtual ExpRef expCreate_Refer(Exp const *e, Origin pos);

      virtual ExpRef expCreate_ShL(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_ShLEq(Exp const *l, Exp const *r, Origin pos);

      virtual ExpRef expCreate_ShR(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_ShREq(Exp const *l, Exp const *r, Origin pos);

      virtual ExpRef expCreate_SizeAlign(Type const *type, Origin pos);

      virtual ExpRef expCreate_SizeBytes(Exp const *type, Origin pos);
      virtual ExpRef expCreate_SizeBytes(Type const *type, Origin pos);

      virtual ExpRef expCreate_StrIdx(IR::Program &prog, Scope &scope,
         Core::String lit, Origin pos);
      virtual ExpRef expCreate_StrU08(IR::Program &prog, Scope &scope,
         Core::String lit, Origin pos);
      virtual ExpRef expCreate_StrU16(IR::Program &prog, Scope &scope,
         Core::String lit, Origin pos);
      virtual ExpRef expCreate_StrU32(IR::Program &prog, Scope &scope,
         Core::String lit, Origin pos);
      virtual ExpRef expCreate_String(IR::Program &prog, Scope &scope,
         Core::String lit, Origin pos);

      virtual ExpRef expCreate_Sub(Exp const *e, Origin pos);

      virtual ExpRef expCreate_Sub(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_SubEq(Exp const *l, Exp const *r, Origin pos);
      virtual ExpRef expCreate_SubEq(Exp const *l, Exp const *r, Origin pos, bool post);

      virtual ExpRef expCreate_VaArg(Type const *type, Exp const *exp, Origin pos);

      // Default argument promotions.
      virtual ExpRef expPromo_Arg(Exp const *e, Origin pos);

      // Usual arithmtic conversions.
      std::tuple<TypeRef /*type*/, ExpRef /*expL*/, ExpRef /*expR*/>
      virtual expPromo_Arith(Exp const *l, Exp const *r, Origin pos);

      // Usual arithmetic conversions, always promoted to common type.
      std::tuple<TypeRef /*type*/, ExpRef /*expL*/, ExpRef /*expR*/>
      virtual expPromo_ArithAlways(Exp const *l, Exp const *r, Origin pos);

      // Conversion as if by assignment.
      ExpRef expPromo_Assign(Type const *t, Exp const *e);
      virtual ExpRef expPromo_Assign(Type const *t, Exp const *e, Origin pos);

      // Equality expression contraints and promotion.
      std::tuple<TypeRef /*type*/, ExpRef /*expL*/, ExpRef /*expR*/>
      virtual expPromo_CmpEqu(Exp const *l, Exp const *r, Origin pos);

      // Relational expression contraints and promotion.
      std::tuple<TypeRef /*type*/, ExpRef /*expL*/, ExpRef /*expR*/>
      virtual expPromo_CmpRel(Exp const *l, Exp const *r, Origin pos);

      // Promotes a branch condition to a soft boolean.
      virtual ExpRef expPromo_Cond(Exp const *e, Origin pos);

      // Conditional expression contraints and promotion. (Center and right.)
      std::tuple<TypeRef /*type*/, ExpRef /*expL*/, ExpRef /*expR*/>
      virtual expPromo_Cond(Exp const *l, Exp const *r, Origin pos);

      // Conditional expression contraints and promotion.
      std::tuple<TypeRef /*type*/, ExpRef /*expC*/, ExpRef /*expL*/, ExpRef /*expR*/>
      virtual expPromo_Cond(Exp const *c, Exp const *l, Exp const *r, Origin pos);

      // Integer promotion.
      virtual ExpRef expPromo_Int(Exp const *e, Origin pos);

      // Lvalues, arrays, and function designators.
      ExpRef expPromo_LValue(Exp const *e);
      virtual ExpRef expPromo_LValue(Exp const *e, Origin pos);

      // Equality expression contraints and promotion for pointers.
      std::tuple<TypeRef /*type*/, ExpRef /*expL*/, ExpRef /*expR*/>
      virtual expPromo_PtrEqu(Exp const *l, Exp const *r, Origin pos);

      // Relational expression contraints and promotion for pointers.
      std::tuple<TypeRef /*type*/, ExpRef /*expL*/, ExpRef /*expR*/>
      virtual expPromo_PtrRel(Exp const *l, Exp const *r, Origin pos);

      virtual StRef stCreate_Asm(Labels &&labels, Origin pos,
         Core::Array<Core::Token> &&tokens);

      virtual StRef stCreate_Break(Labels &&labels, Origin pos, Scope_Local &ctx);

      virtual StRef stCreate_Continue(Labels &&labels, Origin pos, Scope_Local &ctx);

      virtual StRef stCreate_Decl(Origin pos, Scope &ctx);

      virtual StRef stCreate_Do(Labels &&labels, Origin pos, Scope_Local &ctx,
         St const *body, Exp const *cond);

      virtual StRef stCreate_Empty(Labels &&labels, Origin pos);

      StRef stCreate_Exp(Labels &&labels, Exp const *exp);
      virtual StRef stCreate_Exp(Labels &&labels, Origin pos, Exp const *exp);

      virtual StRef stCreate_For(Labels &&labels, Origin pos, Scope_Local &ctx,
         St const *init, Exp const *cond, St const *iter, St const *body);

      virtual StRef stCreate_FuncPre(Origin pos, Scope_Function &scope);

      virtual StRef stCreate_FuncPro(Origin pos, Scope_Function &scope);

      virtual StRef stCreate_Goto(Labels &&labels, Origin pos, Exp const *exp);

      virtual StRef stCreate_Goto(Labels &&labels, Origin pos,
         Scope_Local &ctx, Core::String name);

      virtual StRef stCreate_If(Labels &&labels, Origin pos, Exp const *cond,
         St const *body);

      virtual StRef stCreate_If(Labels &&labels, Origin pos, Exp const *cond,
         St const *bodyT, St const *bodyF);

      virtual StRef stCreate_Multi(Labels &&labels, Origin pos,
         Core::Array<StRef> &&sts);

      virtual StRef stCreate_Return(Labels &&labels, Origin pos,
         Scope_Function &scope, Exp const *exp = nullptr);

      virtual StRef stCreate_Switch(Labels &&labels, Origin pos,
         Scope_Case &ctx, Exp const *cond, St const *body);

      virtual StRef stCreate_While(Labels &&labels, Origin pos,
         Scope_Local &ctx, Exp const *cond, St const *body);
   };
}

#endif//GDCC__CC__Factory_H__

