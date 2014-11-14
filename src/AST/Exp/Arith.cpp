//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree arithmetic expression bases.
//
//-----------------------------------------------------------------------------

#include "AST/Exp/Arith.hpp"

#include "AST/Arg.hpp"
#include "AST/Type.hpp"

#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_AST_Exp_ArithImpl
//
#define GDCC_AST_Exp_ArithImpl(class, ir) \
   IR::Exp::CRef class::v_getIRExp() const \
   { \
      return IR::ExpCreate_##ir( \
         expL->getIRExp(), expR->getIRExp(), pos); \
   }


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      GDCC_AST_Exp_ArithImpl(Exp_Add, Add)
      GDCC_AST_Exp_ArithImpl(Exp_Div, Div)
      GDCC_AST_Exp_ArithImpl(Exp_Mod, Mod)
      GDCC_AST_Exp_ArithImpl(Exp_Mul, Mul)
      GDCC_AST_Exp_ArithImpl(Exp_Sub, Sub)

      //
      // GenStmnt_Arith
      //
      void GenStmnt_Arith(Exp_Binary const *exp, IR::Code code,
         GenStmntCtx const &ctx, AST::Arg const &dst)
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
               irArgL = argL.getIRArg();
               irArgR = argR.getIRArg();
            }
            else
            {
               // Evaluate left expression to stack.
               exp->expL->genStmntStk(ctx);

               irArgL = IR::Arg_Stk();
               irArgR = argR.getIRArg();
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
            ctx.block.addStatementArgs(code, dst.getIRArg(), irArgL, irArgR);
         }
         else
         {
            ctx.block.addStatementArgs(code, IR::Arg_Stk(), irArgL, irArgR);

            // Move to destination.
            GenStmnt_MovePart(exp, ctx, dst, false, true);
         }
      }
   }
}

// EOF

