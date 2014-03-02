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

#include "AST/Arg.hpp"
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

