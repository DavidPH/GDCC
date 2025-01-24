//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator !" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Not.hpp"

#include "CC/Factory.hpp"
#include "CC/Type.hpp"

#include "IR/Block.hpp"

#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_Not::v_genStmnt
   //
   void Exp_Not::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      if(GenStmntNul(this, ctx, dst)) return;

      GenStmnt_MoveDstPre(this, ctx, dst);

      // Evaluate sub-expression to stack.
      exp->genStmntStk(ctx);

      // Operate on stack.
      ctx.block.setArgSize().addStmnt(IR::CodeBase::LNot,
         IR::Block::Stk(), IR::Block::Stk());

      // Move to destination.
      GenStmnt_MoveDstSuf(this, ctx, dst);
   }

   //
   // Exp_Not::v_getIRExp
   //
   IR::Exp::CRef Exp_Not::v_getIRExp() const
   {
      return IR::ExpCreate_Not(exp->getIRExp(), pos);
   }

   //
   // Exp_Not::v_isBoolean
   //
   bool Exp_Not::v_isBoolean() const
   {
      return true;
   }

   //
   // Factory::expCreate_Not
   //
   SR::Exp::CRef Factory::expCreate_Not(SR::Exp const *e, Core::Origin pos)
   {
      return Exp_Not::Create(TypeIntegPrS, expPromo_Cond(e, pos), pos);
   }
}

// EOF

