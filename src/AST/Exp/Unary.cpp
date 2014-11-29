//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree unary expression base.
//
//-----------------------------------------------------------------------------

#include "AST/Exp/Unary.hpp"

#include "AST/Arg.hpp"
#include "AST/Type.hpp"

#include "IR/Block.hpp"
#include "IR/Exp.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
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
      AST::Type::CRef Exp_Unary::v_getType() const
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

         // Evaluate both sub-expressions to stack.
         exp->exp->genStmntStk(ctx);

         // Operate on stack.
         ctx.block.addStatementArgs(code, IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         GenStmnt_MovePart(exp, ctx, dst, false, true);
      }
   }
}

// EOF

