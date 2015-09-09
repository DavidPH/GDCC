//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
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

#include "AST/Temporary.hpp"
#include "AST/Type.hpp"

#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
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
         AST::GenStmnt_Point(this, {IR::Code::SubU_W, 1}, ctx, dst);
      }

      //
      // Exp_SubPtrIntEq::v_genStmnt
      //
      void Exp_SubPtrIntEq::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         AST::GenStmnt_PointEq(this, {IR::Code::SubU_W, 1}, ctx, dst, post);
      }

      //
      // Exp_SubPtrIntEq::v_getIRExp
      //
      IR::Exp::CRef Exp_SubPtrIntEq::v_getIRExp() const
      {
         return post ? expL->getIRExp() : Super::v_getIRExp();
      }

      //
      // Exp_SubPtrIntEq::v_isIRExp
      //
      bool Exp_SubPtrIntEq::v_isIRExp() const
      {
         return post ? expL->isIRExp() : Super::v_isIRExp();
      }

      //
      // Exp_SubPtrPtrW::v_genStmnt
      //
      void Exp_SubPtrPtrW::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         if(GenStmntNul(this, ctx, dst)) return;

         Core::FastU pointWords = expL->getType()->getSizeWords();

         // Evaluate both sub-expressions to stack.
         expL->genStmntStk(ctx);
         expR->genStmntStk(ctx);

         if(pointWords > 1)
         {
            AST::Temporary tmp{ctx, pos, 1};

            ctx.block.addStatementArgs({IR::Code::Move_W, 1},
               tmp.getArg(), IR::Arg_Stk());
            ctx.block.addStatementArgs({IR::Code::Move_W, pointWords - 1},
               IR::Arg_Nul(), IR::Arg_Stk());

            ctx.block.addStatementArgs({IR::Code::SubI_W, 1},
               tmp.getArg(), IR::Arg_Stk(), tmp.getArg());
            ctx.block.addStatementArgs({IR::Code::Move_W, pointWords - 1},
               IR::Arg_Nul(), IR::Arg_Stk());

            ctx.block.addStatementArgs({IR::Code::Move_W, 1},
               IR::Arg_Stk(), tmp.getArg());
         }
         else
         {
            // Subtract on stack.
            ctx.block.addStatementArgs({IR::Code::SubI_W, 1},
               IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());
         }

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
            ctx.block.addStatementArgs({IR::Code::DivI_W, 1},
               IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Lit(lit));
         }

         // Move to destination.
         GenStmnt_MovePart(this, ctx, dst, false, true);
      }
   }
}

// EOF

