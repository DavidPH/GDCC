//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation logical expression bases.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/Logical.hpp"

#include "SR/Arg.hpp"
#include "SR/Function.hpp"
#include "SR/Type.hpp"

#include "IR/Block.hpp"
#include "IR/Exp.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace SR
   {
      //
      // GenStmnt_Logical
      //
      static void GenStmnt_Logical(Exp_Binary const *exp,
         GenStmntCtx const &ctx, Arg const &dst, bool valShrt)
      {
         IR::OpCode opShrt{valShrt ? IR::Code::Jcnd_Tru : IR::Code::Jcnd_Nil, 1};
         bool valLong = !valShrt;

         // Only evaluating for side-effects?
         if(dst.type->getQualAddr().base == IR::ArgBase::Nul)
         {
            // If right operand has no side effects, then it does not need to
            // be evaluated regardless of the value of the left.
            if(!exp->expR->isEffect())
            {
               exp->expL->genStmnt(ctx);
            }
            else
            {
               // If left operand would short, then do not bother doing codegen
               // for right.
               if((!valShrt && exp->expL->isZero()) ||
                  (valShrt && exp->expL->isNonzero()))
               {
                  exp->expL->genStmnt(ctx);
               }
               else
               {
                  IR::Glyph labelEnd = {ctx.prog, ctx.fn->genLabel()};

                  exp->expL->genStmntStk(ctx);
                  ctx.block.addStatementArgs(opShrt, IR::Arg_Stk(), labelEnd);

                  exp->expR->genStmnt(ctx);
                  ctx.block.addLabel(labelEnd);
               }
            }

            return;
         }

         // Is left operand known to not short?
         if((valShrt && exp->expL->isZero()) ||
            (!valShrt && exp->expL->isNonzero()))
         {
            exp->expL->genStmnt(ctx);

            // If expR is already boolean, use its result directly.
            if(exp->expR->isBoolean())
            {
               exp->expR->genStmnt(ctx, dst);
            }
            else
            {
               exp->expR->genStmntStk(ctx);

               ctx.block.addStatementArgs({IR::Code::LNot, 1},
                  IR::Arg_Stk(), IR::Arg_Stk());
               ctx.block.addStatementArgs({IR::Code::LNot, 1},
                  IR::Arg_Stk(), IR::Arg_Stk());

               // Move to destination.
               GenStmnt_MovePart(exp, ctx, dst, false, true);
            }

            return;
         }

         // Inversely, if the left operand was known to short, exp would pass
         // isIRExp and this would only be called to evaluate side effects.

         // TODO: If expR is trivial, possibly forego short-circuiting.

         IR::Glyph labelEnd   = {ctx.prog, ctx.fn->genLabel()};
         IR::Glyph labelShort = {ctx.prog, ctx.fn->genLabel()};

         // Evaluate left operand to stack.
         exp->expL->genStmntStk(ctx);

         // Possibly jump to short result.
         ctx.block.addStatementArgs(opShrt, IR::Arg_Stk(), labelShort);

         // Evaluate right operand to stack.
         exp->expR->genStmntStk(ctx);

         // If expR is already boolean, use its result directly.
         if(exp->expR->isBoolean())
         {
            ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelEnd);
         }
         else
         {
            // Possibly jump to short result.
            ctx.block.addStatementArgs(opShrt, IR::Arg_Stk(), labelShort);

            // Long result.
            ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), valLong);
            ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelEnd);
         }

         // Short result.
         ctx.block.addLabel(labelShort);
         ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), valShrt);

         // End.
         ctx.block.addLabel(labelEnd);

         // Move to destination.
         GenStmnt_MovePart(exp, ctx, dst, false, true);
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace SR
   {
      //
      // Exp_LogAnd::v_genStmnt
      //
      void Exp_LogAnd::v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
      {
         GenStmnt_Logical(this, ctx, dst, false);
      }

      //
      // Exp_LogAnd::v_getIRExp
      //
      IR::Exp::CRef Exp_LogAnd::v_getIRExp() const
      {
         if(expL->isZero() || expR->isZero())
            return IR::ExpCreate_Value(IR::Value_Fixed::Bool0, pos);

         if(expL->isNonzero() && expR->isNonzero())
            return IR::ExpCreate_Value(IR::Value_Fixed::Bool1, pos);

         return IR::ExpCreate_LogAnd(expL->getIRExp(), expR->getIRExp(), pos);
      }

      //
      // Exp_LogAnd::v_isBoolean
      //
      bool Exp_LogAnd::v_isBoolean() const
      {
         return true;
      }

      //
      // Exp_LogAnd::v_isIRExp
      //
      bool Exp_LogAnd::v_isIRExp() const
      {
         if(expL->isZero()    || expR->isZero())    return true;
         if(expL->isNonzero() && expR->isNonzero()) return true;

         return Super::v_isIRExp();
      }

      //
      // Exp_LogOrI::v_genStmnt
      //
      void Exp_LogOrI::v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
      {
         GenStmnt_Logical(this, ctx, dst, true);
      }

      //
      // Exp_LogOrI::v_getIRExp
      //
      IR::Exp::CRef Exp_LogOrI::v_getIRExp() const
      {
         if(expL->isZero() && expR->isZero())
            return IR::ExpCreate_Value(IR::Value_Fixed::Bool0, pos);

         if(expL->isNonzero() || expR->isNonzero())
            return IR::ExpCreate_Value(IR::Value_Fixed::Bool1, pos);

         return IR::ExpCreate_LogOrI(expL->getIRExp(), expR->getIRExp(), pos);
      }

      //
      // Exp_LogOrI::v_isBoolean
      //
      bool Exp_LogOrI::v_isBoolean() const
      {
         return true;
      }

      //
      // Exp_LogOrI::v_isIRExp
      //
      bool Exp_LogOrI::v_isIRExp() const
      {
         if(expL->isZero()    && expR->isZero())    return true;
         if(expL->isNonzero() || expR->isNonzero()) return true;

         return Super::v_isIRExp();
      }
   }
}

// EOF

