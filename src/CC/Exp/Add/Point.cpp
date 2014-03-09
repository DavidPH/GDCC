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
      void Exp_AddPoint::v_genStmnt(IR::Block &block, AST::Function *fn,
         AST::Arg const &dst) const
      {
         if(GenStmntNul(this, block, fn, dst)) return;

         // Evaluate pointer to stack.
         expL->genStmntStk(block, fn);

         // Evaluate index, adjusting if necessary.
         auto point = type->getBaseType()->getSizePoint();
         if(point > 1)
         {
            auto lit = ExpCreate_LitInt(expR->getType(), point, pos);
            ExpCreate_Mul(expR, lit, pos)->genStmntStk(block, fn);
         }
         else
            expR->genStmntStk(block, fn);

         // Add on stack.
         block.addStatementArgs(IR::Code::AddU_W,
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         genStmntMovePart(block, fn, dst, false, true);
      }
   }
}

// EOF

