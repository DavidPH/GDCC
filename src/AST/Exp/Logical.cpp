//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree logical expression bases.
//
//-----------------------------------------------------------------------------

#include "AST/Exp/Logical.hpp"

#include "AST/Function.hpp"

#include "IR/Block.hpp"
#include "IR/Exp.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// GenStmnt_Logical
//
static void GenStmnt_Logical(GDCC::AST::Exp_Binary const *exp,
   GDCC::AST::GenStmntCtx const &ctx, GDCC::AST::Arg const &dst,
   GDCC::IR::Code codeShort, int valueShort, int valueLong)
{
   using namespace GDCC;

   // TODO: If expR is trivial, possibly forego short-circuiting.

   IR::Glyph labelEnd   = {ctx.prog, ctx.fn->genLabel()};
   IR::Glyph labelShort = {ctx.prog, ctx.fn->genLabel()};

   // Evaluate left operand to stack.
   exp->expL->genStmntStk(ctx);

   // Possibly jump to short result.
   ctx.block.addStatementArgs(codeShort, IR::Arg_Stk(), labelShort);

   // Evaluate right operand to stack.
   exp->expR->genStmntStk(ctx);

   // TODO: If expR is already boolean, use its result directly.

   // Possibly jump to short result.
   ctx.block.addStatementArgs(codeShort, IR::Arg_Stk(), labelShort);

   // Long result.
   ctx.block.addStatementArgs(IR::Code::Move_W, IR::Arg_Stk(), valueLong);
   ctx.block.addStatementArgs(IR::Code::Jump, labelEnd);

   // Short result.
   ctx.block.addLabel(labelShort);
   ctx.block.addStatementArgs(IR::Code::Move_W, IR::Arg_Stk(), valueShort);

   // End.
   ctx.block.addLabel(labelEnd);

   // Move to destination.
   GenStmnt_MovePart(exp, ctx, dst, false, true);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_LogAnd::v_genStmnt
      //
      void Exp_LogAnd::v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
      {
         GenStmnt_Logical(this, ctx, dst, IR::Code::Jcnd_Nil, 0, 1);
      }

      //
      // Exp_LogAnd::v_getIRExp
      //
      IR::Exp::CRef Exp_LogAnd::v_getIRExp() const
      {
         return IR::ExpCreate_LogAnd(expL->getIRExp(), expR->getIRExp(), pos);
      }

      //
      // Exp_LogOrI::v_genStmnt
      //
      void Exp_LogOrI::v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
      {
         GenStmnt_Logical(this, ctx, dst, IR::Code::Jcnd_Tru, 1, 0);
      }

      //
      // Exp_LogOrI::v_getIRExp
      //
      IR::Exp::CRef Exp_LogOrI::v_getIRExp() const
      {
         return IR::ExpCreate_LogOrI(expL->getIRExp(), expR->getIRExp(), pos);
      }
   }
}

// EOF

