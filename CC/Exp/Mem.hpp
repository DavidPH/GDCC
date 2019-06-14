//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator ." and "operator ->" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Mem_H__
#define GDCC__CC__Exp__Mem_H__

#include "../../CC/Exp.hpp"

#include "../../CC/Type/Struct.hpp"

#include "../../SR/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Exp_Mem
   //
   class Exp_Mem : public SR::Exp
   {
      GDCC_Core_CounterPreamble(GDCC::CC::Exp_Mem, GDCC::SR::Exp);

   public:
      SR::Exp::CRef const expL;
      Core::String  const expR;


      // Create
      static CRef Create(SR::Exp const *l, Core::String r, Core::Origin pos,
         Factory &fact)
         {return CRef(new This(l, r, pos, fact));}

   protected:
      Exp_Mem(SR::Exp const *l, Core::String r, Core::Origin pos_, Factory &fact_) :
         Super{pos_}, expL{l}, expR{r}, fact{fact_} {}

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

      virtual SR::Arg v_getArg() const;

      virtual TypeCRef v_getType() const;

      virtual bool v_isEffect() const;

      virtual bool v_isIRExp() const {return false;}

      virtual bool v_isNoAuto() const;

      virtual void v_setRefer() const;

      Factory &fact;
   };

   //
   // Exp_MemProp
   //
   // Special transitionary type for handling members that are properties. Has
   // no actual type or value until converted to another expression.
   //
   class Exp_MemProp : public Exp_Mem
   {
      GDCC_Core_CounterPreamble(GDCC::CC::Exp_MemProp, GDCC::CC::Exp_Mem);

   public:
      SR::Exp::CRef createExp() const;
      SR::Exp::CRef createExp_Add2() const
         {return createExpEq(&Type_Struct::Prop::propAdd2, "++");}
      SR::Exp::CRef createExp_Add3() const
         {return createExpEq(&Type_Struct::Prop::propAdd3, "+++");}
      SR::Exp::CRef createExp_AddEq(SR::Exp::CRef const &arg) const
         {return createExpEq(arg, &Type_Struct::Prop::propAddEq, "+=");}
      SR::Exp::CRef createExp_AndEq(SR::Exp::CRef const &arg) const
         {return createExpEq(arg, &Type_Struct::Prop::propAndEq, "&=");}
      SR::Exp::CRef createExp_DivEq(SR::Exp::CRef const &arg) const
         {return createExpEq(arg, &Type_Struct::Prop::propDivEq, "/=");}
      SR::Exp::CRef createExp_Equal(SR::Exp::CRef const &arg) const
         {return createExpEq(arg, &Type_Struct::Prop::propEqual, "=");}
      SR::Exp::CRef createExp_ModEq(SR::Exp::CRef const &arg) const
         {return createExpEq(arg, &Type_Struct::Prop::propModEq, "%=");}
      SR::Exp::CRef createExp_MulEq(SR::Exp::CRef const &arg) const
         {return createExpEq(arg, &Type_Struct::Prop::propMulEq, "*=");}
      SR::Exp::CRef createExp_OrIEq(SR::Exp::CRef const &arg) const
         {return createExpEq(arg, &Type_Struct::Prop::propOrIEq, "|=");}
      SR::Exp::CRef createExp_OrXEq(SR::Exp::CRef const &arg) const
         {return createExpEq(arg, &Type_Struct::Prop::propOrXEq, "^=");}
      SR::Exp::CRef createExp_Paren(Core::Array<SR::Exp::CRef> &&args) const;
      SR::Exp::CRef createExp_ShLEq(SR::Exp::CRef const &arg) const
         {return createExpEq(arg, &Type_Struct::Prop::propShLEq, "<<=");}
      SR::Exp::CRef createExp_ShREq(SR::Exp::CRef const &arg) const
         {return createExpEq(arg, &Type_Struct::Prop::propShREq, ">>=");}
      SR::Exp::CRef createExp_Sub2() const
         {return createExpEq(&Type_Struct::Prop::propSubEq, "--");}
      SR::Exp::CRef createExp_Sub3() const
         {return createExpEq(&Type_Struct::Prop::propSubEq, "---");}
      SR::Exp::CRef createExp_SubEq(SR::Exp::CRef const &arg) const
         {return createExpEq(arg, &Type_Struct::Prop::propSubEq, "-=");}

      Type_Struct::CRef type;
      Scope            &scope;


      // Create
      static CRef Create(SR::Exp const *l, Core::String r, Core::Origin pos,
         Factory &fact, Type_Struct const *t, Scope &scope)
         {return CRef(new This(l, r, pos, fact, t, scope));}

   protected:
      Exp_MemProp(SR::Exp const *l, Core::String r, Core::Origin pos_,
         Factory &fact_, Type_Struct const *t, Scope &scope_) :
         Super{l, r, pos_, fact_}, type{t}, scope{scope_} {}

      SR::Exp::CRef createExp(Type_Struct::Prop const &prop, StructProp const *func,
         SR::Exp::CRef const *argv, std::size_t argc) const;

      SR::Exp::CRef createExpEq(SR::Exp::CRef const &arg,
         StructProp const *Type_Struct::Prop::*mem, char const *op) const;

      SR::Exp::CRef createExpEq(StructProp const *Type_Struct::Prop::*mem,
         char const *op) const;

      virtual SR::Arg v_getArg() const;

      virtual TypeCRef v_getType() const;
   };
}

#endif//GDCC__CC__Exp__Mem_H__

