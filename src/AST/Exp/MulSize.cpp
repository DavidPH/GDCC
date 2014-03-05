//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree Size-type multiply expression.
//
//-----------------------------------------------------------------------------

#include "AST/Exp/MulSize.hpp"

#include "AST/Type.hpp"

#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_MulSize constructor
      //
      Exp_MulSize::Exp_MulSize(Exp const *l, Exp const *r,
         Core::Origin pos_) : Super{l, r, pos_}
      {
      }

      //
      // Exp_MulSize::v_genStmnt
      //
      void Exp_MulSize::v_genStmnt(IR::Block &block, Function *fn,
         Arg const &dst) const
      {
         if(tryGenStmntNul(block, fn, dst)) return;

         // Evaluate both sub-expressions to stack.
         expL->genStmntStk(block, fn);
         expR->genStmntStk(block, fn);

         // Multiply on stack.
         block.addStatementArgs(IR::Code::MulU_W,
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         genStmntMovePart(block, fn, dst, false, true);
      }

      //
      // Exp_MulSize::v_getIRExp
      //
      IR::Exp::CRef Exp_MulSize::v_getIRExp() const
      {
         return IR::ExpCreate_BinaryMul(expL->getIRExp(), expR->getIRExp(), pos);
      }

      //
      // Exp_MulSize::v_getType
      //
      Type::CRef Exp_MulSize::v_getType() const
      {
         return Type::Size;
      }

      //
      // ExpCreate_MulSize
      //
      Exp::CRef ExpCreate_MulSize(Exp const *l, Exp const *r)
      {
         auto pos = l->pos ? l->pos : r->pos;
         return static_cast<Exp::CRef>(new Exp_MulSize(l, r, pos));
      }
   }
}

// EOF

