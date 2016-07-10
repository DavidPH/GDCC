//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C unary "operator &" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Refer_H__
#define GDCC__CC__Exp__Refer_H__

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
      // Exp_Refer
      //
      class Exp_Refer : public SR::Exp_Unary
      {
         GDCC_Core_CounterPreamble(GDCC::CC::Exp_Refer, GDCC::SR::Exp_Unary);

      public:
         // Create
         static CRef Create(SR::Exp const *e, Core::Origin pos)
            {return CRef(new This(e, pos));}

      protected:
         Exp_Refer(SR::Exp const *e, Core::Origin pos);

         virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

         virtual FunctionRef v_getFunction() const;

         virtual IRExpCRef v_getIRExp() const;

         virtual bool v_isEffect() const;

         virtual bool v_isFunction() const;

         virtual bool v_isIRExp() const;
      };
   }
}

#endif//GDCC__CC__Exp__Refer_H__

