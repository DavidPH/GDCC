//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation arithmetic expression bases.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/Arith.hpp"

#include "SR/Arg.hpp"
#include "SR/Type.hpp"

#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_SR_Exp_ArithImpl
//
#define GDCC_SR_Exp_ArithImpl(class, ir) \
   IR::Exp::CRef class::v_getIRExp() const \
   { \
      return IR::ExpCreate_##ir( \
         expL->getIRExp(), expR->getIRExp(), pos); \
   }


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace SR
   {
      GDCC_SR_Exp_ArithImpl(Exp_Add, Add)
      GDCC_SR_Exp_ArithImpl(Exp_Div, Div)
      GDCC_SR_Exp_ArithImpl(Exp_Mod, Mod)
      GDCC_SR_Exp_ArithImpl(Exp_Mul, Mul)
      GDCC_SR_Exp_ArithImpl(Exp_Sub, Sub)

      //
      // GenStmnt_Arith
      //
      void GenStmnt_Arith(Exp_Binary const *exp, IR::OpCode op,
         GenStmntCtx const &ctx, SR::Arg const &dst)
      {
         if(GenStmntNul(exp, ctx, dst)) return;

         auto argL = exp->expL->getArg();
         auto argR = exp->expR->getArg();

         IR::Arg irArgL, irArgR;

         // Try to use IR args instead of stack args.
         // However, avoid a stack arg only on the left for asymmetric ops.
         if(argR.isIRArg())
         {
            if(argL.isIRArg())
            {
               irArgL = argL.getIRArg(ctx.prog);
               irArgR = argR.getIRArg(ctx.prog);
            }
            else
            {
               // Evaluate left expression to stack.
               exp->expL->genStmntStk(ctx);

               irArgL = IR::Arg_Stk();
               irArgR = argR.getIRArg(ctx.prog);
            }
         }
         else
         {
            // Evaluate both sub-expressions to stack.
            exp->expL->genStmntStk(ctx);
            exp->expR->genStmntStk(ctx);

            irArgL = IR::Arg_Stk();
            irArgR = IR::Arg_Stk();
         }

         if(dst.isIRArg())
         {
            auto irDst = dst.getIRArg(ctx.prog);
            ctx.block.addStatementArgs(op, irDst, irArgL, irArgR);
         }
         else
         {
            ctx.block.addStatementArgs(op, IR::Arg_Stk(), irArgL, irArgR);

            // Move to destination.
            GenStmnt_MovePart(exp, ctx, dst, false, true);
         }
      }
   }
}

// EOF

