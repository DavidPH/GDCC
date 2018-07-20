//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
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

      GDCC_SR_Exp_UnaryCreator(Exp_VaArg);

   protected:
      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

      virtual SR::Arg v_getArg() const;

      virtual bool v_isIRExp() const;
   };
}

#endif//GDCC__CC__Exp__VaArg__H__

