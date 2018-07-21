//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
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
      static CRef Create(SR::Exp const *l, Core::String r, Core::Origin pos)
         {return CRef(new This(l, r, pos));}

   protected:
      Exp_Mem(SR::Exp const *l, Core::String r, Core::Origin pos_) :
         Super{pos_}, expL{l}, expR{r} {}

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

      virtual SR::Arg v_getArg() const;

      virtual TypeCRef v_getType() const;

      virtual bool v_isEffect() const;

      virtual bool v_isIRExp() const {return false;}

      virtual bool v_isNoAuto() const;

      virtual void v_setRefer() const;
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
      SR::Exp::CRef createExp_call(Core::Array<SR::Exp::CRef> &&args) const;
      SR::Exp::CRef createExp_get() const;
      SR::Exp::CRef createExp_set(SR::Exp::CRef const &arg) const;

      Type_Struct::CRef type;
      Scope            &scope;


      // Create
      static CRef Create(SR::Exp const *l, Core::String r, Core::Origin pos,
         Type_Struct const *t, Scope &scope)
         {return CRef(new This(l, r, pos, t, scope));}

   protected:
      Exp_MemProp(SR::Exp const *l, Core::String r, Core::Origin pos_,
         Type_Struct const *t, Scope &scope_) :
         Super{l, r, pos_}, type{t}, scope{scope_} {}

      SR::Exp::CRef createExp(Type_Struct::Prop const &prop, StructProp const *func,
         SR::Exp::CRef const *argv, std::size_t argc) const;

      virtual SR::Arg v_getArg() const;

      virtual TypeCRef v_getType() const;
   };
}

#endif//GDCC__CC__Exp__Mem_H__

