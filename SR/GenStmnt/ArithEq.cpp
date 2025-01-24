//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
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
   // GenStmnt_ArithEqBitIdx
   //
   // Does general statemenet generation with an IR arg for l.
   //
   template<typename ArgT, typename IdxT>
   static void GenStmnt_ArithEqBitIdx(Exp_Binary const *exp, IR::Code code,
      GenStmntCtx const &ctx, Arg const &dst, Type const *evalT, bool post,
      Arg const &arg, IdxT const &idx)
   {
      // Bitfield info.
      auto bitsT = exp->expL->getType();
      bool bitsS = bitsT->getSizeBitsS();
      auto bitsW = ctx.block.getExp(bitsT->getSizeBitsF() + bitsT->getSizeBitsI() + bitsS);
      auto bitsO = ctx.block.getExp(bitsT->getSizeBitsO());
      auto bitsG = bitsS ? IR::CodeBase::Bges : IR::CodeBase::Bget;

      IR::Arg_Stk stkR = exp->expR->getIRArgStk();

      ctx.block.setArgSize();

      // Duplicate to destination, if necessary.
      if(post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
      {
         GenStmnt_MoveDstPre(exp, ctx, dst);

         // Push l.
         ctx.block.addStmnt(bitsG,
            dst.getIRArgStk(), GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0),
            bitsW, bitsO);

         // Assign dst.
         GenStmnt_MoveDstSuf(exp, ctx, dst);
      }

      // Push l.
      ctx.block.addStmnt(bitsG,
         arg.getIRArgStk(), GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0),
         bitsW, bitsO);

      // Convert to evaluation type.
      GenStmnt_ConvertArith(exp, evalT, exp->type, ctx);

      // Attempt to turn r into an IR Arg.
      if(exp->expR->getArg().isIRArg())
      {
         ctx.block.addStmnt(code, stkR, stkR, exp->expR->getArg().getIRArg(ctx.prog));
      }

      // Otherwise, just operate on stack.
      else
      {
         // Push r.
         exp->expR->genStmntStk(ctx);

         // Operate on stack.
         ctx.block.addStmnt(code, stkR, stkR, stkR);
      }

      // Convert to result type.
      GenStmnt_ConvertArith(exp, exp->type, evalT, ctx);

      // Assign l.
      ctx.block.addStmnt(IR::CodeBase::Bset,
         GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0), arg.getIRArgStk(),
         bitsW, bitsO);

      // Duplicate to destination, if necessary.
      if(!post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
      {
         GenStmnt_MoveDstPre(exp, ctx, dst);

         // Push l.
         ctx.block.addStmnt(bitsG,
            dst.getIRArgStk(), GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0),
            bitsW, bitsO);

         // Assign dst.
         GenStmnt_MoveDstSuf(exp, ctx, dst);
      }
   }

   //
   // GenStmnt_ArithEqBitT
   //
   template<typename ArgT>
   static void GenStmnt_ArithEqBitT(Exp_Binary const *exp, IR::Code code,
      GenStmntCtx const &ctx, Arg const &dst, Type const *evalT, bool post,
      Arg const &arg)
   {
      // If arg address is a constant, then use Arg_Lit address.
      if(arg.data->isIRExp())
      {
         // Evaluate arg's data for side effects.
         arg.data->genStmnt(ctx);

         // Use literal as index.
         GenStmnt_ArithEqBitIdx<ArgT>(exp, code, ctx, dst, evalT, post, arg,
            IR::Arg_Lit(arg.type->getSizeBytes(), arg.data->getIRExp()));

         return;
      }

      // As a fallback, just evaluate the pointer and store in a temporary.
      {
         // Evaluate arg's data.
         arg.data->genStmntStk(ctx);

         // Move to temporary.
         Temporary tmp{ctx, exp->pos, arg.data->getType()->getSizeWords()};
         ctx.block.addStmnt(IR::CodeBase::Move, tmp.getArg(), tmp.getArgStk());

         // Use temporary as index.
         GenStmnt_ArithEqBitIdx<ArgT>(exp, code, ctx, dst, evalT, post, arg, tmp.getArg());

         return;
      }
   }

   //
   // GenStmnt_ArithEqBitT<IR::Arg_Cpy>
   //
   template<>
   void GenStmnt_ArithEqBitT<IR::Arg_Cpy>(Exp_Binary const *exp, IR::Code,
      GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Cpy op=");
   }

   //
   // GenStmnt_ArithEqBitT<IR::Arg_Lit>
   //
   template<>
   void GenStmnt_ArithEqBitT<IR::Arg_Lit>(Exp_Binary const *exp, IR::Code,
      GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Lit op=");
   }

   //
   // GenStmnt_ArithEqBitT<IR::Arg_Nul>
   //
   template<>
   void GenStmnt_ArithEqBitT<IR::Arg_Nul>(Exp_Binary const *exp, IR::Code,
      GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Nul op=");
   }

   //
   // GenStmnt_ArithEqBitT<IR::Arg_Stk>
   //
   template<>
   void GenStmnt_ArithEqBitT<IR::Arg_Stk>(Exp_Binary const *exp, IR::Code,
      GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Stk op=");
   }

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
      IR::Arg_Stk stkR = exp->expR->getIRArgStk();

      // Duplicate to destination, if necessary.
      if(post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
      {
         GenStmnt_MoveDstPre(exp, ctx, dst);

         // Push l.
         ctx.block.addStmnt(IR::CodeBase::Move,
            dst.getIRArgStk(), GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0));

         // Assign dst.
         GenStmnt_MoveDstSuf(exp, ctx, dst);
      }

      // Push l.
      ctx.block.addStmnt(IR::CodeBase::Move,
         arg.getIRArgStk(), GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0));

      // Convert to evaluation type.
      GenStmnt_ConvertArith(exp, evalT, exp->type, ctx);

      // Attempt to turn r into an IR Arg.
      ctx.block.setArgSize(evalT->getSizeBytes());
      if(exp->expR->getArg().isIRArg())
      {
         ctx.block.addStmnt(code, stkR, stkR, exp->expR->getArg().getIRArg(ctx.prog));
      }

      // Otherwise, just operate on stack.
      else
      {
         // Push r.
         exp->expR->genStmntStk(ctx);

         // Operate on stack.
         ctx.block.addStmnt(code, stkR, stkR, stkR);
      }

      // Convert to result type.
      GenStmnt_ConvertArith(exp, exp->type, evalT, ctx);

      // Assign l.
      ctx.block.addStmnt(IR::CodeBase::Move,
         GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0), arg.getIRArgStk());

      // Duplicate to destination, if necessary.
      if(!post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
      {
         GenStmnt_MoveDstPre(exp, ctx, dst);

         // Push l.
         ctx.block.addStmnt(IR::CodeBase::Move,
            dst.getIRArgStk(), GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0));

         // Assign dst.
         GenStmnt_MoveDstSuf(exp, ctx, dst);
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
         ctx.block.addStmnt(IR::CodeBase::Move, tmp.getArg(), tmp.getArgStk());

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
      Core::Error(exp->pos, "AddrBase::Cpy op=");
   }

   //
   // GenStmnt_ArithEqT<IR::Arg_Lit>
   //
   template<>
   void GenStmnt_ArithEqT<IR::Arg_Lit>(Exp_Binary const *exp, IR::Code,
      GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Lit op=");
   }

   //
   // GenStmnt_ArithEqT<IR::Arg_Nul>
   //
   template<>
   void GenStmnt_ArithEqT<IR::Arg_Nul>(Exp_Binary const *exp, IR::Code,
      GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
   {
      Core::Error(exp->pos, "AddrBase::Nul op=");
   }

   //
   // GenStmnt_ArithEqT<IR::Arg_Stk>
   //
   template<>
   void GenStmnt_ArithEqT<IR::Arg_Stk>(Exp_Binary const *exp, IR::Code,
      GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
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
   // GenStmnt_ArithEq
   //
   void GenStmnt_ArithEq(Exp_Binary const *exp, IR::Code code,
      GenStmntCtx const &ctx, Arg const &dst, Type const *evalT, bool post)
   {
      auto arg = exp->expL->getArgDup();

      // Map from generic address space for codegen.
      if(arg.type->getQualAddr().base == IR::AddrBase::Gen)
         arg.type = arg.type->getTypeQual(Target::GetAddrGen());

      // Bitfield op?
      if(exp->expL->getType()->isTypeBitfield())
      {
         switch(arg.type->getQualAddr().base)
         {
            #define GDCC_Target_AddrList(addr) \
            case IR::AddrBase::addr: \
               GenStmnt_ArithEqBitT<IR::Arg_##addr>( \
                  exp, code, ctx, dst, evalT, post, arg); \
               break;
            #include "Target/AddrList.hpp"
         }

         return;
      }

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
            GenStmnt_MoveDstPre(exp, ctx, dst);
            ctx.block.addStmnt(IR::CodeBase::Move, dst.getIRArgStk(), irArgL);
            GenStmnt_MoveDstSuf(exp, ctx, dst);
         }

         ctx.block.addStmnt(code, irArgL, irArgL, irArgR);

         // Duplicate to destination, if necessary.
         if(!post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
         {
            GenStmnt_MoveDstPre(exp, ctx, dst);
            ctx.block.addStmnt(IR::CodeBase::Move, dst.getIRArgStk(), irArgL);
            GenStmnt_MoveDstSuf(exp, ctx, dst);
         }

         return;
      }

      switch(arg.type->getQualAddr().base)
      {
         #define GDCC_Target_AddrList(addr) \
         case IR::AddrBase::addr: \
            GenStmnt_ArithEqT<IR::Arg_##addr>( \
               exp, code, ctx, dst, evalT, post, arg); \
            break;
         #include "Target/AddrList.hpp"
      }
   }
}

// EOF

