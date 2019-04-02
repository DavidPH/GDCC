//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C __va_arg expression.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__VaArg__H__
#define GDCC__CC__Exp__VaArg__H__

#include "../../CC/Exp.hpp"

#include "../../SR/Exp/Unary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Exp_VaArg
   //
   class Exp_VaArg : public SR::Exp_Unary
   {
      GDCC_Core_CounterPreamble(GDCC::CC::Exp_VaArg, GDCC::SR::Exp_Unary);

   public:
      static SR::Exp::CRef Create(SR::Type const *t, SR::Exp const *e,
         Core::Origin pos, Factory &fact)
      {
         return CRef(new This(t, e, pos, fact));
      }

   protected:
      Exp_VaArg(SR::Type const *t, SR::Exp const *e, Core::Origin pos_,
         Factory &fact_) : Super{t, e, pos_}, fact{fact_} {}

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

      virtual SR::Arg v_getArg() const;

      virtual bool v_isIRExp() const;

      Factory &fact;
   };
}

#endif//GDCC__CC__Exp__VaArg__H__

