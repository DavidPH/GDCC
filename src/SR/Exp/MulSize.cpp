//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation Size-type multiply expression.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/MulSize.hpp"

#include "SR/Exp/Arith.hpp"
#include "SR/Type.hpp"

#include "IR/Code.hpp"
#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // Exp_MulSize constructor
   //
   Exp_MulSize::Exp_MulSize(Exp const *l, Exp const *r,
      Core::Origin pos_) : Super{Type::Size, l, r, pos_}
   {
   }

   //
   // Exp_MulSize::v_genStmnt
   //
   void Exp_MulSize::v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
   {
      GenStmnt_Arith(this, IR::Code::MulU, ctx, dst);
   }

   //
   // Exp_MulSize::v_getIRExp
   //
   IR::Exp::CRef Exp_MulSize::v_getIRExp() const
   {
      return IR::ExpCreate_Mul(expL->getIRExp(), expR->getIRExp(), pos);
   }

   //
   // Exp_MulSize::v_getType
   //
   Type::CRef Exp_MulSize::v_getType() const
   {
      return Type::Size;
   }
}

// EOF

