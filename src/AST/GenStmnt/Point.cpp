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

#include "AST/Exp/Binary.hpp"

#include "AST/GenStmnt/Move.hpp"
#include "AST/Temporary.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // GenStmnt_PointIdx
      //
      static void GenStmnt_PointIdx(Exp const *idx, Core::FastU point,
         GenStmntCtx const &ctx)
      {
         if(point > 1)
         {
            if(idx->isIRExp())
            {
               auto pointV = IR::Value_Fixed(point, idx->getType()->getIRType().tFixed);
               auto pointE = IR::ExpCreate_Value(std::move(pointV), idx->pos);

               ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(),
                  IR::ExpCreate_Mul(idx->getIRExp(), pointE, idx->pos));
            }
            else
            {
               idx->genStmntStk(ctx);
               ctx.block.addStatementArgs({IR::Code::Move_W, 1},
                  IR::Arg_Stk(), point);
               ctx.block.addStatementArgs({IR::Code::MulU_W, 1},
                  IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());
            }
         }
         else
            idx->genStmntStk(ctx);
      }

      //
      // GenStmnt_PointEqIdx
      //
      // Does general statemenet generation with an IR arg for l.
      //
      template<typename ArgT, typename IdxT>
      static void GenStmnt_PointEqIdx(Exp_Binary const *exp, IR::OpCode op,
         GenStmntCtx const &ctx, Arg const &dst, bool post, Arg const &arg,
         IdxT const &idx)
      {
         // Duplicate to destination, if necessary.
         if(post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
         {
            // Push l.
            for(Core::FastU n = 0, e = arg.type->getSizeWords(); n != e; ++n)
               GenStmnt_MoveWordGetT<ArgT>(exp, ctx, arg, idx, n);

            // Assign dst.
            GenStmnt_MovePart(exp, ctx, dst, false, true);
         }

         // Push l.
         for(Core::FastU n = 0, e = arg.type->getSizeWords(); n != e; ++n)
            GenStmnt_MoveWordGetT<ArgT>(exp, ctx, arg, idx, n);

         // Push r.
         GenStmnt_PointIdx(exp->expR, exp->type->getBaseType()->getSizePoint(), ctx);

         // Operate on stack.
         ctx.block.addStatementArgs(op,
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Assign l.
         for(Core::FastU n = arg.type->getSizeWords(); n--;)
            GenStmnt_MoveWordSetT<ArgT>(exp, ctx, arg, idx, n);

         // Duplicate to destination, if necessary.
         if(!post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
         {
            // Push l.
            for(Core::FastU n = 0, e = arg.type->getSizeWords(); n != e; ++n)
               GenStmnt_MoveWordGetT<ArgT>(exp, ctx, arg, idx, n);

            // Assign dst.
            GenStmnt_MovePart(exp, ctx, dst, false, true);
         }
      }

      //
      // GenStmnt_PointEqT
      //
      template<typename ArgT>
      static void GenStmnt_PointEqT(Exp_Binary const *exp, IR::OpCode op,
         GenStmntCtx const &ctx, Arg const &dst, bool post, Arg const &arg)
      {
         // If arg address is a constant, then use Arg_Lit address.
         if(arg.data->isIRExp())
         {
            // Evaluate arg's data for side effects.
            arg.data->genStmnt(ctx);

            // Use literal as index.
            GenStmnt_PointEqIdx<ArgT>(exp, op, ctx, dst, post, arg,
               IR::Arg_Lit(arg.data->getIRExp()));

            return;
         }

         // As a fallback, just evaluate the pointer and store in a temporary.
         {
            // Evaluate arg's data.
            arg.data->genStmntStk(ctx);

            // Move to temporary.
            Temporary tmp{ctx, exp->pos, arg.data->getType()->getSizeWords()};
            ctx.block.addStatementArgs({IR::Code::Move_W, tmp.size()},
               tmp.getArg(), IR::Arg_Stk());

            // Use temporary as index.
            GenStmnt_PointEqIdx<ArgT>(exp, op, ctx, dst, post, arg, tmp.getArg());

            return;
         }
      }

      //
      // GenStmnt_PointEqT<IR::Arg_Cpy>
      //
      template<> void GenStmnt_PointEqT<IR::Arg_Cpy>(Exp_Binary const *exp,
         IR::OpCode, GenStmntCtx const &, Arg const &, bool, Arg const &)
      {
         throw Core::ExceptStr(exp->pos, "AddrBase::Cpy op=");
      }

      //
      // GenStmnt_PointEqT<IR::Arg_Lit>
      //
      template<> void GenStmnt_PointEqT<IR::Arg_Lit>(Exp_Binary const *exp,
         IR::OpCode, GenStmntCtx const &, Arg const &, bool, Arg const &)
      {
         throw Core::ExceptStr(exp->pos, "AddrBase::Lit op=");
      }

      //
      // GenStmnt_PointEqT<IR::Arg_Nul>
      //
      template<> void GenStmnt_PointEqT<IR::Arg_Nul>(Exp_Binary const *exp,
         IR::OpCode, GenStmntCtx const &, Arg const &, bool, Arg const &)
      {
         throw Core::ExceptStr(exp->pos, "AddrBase::Nul op=");
      }

      //
      // GenStmnt_PointEqT<IR::Arg_Stk>
      //
      template<> void GenStmnt_PointEqT<IR::Arg_Stk>(Exp_Binary const *exp,
         IR::OpCode, GenStmntCtx const &, Arg const &, bool, Arg const &)
      {
         throw Core::ExceptStr(exp->pos, "AddrBase::Stk op=");
      }
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      //
      // GenStmnt_Point
      //
      void GenStmnt_Point(Exp_Binary const *exp, IR::OpCode op,
         GenStmntCtx const &ctx, Arg const &dst)
      {
         if(GenStmntNul(exp, ctx, dst)) return;

         // Evaluate pointer to stack.
         exp->expL->genStmntStk(ctx);

         // Evaluate index, adjusting if necessary.
         GenStmnt_PointIdx(exp->expR, exp->type->getBaseType()->getSizePoint(), ctx);

         // Add on stack.
         ctx.block.addStatementArgs(op,
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         GenStmnt_MovePart(exp, ctx, dst, false, true);
      }

      //
      // GenStmnt_PointEq
      //
      void GenStmnt_PointEq(Exp_Binary const *exp, IR::OpCode op,
         GenStmntCtx const &ctx, Arg const &dst, bool post)
      {
         auto arg = exp->expL->getArgDup();

         // Map from generic address space for codegen.
         if(arg.type->getQualAddr().base == IR::AddrBase::Gen)
            arg.type = arg.type->getTypeQual(IR::GetAddrGen());

         // If possible, operate with IR args.
         auto point = exp->type->getBaseType()->getSizePoint();
         auto argL = exp->expL->getArg();
         auto argR = exp->expR->getArg();
         if(exp->type->getSizeWords() == 1 && argL.isIRArg() &&
            (exp->expR->isIRExp() || (argR.isIRArg() && point == 1)))
         {
            auto irArgL = argL.getIRArg(ctx.prog);
            auto irArgR = argR.getIRArg(ctx.prog);

            IR::OpCode opMove = {IR::Code::Move_W, argL.type->getSizeWords()};

            // Duplicate to destination, if necessary.
            if(post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
            {
               ctx.block.addStatementArgs(opMove, IR::Arg_Stk(), irArgL);
               GenStmnt_MovePart(exp, ctx, dst, false, true);
            }

            if(point == 1)
               ctx.block.addStatementArgs(op, irArgL, irArgL, irArgR);
            else
            {
               auto pointV = IR::Value_Fixed(point,
                  exp->expR->getType()->getIRType().tFixed);
               auto pointE = IR::ExpCreate_Value(std::move(pointV), exp->pos);

               ctx.block.addStatementArgs(op, irArgL, irArgL,
                  IR::ExpCreate_Mul(exp->expR->getIRExp(), pointE, exp->pos));
            }

            // Duplicate to destination, if necessary.
            if(!post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
            {
               ctx.block.addStatementArgs(opMove, IR::Arg_Stk(), irArgL);
               GenStmnt_MovePart(exp, ctx, dst, false, true);
            }

            return;
         }

         switch(arg.type->getQualAddr().base)
         {
            #define GDCC_IR_AddrList(addr) \
            case IR::AddrBase::addr: \
               GenStmnt_PointEqT<IR::Arg_##addr>( \
                  exp, op, ctx, dst, post, arg); \
               break;
            #include "IR/AddrList.hpp"
         }
      }
   }
}

// EOF

