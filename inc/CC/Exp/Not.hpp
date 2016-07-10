//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator !" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Not_H__
#define GDCC__CC__Exp__Not_H__

#include "../../CC/Exp.hpp"

#include "../../SR/Exp/Unary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Not
      //
      class Exp_Not : public SR::Exp_Unary
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_Not, GDCC::SR::Exp_Unary);

         GDCC_SR_Exp_UnaryCreator(Exp_Not);

      protected:
         virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;

         virtual bool v_isBoolean() const;
      };
   }
}

#endif//GDCC__CC__Exp__Not_H__

