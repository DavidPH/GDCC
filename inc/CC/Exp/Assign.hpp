//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator =" expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__CC__Exp__Assign_H__
#define GDCC__CC__Exp__Assign_H__

#include "../../CC/Exp.hpp"

#include "../../SR/Exp/Binary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::CC
{
   //
   // Exp_Assign
   //
   class Exp_Assign : public SR::Exp_Binary
   {
      GDCC_Core_CounterPreamble(GDCC::CC::Exp_Assign, GDCC::SR::Exp_Binary);

   public:
      // Create
      static CRef Create(SR::Exp const *l, SR::Exp const *r, Core::Origin pos)
         {return CRef(new This(l, r, pos));}

   protected:
      Exp_Assign(SR::Exp const *l, SR::Exp const *r, Core::Origin pos);

      virtual void v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const;

      virtual IRExpCRef v_getIRExp() const;

      virtual bool v_isEffect() const;

      virtual bool v_isIRExp() const;
   };
}

#endif//GDCC__CC__Exp__Assign_H__

