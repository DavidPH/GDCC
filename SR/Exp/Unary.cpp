//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation unary expression base.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/Unary.hpp"

#include "SR/Arg.hpp"
#include "SR/Type.hpp"

#include "IR/Block.hpp"
#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // Exp_Unary constructor
   //
   Exp_Unary::Exp_Unary(Type const *t, Exp const *e, Core::Origin pos_) :
      Super{pos_}, exp{e}, type{t}
   {
   }

   //
   // Exp_Unary Exp_Unary
   //
   Exp_Unary::~Exp_Unary()
   {
   }

   //
   // Exp_Unary::v_getType
   //
   Type::CRef Exp_Unary::v_getType() const
   {
      return type;
   }

   //
   // Exp_Unary::v_isEffect
   //
   bool Exp_Unary::v_isEffect() const
   {
      return exp->isEffect();
   }

   //
   // Exp_Unary::v_isIRExp
   //
   bool Exp_Unary::v_isIRExp() const
   {
      return exp->isIRExp();
   }

   //
   // Exp_Unary::v_isNoAuto
   //
   bool Exp_Unary::v_isNoAuto() const
   {
      return exp->isNoAuto();
   }

   //
   // Exp_Inv::v_getIRExp
   //
   IR::Exp::CRef Exp_Inv::v_getIRExp() const
   {
      return IR::ExpCreate_Inv(exp->getIRExp(), pos);
   }

   //
   // Exp_Neg::v_getIRExp
   //
   IR::Exp::CRef Exp_Neg::v_getIRExp() const
   {
      return IR::ExpCreate_Neg(exp->getIRExp(), pos);
   }

   //
   // GenStmntNul
   //
   bool GenStmntNul(Exp_Unary const *exp, GenStmntCtx const &ctx, Arg const &dst)
   {
      // If only evaluating for side-effect, just evaluate sub-expression.
      if(dst.type->getQualAddr().base == IR::AddrBase::Nul)
      {
         exp->exp->genStmnt(ctx);
         return true;
      }

      return false;
   }

   //
   // GenStmnt_UnaryCode
   //
   void GenStmnt_UnaryCode(Exp_Unary const *exp, IR::Code code,
      GenStmntCtx const &ctx, Arg const &dst)
   {
      if(GenStmntNul(exp, ctx, dst)) return;

      GenStmnt_MoveDstPre(exp, ctx, dst);

      // Evaluate sub-expression to stack.
      exp->exp->genStmntStk(ctx);

      // Operate on stack.
      ctx.block.addStmnt(code, dst.getIRArgStk(), exp->exp->getIRArgStk());

      // Move to destination.
      GenStmnt_MoveDstSuf(exp, ctx, dst);
   }
}

// EOF

