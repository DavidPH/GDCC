//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree conversion expression bases.
//
//-----------------------------------------------------------------------------

#include "AST/Exp/Convert.hpp"

#include "AST/Type.hpp"

#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_Convert::v_getIRExp
      //
      IR::Exp::CRef Exp_Convert::v_getIRExp() const
      {
         // The conversion logic is handled in Exp::getIRExp.
         return exp->getIRExp();
      }

      //
      // Exp_ConvertArith::v_genStmnt
      //
      void Exp_ConvertArith::v_genStmnt(GenStmntCtx const &ctx,
         Arg const &dst) const
      {
         if(GenStmntNul(this, ctx, dst)) return;

         // Evaluate expression to stack.
         exp->genStmntStk(ctx);

         // Convert on stack.
         GenStmnt_ConvertArith(this, type, exp->getType(), ctx);

         // Move to destination.
         GenStmnt_MovePart(exp, ctx, dst, false, true);
      }
   }
}

// EOF

