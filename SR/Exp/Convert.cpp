//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation conversion expression bases.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/Convert.hpp"

#include "SR/Arg.hpp"
#include "SR/Type.hpp"

#include "IR/Block.hpp"
#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // Exp_Convert::v_getIRExp
   //
   IR::Exp::CRef Exp_Convert::v_getIRExp() const
   {
      // The conversion logic is handled in Exp::getIRExp.
      return exp->getIRExp();
   }

   //
   // Exp_ConvertArith::v_genStmnt
   //
   void Exp_ConvertArith::v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
   {
      if(GenStmntNul(this, ctx, dst)) return;

      // Evaluate expression to stack.
      exp->genStmntStk(ctx);

      // Convert on stack.
      GenStmnt_ConvertArith(this, type, exp->getType(), ctx);

      // Move to destination.
      GenStmnt_MovePart(exp, ctx, dst, false, true);
   }

   //
   // Exp_ConvertBitfield::v_genStmnt
   //
   void Exp_ConvertBitfield::v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
   {
      if(GenStmntNul(this, ctx, dst)) return;

      // Evaluate expression to stack.
      exp->genStmntStk(ctx);

      // Convert on stack.
      auto bitT = exp->getType();
      auto sign = bitT->getSizeBitsS();
      auto bits = bitT->getSizeBitsF() + bitT->getSizeBitsI() + sign;
      auto offs = bitT->getSizeBitsO();
      auto code = sign ? IR::CodeBase::Bges : IR::CodeBase::Bget;
      ctx.block.setArgSize()
         .addStmnt(code, dst.getIRArgStk(), exp->getIRArgStk(), bits, offs);

      // Move to destination.
      GenStmnt_MovePart(exp, ctx, dst, false, true);
   }
}

// EOF

