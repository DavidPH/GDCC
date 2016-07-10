//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C setjmp/longjmp expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Jmp_H__
#define GDCC__CC__Exp__Jmp_H__

#include "../../CC/Exp.hpp"

#include "../../SR/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      class Scope_Local;

      //
      // Exp_JmpLng
      //
      class Exp_JmpLng : public SR::Exp
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_JmpLng, GDCC::SR::Exp);

      public:
         static CRef Create(Scope_Local &scope, SR::Exp const *env,
            SR::Exp const *val, Core::Origin pos)
            {return CRef(new This(scope, env, val, pos));}

      protected:
         Exp_JmpLng(Scope_Local &scope_, SR::Exp const *env_,
            SR::Exp const *val_, Core::Origin pos_) :
            Super{pos_}, scope(scope_), env{env_}, val{val_} {}

         virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const {return true;}

         virtual bool v_isIRExp() const {return false;}

         virtual bool v_isNoAuto() const;

         Scope_Local  &scope;
         SR::Exp::CRef env;
         SR::Exp::CRef val;
      };

      //
      // Exp_JmpSet
      //
      class Exp_JmpSet : public SR::Exp
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_JmpSet, GDCC::SR::Exp);

      public:
         static CRef Create(SR::Exp const *env, Core::Origin pos)
            {return CRef(new This(env, pos));}

      protected:
         Exp_JmpSet(SR::Exp const *env_, Core::Origin pos_) : Super{pos_}, env{env_} {}

         virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

         virtual TypeCRef v_getType() const;

         virtual bool v_isEffect() const {return true;}

         virtual bool v_isIRExp() const {return false;}

         virtual bool v_isNoAuto() const;

         SR::Exp::CRef env;
      };
   }
}

#endif//GDCC__CC__Exp__Jmp_H__

