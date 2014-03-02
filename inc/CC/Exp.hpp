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

#ifndef GDCC__CC__Exp_H__
#define GDCC__CC__Exp_H__

#include "../Core/Counter.hpp"
#include "../Core/Number.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      class Exp;
      class Function;
      class Object;
      class Type;
   }

   namespace Core
   {
      template<typename T> class Array;
      struct Origin;
      class String;
   }

   namespace IR
   {
      class Program;
   }

   namespace CC
   {
      //
      // GenAssoc
      //
      struct GenAssoc
      {
         GenAssoc(AST::Type const *type, AST::Exp const *exp);

         Core::CounterRef<AST::Type const> type;
         Core::CounterRef<AST::Exp const>  exp;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      Core::CounterRef<AST::Exp const> ExpConvert_Arith(AST::Type const *t,
         AST::Exp const *e, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpConvert_Bool(AST::Type const *t,
         AST::Exp const *e, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpConvert_Pointer(AST::Type const *t,
         AST::Exp const *e, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Add(AST::Exp const *e,
         Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Add(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_AddEq(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Array(AST::Exp const *exp,
         AST::Exp const *idx, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Assign(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_BitAnd(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_BitAndEq(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_BitOrI(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_BitOrIEq(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_BitOrX(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_BitOrXEq(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Call(AST::Exp const *e,
         Core::Array<Core::CounterRef<AST::Exp const>> const &args,
         Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_Call(AST::Exp const *e,
         Core::Array<Core::CounterRef<AST::Exp const>>      &&args,
         Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_CmpEQ(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_CmpGE(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_CmpGT(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_CmpLE(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_CmpLT(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_CmpNE(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Cnd(AST::Exp const *c,
         AST::Exp const *l, AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Comma(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Cst(AST::Type const *t,
         AST::Exp const *e, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_DecPre(AST::Exp const *e,
         Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_DecSuf(AST::Exp const *e,
         Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Deref(AST::Exp const *e,
         Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Div(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_DivEq(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Func(IR::Program &prog,
         AST::Function *fn, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_GenSel(AST::Exp const *exp,
         AST::Exp const *def, Core::Array<GenAssoc> const &a, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_GenSel(AST::Exp const *exp,
         AST::Exp const *def, Core::Array<GenAssoc>      &&a, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_IncPre(AST::Exp const *e,
         Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_IncSuf(AST::Exp const *e,
         Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Inv(AST::Exp const *e,
         Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_LitInt(AST::Type const *type,
         Core::Integ const &lit, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_LitInt(AST::Type const *type,
         Core::Integ      &&lit, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_LogAnd(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_LogOrI(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Mem(AST::Exp const *l,
         Core::String r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_MemPt(AST::Exp const *l,
         Core::String r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Mod(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_ModEq(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Mul(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_MulEq(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Not(AST::Exp const *e,
         Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Obj(IR::Program &prog,
         AST::Object *obj, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Refer(AST::Exp const *e,
         Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_ShL(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_ShLEq(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_ShR(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_ShREq(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_SizeAlign(
         AST::Type const *type, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_SizeBytes(
         AST::Exp const *type, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_SizeBytes(
         AST::Type const *type, Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Sub(AST::Exp const *e,
         Core::Origin pos);

      Core::CounterRef<AST::Exp const> ExpCreate_Sub(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);
      Core::CounterRef<AST::Exp const> ExpCreate_SubEq(AST::Exp const *l,
         AST::Exp const *r, Core::Origin pos);

      // Default argument promotions.
      Core::CounterRef<AST::Exp const> ExpPromo_Arg(AST::Exp const *e,
         Core::Origin pos);

      // Usual arithmtic conversions.
      std::pair<Core::CounterRef<AST::Exp const>, Core::CounterRef<AST::Exp const>>
      ExpPromo_Arith(AST::Exp const *l, AST::Exp const *r, Core::Origin pos);

      // Conversion as if by assignment.
      Core::CounterRef<AST::Exp const> ExpPromo_Assign(AST::Type const *t,
         AST::Exp const *e, Core::Origin pos);

      // Integer promotion.
      Core::CounterRef<AST::Exp const> ExpPromo_Int(AST::Exp const *e,
         Core::Origin pos);

      // Lvalues, arrays, and function designators.
      Core::CounterRef<AST::Exp const> ExpPromo_LValue(AST::Exp const *e,
         Core::Origin pos);

      Core::FastU ExpToFastU(AST::Exp const *exp);
      Core::Integ ExpToInteg(AST::Exp const *exp);

      // Is exp an lvalue?
      bool IsLValue(AST::Exp const *exp);

      // Is exp a modifiable lvalue?
      bool IsModLValue(AST::Exp const *exp);
   }
}

#endif//GDCC__CC__Exp_H__

