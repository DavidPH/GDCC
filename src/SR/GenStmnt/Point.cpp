//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator +" pointer expressions.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/Binary.hpp"

#include "SR/GenStmnt/Move.hpp"
#include "SR/Temporary.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::SR
{
   //
   // GenStmnt_PointIdx
   //
   static void GenStmnt_PointIdx(Exp const *idx, Core::FastU point,
      GenStmntCtx const &ctx)
   {
      if(point > 1)
      {
         ctx.block.setArgSize();
         if(idx->isIRExp())
         {
            auto pointV = IR::Value_Fixed(point, idx->getType()->getIRType().tFixed);
            auto pointE = IR::ExpCreate_Value(std::move(pointV), idx->pos);

            ctx.block.addStmnt(IR::Code::Move, IR::Block::Stk(),
               IR::ExpCreate_Mul(idx->getIRExp(), pointE, idx->pos));
         }
         else
         {
            idx->genStmntStk(ctx);
            ctx.block.addStmnt(IR::Code::Move, IR::Block::Stk(), point);
            ctx.block.addStmnt(IR::Code::MulU,
               IR::Block::Stk(), IR::Block::Stk(), IR::Block::Stk());
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
   static void GenStmnt_PointEqIdx(Exp_Binary const *exp, IR::Code code,
      GenStmntCtx const &ctx, Arg const &dst, bool post, Arg const &arg,
      IdxT const &idx)
   {
      IR::Arg_Stk stkL = exp->expL->getIRArgStk();
      IR::Arg_Stk stkR = exp->expR->getIRArgStk();

      // Duplicate to destination, if necessary.
      if(post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
      {
         // Push l.
         ctx.block.addStmnt(IR::Code::Move, stkL,
            GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0));

         // Assign dst.
         GenStmnt_MovePart(exp, ctx, dst, false, true);
      }

      // Push l.
      ctx.block.addStmnt(IR::Code::Move, stkL,
         GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0));

      // Push r.
      GenStmnt_PointIdx(exp->expR, exp->type->getBaseType()->getSizePoint(), ctx);

      // Operate on stack.
      ctx.block.addStmnt(code, stkR, stkR, stkR);

      // Assign l.
      ctx.block.addStmnt(IR::Code::Move,
         GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0), stkL);

      // Duplicate to destination, if necessary.
      if(!post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
      {
         // Push l.
         ctx.block.addStmnt(IR::Code::Move,
            stkL, GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0));

         // Assign dst.
         GenStmnt_MovePart(exp, ctx, dst, false, true);
      }
   }

   //
   // GenStmnt_PointEqT
   //
   template<typename ArgT>
   static void GenStmnt_PointEqT(Exp_Binary const *exp, IR::Code code,
      GenStmntCtx const &ctx, Arg const &dst, bool post, Arg const &arg)
   {
      // If arg address is a constant, then use Arg_Lit address.
      if(arg.data->isIRExp())
      {
         // Evaluate arg's data for side effects.
         arg.data->genStmnt(ctx);

         // Use literal as index.
         GenStmnt_PointEqIdx<ArgT>(exp, code, ctx, dst, post, arg, arg.data->getIRArgLit());

         return;
      }

      // As a fallback, just evaluate the pointer and store in a temporary.
      {
         // Evaluate arg's data.
         arg.data->genStmntStk(ctx);

         // Move to temporary.
         Temporary tmp{ctx, exp->pos, arg.data->getType()->getSizeWords()};
         ctx.block.addStmnt(IR::Code::Move, tmp.getArg(), tmp.getArgStk());

         // Use temporary as index.
         GenStmnt_PointEqIdx<ArgT>(exp, code, ctx, dst, post, arg, tmp.getArg());

         return;
      }
   }

   //
   // GenStmnt_PointEqT<IR::Arg_Cpy>
   //
   template<> void GenStmnt_PointEqT<IR::Arg_Cpy>(Exp_Binary const *exp,
      IR::Code, GenStmntCtx const &, Arg const &, bool, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Cpy op=");
   }

   //
   // GenStmnt_PointEqT<IR::Arg_Lit>
   //
   template<> void GenStmnt_PointEqT<IR::Arg_Lit>(Exp_Binary const *exp,
      IR::Code, GenStmntCtx const &, Arg const &, bool, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Lit op=");
   }

   //
   // GenStmnt_PointEqT<IR::Arg_Nul>
   //
   template<> void GenStmnt_PointEqT<IR::Arg_Nul>(Exp_Binary const *exp,
      IR::Code, GenStmntCtx const &, Arg const &, bool, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Nul op=");
   }

   //
   // GenStmnt_PointEqT<IR::Arg_Stk>
   //
   template<> void GenStmnt_PointEqT<IR::Arg_Stk>(Exp_Binary const *exp,
      IR::Code, GenStmntCtx const &, Arg const &, bool, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Stk op=");
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // GenStmnt_Point
   //
   void GenStmnt_Point(Exp_Binary const *exp, IR::Code code,
      GenStmntCtx const &ctx, Arg const &dst)
   {
      if(GenStmntNul(exp, ctx, dst)) return;

      IR::Arg_Stk stk = exp->expR->getIRArgStk();

      // Evaluate pointer to stack.
      exp->expL->genStmntStk(ctx);

      // Evaluate index, adjusting if necessary.
      GenStmnt_PointIdx(exp->expR, exp->type->getBaseType()->getSizePoint(), ctx);

      // Add on stack.
      ctx.block.addStmnt(code, stk, stk, stk);

      // Move to destination.
      GenStmnt_MovePart(exp, ctx, dst, false, true);
   }

   //
   // GenStmnt_PointEq
   //
   void GenStmnt_PointEq(Exp_Binary const *exp, IR::Code code,
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

         // Duplicate to destination, if necessary.
         if(post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
         {
            ctx.block.addStmnt(IR::Code::Move, argL.getIRArgStk(), irArgL);
            GenStmnt_MovePart(exp, ctx, dst, false, true);
         }

         if(point == 1)
            ctx.block.addStmnt(code, irArgL, irArgL, irArgR);
         else
         {
            auto pointV = IR::Value_Fixed(point,
               exp->expR->getType()->getIRType().tFixed);
            auto pointE = IR::ExpCreate_Value(std::move(pointV), exp->pos);

            ctx.block.setArgSize().addStmnt(code, irArgL, irArgL,
               IR::ExpCreate_Mul(exp->expR->getIRExp(), pointE, exp->pos));
         }

         // Duplicate to destination, if necessary.
         if(!post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
         {
            ctx.block.addStmnt(IR::Code::Move, argL.getIRArgStk(), irArgL);
            GenStmnt_MovePart(exp, ctx, dst, false, true);
         }

         return;
      }

      switch(arg.type->getQualAddr().base)
      {
         #define GDCC_IR_AddrList(addr) \
         case IR::AddrBase::addr: \
            GenStmnt_PointEqT<IR::Arg_##addr>( \
               exp, code, ctx, dst, post, arg); \
            break;
         #include "IR/AddrList.hpp"
      }
   }
}

// EOF

