//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C conversion to void expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Convert__Void_H__
#define GDCC__CC__Exp__Convert__Void_H__

#include "../../../CC/Exp.hpp"

#include "../../../SR/Exp/Convert.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Exp_ConvertVoid
   //
   class Exp_ConvertVoid : public SR::Exp_Convert
   {
      GDCC_Core_CounterPreamble(
         GDCC::CC::Exp_ConvertVoid, GDCC::SR::Exp_Convert);

      GDCC_SR_Exp_UnaryCreator(Exp_ConvertVoid);

   protected:
      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;
   };
}

#endif//GDCC__CC__Exp__Convert__Void_H__

