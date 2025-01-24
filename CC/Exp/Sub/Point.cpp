//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
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

#include "IR/Block.hpp"

#include "SR/Temporary.hpp"
#include "SR/Type.hpp"

#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_SubPtrInt::v_genStmnt
   //
   void Exp_SubPtrInt::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      SR::GenStmnt_Point(this, IR::CodeBase::Sub+'U', ctx, dst);
   }

   //
   // Exp_SubPtrIntEq::v_genStmnt
   //
   void Exp_SubPtrIntEq::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      SR::GenStmnt_PointEq(this, IR::CodeBase::Sub+'U', ctx, dst, post);
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
   void Exp_SubPtrPtrW::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      if(GenStmntNul(this, ctx, dst)) return;

      Core::FastU pointWords = expL->getType()->getSizeWords();

      GenStmnt_MoveDstPre(this, ctx, dst);

      // Evaluate both sub-expressions to stack.
      expL->genStmntStk(ctx);
      expR->genStmntStk(ctx);

      if(pointWords > 1)
      {
         auto diffBytes = (pointWords - 1) * Target::GetWordBytes();

         SR::Temporary tmp{ctx, pos, 1};

         ctx.block.addStmnt(IR::CodeBase::Move,
            tmp. getArg(), tmp.getArgStk());
         ctx.block.addStmnt(IR::CodeBase::Move,
            IR::Arg_Nul(diffBytes), IR::Arg_Stk(diffBytes));

         ctx.block.addStmnt(IR::CodeBase::Sub+'I',
            tmp.getArg(), tmp.getArgStk(), tmp.getArg());
         ctx.block.addStmnt(IR::CodeBase::Move,
            IR::Arg_Nul(diffBytes), IR::Arg_Stk(diffBytes));

         ctx.block.addStmnt(IR::CodeBase::Move,
            tmp.getArgStk(), tmp.getArg());
      }
      else
      {
         // Subtract on stack.
         ctx.block.setArgSize().addStmnt(IR::CodeBase::Sub+'I',
            IR::Block::Stk(), IR::Block::Stk(), IR::Block::Stk());
      }

      // Adjust result, if needed.
      auto point = expL->getType()->getBaseType()->getSizePoint();
      if(point > 1)
      {
         // TODO: Use a shift where possible. That is, where it is either
         // known that the result is positive or if the target has
         // logical shift. Also that the rounding behavior of shifting
         // negative integers is acceptable. (That is, it will only break
         // if the pointers are already not properly aligned.)

         ctx.block.setArgSize().addStmnt(IR::CodeBase::Div+'I',
            IR::Block::Stk(), IR::Block::Stk(), point);
      }

      // Move to destination.
      GenStmnt_MoveDstSuf(this, ctx, dst);
   }
}

// EOF

