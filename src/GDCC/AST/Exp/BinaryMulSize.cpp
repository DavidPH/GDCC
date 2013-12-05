//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree Size-type multiply expression.
//
//-----------------------------------------------------------------------------

#include "BinaryMulSize.hpp"

#include "../Arg.hpp"
#include "../Type.hpp"

#include "GDCC/IR/Block.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_BinaryMulSize constructor
      //
      Exp_BinaryMulSize::Exp_BinaryMulSize(Exp const *l, Exp const *r, Origin pos) :
         Super{l, r, pos}
      {
      }

      //
      // Exp_BinaryMulSize::v_genStmnt
      //
      void Exp_BinaryMulSize::v_genStmnt(IR::Block &block, Function *fn,
         Arg const &dst) const
      {
         // If only evaluating for side-effect, the multiply itself has none, so
         // just evaluate sub-expressions.
         if(dst.type->getQualAddr().base == IR::AddrBase::Nul)
         {
            expL->genStmnt(block, fn);
            expR->genStmnt(block, fn);
            return;
         }

         // Evaluate both sub-expressions to stack.
         Arg src{Type::Size, IR::AddrBase::Stk};
         expL->genStmnt(block, fn, src);
         expR->genStmnt(block, fn, src);

         // Multiply on stack.
         block.addStatementArgs(IR::Code::MulU_W, IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         genStmntMovePart(block, fn, dst, false, true);
      }

      //
      // Exp_BinaryMulSize::v_getIRExp
      //
      IR::Exp::CRef Exp_BinaryMulSize::v_getIRExp() const
      {
         return IR::ExpCreate_BinaryMul(expL->getIRExp(), expR->getIRExp(), pos);
      }

      //
      // Exp_BinaryMulSize::v_getType
      //
      Type::CRef Exp_BinaryMulSize::v_getType() const
      {
         return Type::Size;
      }
   }
}

// EOF

