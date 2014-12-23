//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

#include "AST/Type.hpp"

#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Not::v_genStmnt
      //
      void Exp_Not::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         if(GenStmntNul(this, ctx, dst)) return;

         // Evaluate sub-expression to stack.
         exp->genStmntStk(ctx);

         // Operate on stack.
         ctx.block.addStatementArgs({IR::Code::NotU_W, 1},
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
      // ExpCreate_Not
      //
      AST::Exp::CRef ExpCreate_Not(AST::Exp const *e, Core::Origin pos)
      {
         return Exp_Not::Create(TypeIntegPrS, ExpPromo_Cond(e, pos), pos);
      }
   }
}

// EOF

