//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation Size-type multiply expression.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Exp__MulSize_H__
#define GDCC__SR__Exp__MulSize_H__

#include "../../SR/Exp/Binary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::SR
{
   //
   // Exp_MulSize
   //
   class Exp_MulSize final : public Exp_Binary
   {
      GDCC_Core_CounterPreamble(
         GDCC::SR::Exp_MulSize, GDCC::SR::Exp_Binary);

   public:
      friend Exp::CRef ExpCreate_MulSize(Exp const *l, Exp const *r);

   protected:
      Exp_MulSize(Exp const *l, Exp const *r, Core::Origin pos);

      virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

      virtual IRExpCRef v_getIRExp() const;

      virtual TypeCRef v_getType() const;
   };
}

#endif//GDCC__SR__Exp__MulSize_H__

