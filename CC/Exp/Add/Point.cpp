//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator +" pointer expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Add.hpp"

#include "CC/Type.hpp"

#include "IR/Block.hpp"

#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_AddPoint::v_genStmnt
   //
   void Exp_AddPoint::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      SR::GenStmnt_Point(this, IR::CodeBase::Add+'U', ctx, dst);
   }

   //
   // Exp_AddPointEq::v_genStmnt
   //
   void Exp_AddPointEq::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      SR::GenStmnt_PointEq(this, IR::CodeBase::Add+'U', ctx, dst, post);
   }

   //
   // Exp_AddPointEq::v_getIRExp
   //
   IR::Exp::CRef Exp_AddPointEq::v_getIRExp() const
   {
      return post ? expL->getIRExp() : Super::v_getIRExp();
   }

   //
   // Exp_AddPointEq::v_isIRExp
   //
   bool Exp_AddPointEq::v_isIRExp() const
   {
      return post ? expL->isIRExp() : Super::v_isIRExp();
   }

   //
   // Exp_AddPtrRaw::v_genStmnt
   //
   void Exp_AddPtrRaw::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      if(GenStmntNul(this, ctx, dst)) return;

      GenStmnt_MoveDstPre(this, ctx, dst);

      // Evaluate pointer to stack.
      expL->genStmntStk(ctx);

      // Evaluate offset.
      expR->genStmntStk(ctx);

      // Add on stack.
      ctx.block.setArgSize().addStmnt(IR::CodeBase::Add+'U',
         IR::Block::Stk(), IR::Block::Stk(), IR::Block::Stk());

      // Move to destination.
      GenStmnt_MoveDstSuf(this, ctx, dst);
   }

   //
   // Exp_AddPtrRaw::v_getIRExp
   //
   IR::Exp::CRef Exp_AddPtrRaw::v_getIRExp() const
   {
      return IR::ExpCreate_AddPtrRaw(expL->getIRExp(), expR->getIRExp(), pos);
   }

   //
   // Exp_AddStrEntInt::v_genStmnt
   //
   void Exp_AddStrEntInt::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      if(GenStmntNul(this, ctx, dst)) return;

      GenStmnt_MoveDstPre(this, ctx, dst);

      // Evaluate operands.
      expL->genStmntStk(ctx);
      expR->genStmntStk(ctx);

      // Move to destination.
      GenStmnt_MoveDstSuf(this, ctx, dst);
   }
}

// EOF

