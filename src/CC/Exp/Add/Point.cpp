//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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
      // Exp_AddPoint::v_genStmnt
      //
      void Exp_AddPoint::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         AST::GenStmnt_Point(this, IR::Code::AddU_W, ctx, dst);
      }

      //
      // Exp_AddPointEq::v_genStmnt
      //
      void Exp_AddPointEq::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         AST::GenStmnt_PointEq(this, IR::Code::AddU_W, ctx, dst, post);
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
      void Exp_AddPtrRaw::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         if(GenStmntNul(this, ctx, dst)) return;

         // Evaluate pointer to stack.
         expL->genStmntStk(ctx);

         // Evaluate offset.
         expR->genStmntStk(ctx);

         // Add on stack.
         ctx.block.addStatementArgs(IR::Code::AddU_W,
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         GenStmnt_MovePart(this, ctx, dst, false, true);
      }

      //
      // Exp_AddPtrRaw::v_getIRExp
      //
      IR::Exp::CRef Exp_AddPtrRaw::v_getIRExp() const
      {
         return IR::ExpCreate_AddPtrRaw(expL->getIRExp(), expR->getIRExp(), pos);
      }
   }
}

// EOF

