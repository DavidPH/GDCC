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
      GDCC_AST_Exp_ArithImpl(Exp_Add, BinaryAdd)
      GDCC_AST_Exp_ArithImpl(Exp_Div, BinaryDiv)
      GDCC_AST_Exp_ArithImpl(Exp_Mod, BinaryMod)
      GDCC_AST_Exp_ArithImpl(Exp_Mul, BinaryMul)
      GDCC_AST_Exp_ArithImpl(Exp_Sub, BinarySub)

      //
      // GenStmnt_Arith
      //
      void GenStmnt_Arith(Exp_Binary const *exp, IR::Code code,
         GenStmntCtx const &ctx, AST::Arg const &dst)
      {
         if(GenStmntNul(exp, ctx, dst)) return;

         // Evaluate both sub-expressions to stack.
         exp->expL->genStmntStk(ctx);
         exp->expR->genStmntStk(ctx);

         // Operate on stack.
         ctx.block.addStatementArgs(code,
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         GenStmnt_MovePart(exp, ctx, dst, false, true);
      }
   }
}

// EOF

