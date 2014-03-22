//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator =" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Assign.hpp"

#include "AST/Arg.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_Assign constructor
      //
      Exp_Assign::Exp_Assign(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos_) :
         Super{l->getType(), l, r, pos_}
      {
      }

      //
      // Exp_Assign::v_genStmnt
      //
      void Exp_Assign::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         if(dst.type->getQualAddr().base == IR::AddrBase::Nul)
            expR->genStmnt(ctx, expL->getArgDst());
         else
            GenStmnt_Move(this, ctx, dst, expL->getArgDup(), expR->getArgSrc());
      }

      //
      // Exp_Assign::v_isEffect
      //
      bool Exp_Assign::v_isEffect() const
      {
         // Assignment is the most basic form of side-effect.
         return true;
      }

      //
      // Exp_Assign::v_isIRExp
      //
      bool Exp_Assign::v_isIRExp() const
      {
         // TODO: If the right operand is a constant and the cast can be done at
         // compile-time, then result of this expression can be known at
         // compile-time even if it still needs to be evaluated.
         return false;
      }

      //
      // ExpCreate_Assign
      //
      AST::Exp::CRef ExpCreate_Assign(AST::Exp const *l, AST::Exp const *r,
         Core::Origin pos)
      {
         if(!IsModLValue(l))
            throw Core::ExceptStr(l->pos, "expected modifiable lvalue");

         auto expR = ExpPromo_Assign(l->getType(), r, pos);

         return static_cast<AST::Exp::CRef>(new Exp_Assign(l, expR, pos));
      }
   }
}

// EOF

