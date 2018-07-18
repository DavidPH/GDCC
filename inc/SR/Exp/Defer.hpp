//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Deferred expression base.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Exp__Defer_H__
#define GDCC__SR__Exp__Defer_H__

#include "../../SR/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::SR
{
   //
   // Exp_Defer
   //
   class Exp_Defer : public Exp
   {
      GDCC_Core_CounterPreambleAbstract(
         GDCC::SR::Exp_Defer, GDCC::SR::Exp);

   public:
      Exp::CRef getDefer() const;

   protected:
      Exp_Defer(Core::Origin pos_) : Super{pos_} {}

      virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

      virtual Arg v_getArg() const;

      virtual Exp::CRef v_getDefer() const = 0;

      virtual FunctionRef v_getFunction() const;

      virtual IRExpCRef v_getIRExp() const;

      virtual ObjectRef v_getObject() const;

      virtual TypeCRef v_getType() const;

      virtual bool v_isEffect() const;

      virtual bool v_isFunction() const;

      virtual bool v_isIRExp() const;

      virtual bool v_isNoAuto() const;

      virtual bool v_isObject() const;

   private:
      mutable Exp::CPtr cacheDefer;
   };
}

#endif//GDCC__SR__Exp__Defer_H__

