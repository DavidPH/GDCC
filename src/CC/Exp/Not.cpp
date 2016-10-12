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

#include "CC/Exp/Not.hpp"

#include "CC/Type.hpp"

#include "IR/Block.hpp"

#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Not::v_genStmnt
      //
      void Exp_Not::v_genStmnt(SR::GenStmntCtx const &ctx,
         SR::Arg const &dst) const
      {
         if(GenStmntNul(this, ctx, dst)) return;

         // Evaluate sub-expression to stack.
         exp->genStmntStk(ctx);

         // Operate on stack.
         ctx.block.addStatementArgs({IR::Code::LNot, 1},
            IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         GenStmnt_MovePart(this, ctx, dst, false, true);
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
      // ExpCreate_Not
      //
      SR::Exp::CRef ExpCreate_Not(SR::Exp const *e, Core::Origin pos)
      {
         return Exp_Not::Create(TypeIntegPrS, ExpPromo_Cond(e, pos), pos);
      }
   }
}

// EOF

