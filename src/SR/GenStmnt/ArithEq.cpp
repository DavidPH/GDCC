//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Arithmetic-assignment statement generation.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/Binary.hpp"
#include "SR/Exp/Convert.hpp"
#include "SR/GenStmnt/Move.hpp"
#include "SR/Temporary.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::SR
{
   //
   // GenStmnt_ArithEqIdx
   //
   // Does general statemenet generation with an IR arg for l.
   //
   template<typename ArgT, typename IdxT>
   static void GenStmnt_ArithEqIdx(Exp_Binary const *exp, IR::Code code,
      GenStmntCtx const &ctx, Arg const &dst, Type const *evalT, bool post,
      Arg const &arg, IdxT const &idx)
   {
      // Duplicate to destination, if necessary.
      if(post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
      {
         // Push l.
         ctx.block.addStmnt(IR::Code::Move_W,
            dst.getIRArgStk(), GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0));

         // Assign dst.
         GenStmnt_MovePart(exp, ctx, dst, false, true);
      }

      // Push l.
      ctx.block.addStmnt(IR::Code::Move_W,
         arg.getIRArgStk(), GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0));

      // Convert to evaluation type.
      GenStmnt_ConvertArith(exp, evalT, exp->type, ctx);

      // Attempt to turn r into an IR Arg.
      if(exp->expR->getArg().isIRArg())
      {
         ctx.block.addStmnt(code,
            dst.getIRArgStk(), arg.getIRArgStk(),
            exp->expR->getArg().getIRArg(ctx.prog));
      }

      // Otherwise, just operate on stack.
      else
      {
         // Push r.
         exp->expR->genStmntStk(ctx);

         // Operate on stack.
         ctx.block.addStmnt(code,
            dst.getIRArgStk(), arg.getIRArgStk(), arg.getIRArgStk());
      }

      // Convert to result type.
      GenStmnt_ConvertArith(exp, exp->type, evalT, ctx);

      // Assign l.
      ctx.block.addStmnt(IR::Code::Move_W,
         GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0), arg.getIRArgStk());

      // Duplicate to destination, if necessary.
      if(!post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
      {
         // Push l.
         ctx.block.addStmnt(IR::Code::Move_W,
            dst.getIRArgStk(), GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0));

         // Assign dst.
         GenStmnt_MovePart(exp, ctx, dst, false, true);
      }
   }

   //
   // GenStmnt_ArithEqT
   //
   template<typename ArgT>
   static void GenStmnt_ArithEqT(Exp_Binary const *exp, IR::Code code,
      GenStmntCtx const &ctx, Arg const &dst, Type const *evalT, bool post,
      Arg const &arg)
   {
      using namespace GDCC;

      // If arg address is a constant, then use Arg_Lit address.
      if(arg.data->isIRExp())
      {
         // Evaluate arg's data for side effects.
         arg.data->genStmnt(ctx);

         // Use literal as index.
         GenStmnt_ArithEqIdx<ArgT>(exp, code, ctx, dst, evalT, post, arg,
            IR::Arg_Lit(arg.type->getSizeBytes(), arg.data->getIRExp()));

         return;
      }

      // As a fallback, just evaluate the pointer and store in a temporary.
      {
         // Evaluate arg's data.
         arg.data->genStmntStk(ctx);

         // Move to temporary.
         Temporary tmp{ctx, exp->pos, arg.data->getType()->getSizeWords()};
         ctx.block.addStmnt(IR::Code::Move_W, tmp.getArg(), arg.getIRArgStk());

         // Use temporary as index.
         GenStmnt_ArithEqIdx<ArgT>(exp, code, ctx, dst, evalT, post, arg, tmp.getArg());

         return;
      }
   }

   //
   // GenStmnt_ArithEqT<IR::Arg_Cpy>
   //
   template<>
   void GenStmnt_ArithEqT<IR::Arg_Cpy>(Exp_Binary const *exp, IR::Code,
      GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
   {
      throw Core::ExceptStr(exp->pos, "AddrBase::Cpy op=");
   }

   //
   // GenStmnt_ArithEqT<IR::Arg_Lit>
   //
   template<>
   void GenStmnt_ArithEqT<IR::Arg_Lit>(Exp_Binary const *exp, IR::Code,
      GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
   {
      throw Core::ExceptStr(exp->pos, "AddrBase::Lit op=");
   }

   //
   // GenStmnt_ArithEqT<IR::Arg_Nul>
   //
   template<>
   void GenStmnt_ArithEqT<IR::Arg_Nul>(Exp_Binary const *exp, IR::Code,
      GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
   {
      throw Core::ExceptStr(exp->pos, "AddrBase::Nul op=");
   }

   //
   // GenStmnt_ArithEqT<IR::Arg_Stk>
   //
   template<>
   void GenStmnt_ArithEqT<IR::Arg_Stk>(Exp_Binary const *exp, IR::Code,
      GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
   {
      throw Core::ExceptStr(exp->pos, "AddrBase::Stk op=");
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // GenStmnt_ArithEq
   //
   void GenStmnt_ArithEq(Exp_Binary const *exp, IR::Code code,
      GenStmntCtx const &ctx, Arg const &dst, Type const *evalT, bool post)
   {
      auto arg = exp->expL->getArgDup();

      // Map from generic address space for codegen.
      if(arg.type->getQualAddr().base == IR::AddrBase::Gen)
         arg.type = arg.type->getTypeQual(IR::GetAddrGen());

      // If possible, operate with IR args.
      auto argL = exp->expL->getArg();
      auto argR = exp->expR->getArg();
      if(exp->type->getTypeQual() == evalT->getTypeQual() &&
         argL.isIRArg() && argR.isIRArg())
      {
         auto irArgL = argL.getIRArg(ctx.prog);
         auto irArgR = argR.getIRArg(ctx.prog);

         // Duplicate to destination, if necessary.
         if(post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
         {
            ctx.block.addStmnt(IR::Code::Move_W, dst.getIRArgStk(), irArgL);
            GenStmnt_MovePart(exp, ctx, dst, false, true);
         }

         ctx.block.addStmnt(code, irArgL, irArgL, irArgR);

         // Duplicate to destination, if necessary.
         if(!post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
         {
            ctx.block.addStmnt(IR::Code::Move_W, dst.getIRArgStk(), irArgL);
            GenStmnt_MovePart(exp, ctx, dst, false, true);
         }

         return;
      }

      switch(arg.type->getQualAddr().base)
      {
         #define GDCC_IR_AddrList(addr) \
         case IR::AddrBase::addr: \
            GenStmnt_ArithEqT<IR::Arg_##addr>( \
               exp, code, ctx, dst, evalT, post, arg); \
            break;
         #include "IR/AddrList.hpp"
      }
   }
}

// EOF

