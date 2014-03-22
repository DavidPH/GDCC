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

         // Add on stack.
         ctx.block.addStatementArgs(IR::Code::AddU_W,
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         GenStmnt_MovePart(this, ctx, dst, false, true);
      }
   }
}

// EOF

