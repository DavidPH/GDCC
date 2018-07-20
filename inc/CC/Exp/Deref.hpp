//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C unary "operator *" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Deref_H__
#define GDCC__CC__Exp__Deref_H__

#include "../../CC/Exp.hpp"

#include "../../SR/Exp/Unary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Exp_Deref
   //
   class Exp_Deref : public SR::Exp_Unary
   {
      GDCC_Core_CounterPreamble(GDCC::CC::Exp_Deref, GDCC::SR::Exp_Unary);

      GDCC_SR_Exp_UnaryCreator(Exp_Deref);

   protected:
      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

      virtual SR::Arg v_getArg() const;

      virtual bool v_isEffect() const;

      virtual bool v_isIRExp() const;

      virtual bool v_isNoAuto() const;
   };
}

#endif//GDCC__CC__Exp__Deref_H__

