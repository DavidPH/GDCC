//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Deferred expression base.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Exp__Defer_H__
#define GDCC__AST__Exp__Defer_H__

#include "../../AST/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_Defer
      //
      class Exp_Defer : public AST::Exp
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::AST::Exp_Defer, GDCC::AST::Exp);

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

         virtual bool v_isObject() const;

      private:
         mutable Exp::CPtr cacheDefer;
      };
   }
}

#endif//GDCC__AST__Exp__Defer_H__

