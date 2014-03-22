//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator -" pointer expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Sub.hpp"

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
      // Exp_SubPtrInt::v_genStmnt
      //
      void Exp_SubPtrInt::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         if(GenStmntNul(this, ctx, dst)) return;

         // Evaluate pointer to stack.
         expL->genStmntStk(ctx);

         // Evaluate index, adjusting if necessary.
         auto point = type->getBaseType()->getSizePoint();
         if(point > 1)
         {
            auto lit = ExpCreate_LitInt(expR->getType(), point, pos);
            ExpCreate_Mul(expR, lit, pos)->genStmntStk(ctx);
         }
         else
            expR->genStmntStk(ctx);

         // Subtract on stack.
         ctx.block.addStatementArgs(IR::Code::SubU_W,
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         GenStmnt_MovePart(this, ctx, dst, false, true);
      }

      //
      // Exp_SubPtrPtrW::v_genStmnt
      //
      void Exp_SubPtrPtrW::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         if(GenStmntNul(this, ctx, dst)) return;

         // Evaluate both sub-expressions to stack.
         expL->genStmntStk(ctx);
         expR->genStmntStk(ctx);

         // Subtract on stack.
         ctx.block.addStatementArgs(IR::Code::SubI_W,
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Adjust result, if needed.
         auto point = expL->getType()->getBaseType()->getSizePoint();
         if(point > 1)
         {
            // TODO: Use a shift where possible. That is, where it is either
            // known that the result is positive or if the target has
            // arithmetic shift. Also that the rounding behavior of shifting
            // negative integers is acceptable. (That is, it will only break
            // if the pointers are already not properly aligned.)

            auto lit = ExpCreate_LitInt(type, point, pos)->getIRExp();
            ctx.block.addStatementArgs(IR::Code::DivI_W,
               IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Lit(lit));
         }

         // Move to destination.
         GenStmnt_MovePart(this, ctx, dst, false, true);
      }
   }
}

// EOF

