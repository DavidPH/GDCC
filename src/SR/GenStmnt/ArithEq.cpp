//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
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

namespace GDCC
{
   namespace SR
   {
      //
      // GenStmnt_ArithEqIdx
      //
      // Does general statemenet generation with an IR arg for l.
      //
      template<typename ArgT, typename IdxT>
      static void GenStmnt_ArithEqIdx(Exp_Binary const *exp, IR::OpCode op,
         GenStmntCtx const &ctx, Arg const &dst, Type const *evalT, bool post,
         Arg const &arg, IdxT const &idx)
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

         // Convert to evaluation type.
         GenStmnt_ConvertArith(exp, evalT, exp->type, ctx);

         // Attempt to turn r into an IR Arg.
         if(exp->expR->getArg().isIRArg())
         {
            ctx.block.addStatementArgs(op,
               IR::Arg_Stk(), IR::Arg_Stk(),
               exp->expR->getArg().getIRArg(ctx.prog));
         }

         // Otherwise, just operate on stack.
         else
         {
            // Push r.
            exp->expR->genStmntStk(ctx);

            // Operate on stack.
            ctx.block.addStatementArgs(op,
               IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());
         }

         // Convert to result type.
         GenStmnt_ConvertArith(exp, exp->type, evalT, ctx);

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
      // GenStmnt_ArithEqT
      //
      template<typename ArgT>
      static void GenStmnt_ArithEqT(Exp_Binary const *exp, IR::OpCode op,
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
            GenStmnt_ArithEqIdx<ArgT>(exp, op, ctx, dst, evalT, post, arg,
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
            GenStmnt_ArithEqIdx<ArgT>(exp, op, ctx, dst, evalT, post, arg,
               tmp.getArg());

            return;
         }
      }

      //
      // GenStmnt_ArithEqT<IR::Arg_Cpy>
      //
      template<>
      void GenStmnt_ArithEqT<IR::Arg_Cpy>(Exp_Binary const *exp, IR::OpCode,
         GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
      {
         throw Core::ExceptStr(exp->pos, "AddrBase::Cpy op=");
      }

      //
      // GenStmnt_ArithEqT<IR::Arg_Lit>
      //
      template<>
      void GenStmnt_ArithEqT<IR::Arg_Lit>(Exp_Binary const *exp, IR::OpCode,
         GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
      {
         throw Core::ExceptStr(exp->pos, "AddrBase::Lit op=");
      }

      //
      // GenStmnt_ArithEqT<IR::Arg_Nul>
      //
      template<>
      void GenStmnt_ArithEqT<IR::Arg_Nul>(Exp_Binary const *exp, IR::OpCode,
         GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
      {
         throw Core::ExceptStr(exp->pos, "AddrBase::Nul op=");
      }

      //
      // GenStmnt_ArithEqT<IR::Arg_Stk>
      //
      template<>
      void GenStmnt_ArithEqT<IR::Arg_Stk>(Exp_Binary const *exp, IR::OpCode,
         GenStmntCtx const &, Arg const &, Type const *, bool, Arg const &)
      {
         throw Core::ExceptStr(exp->pos, "AddrBase::Stk op=");
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
      // GenStmnt_ArithEq
      //
      void GenStmnt_ArithEq(Exp_Binary const *exp, IR::OpCode op,
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

            IR::OpCode opMove = {IR::Code::Move_W, argL.type->getSizeWords()};

            // Duplicate to destination, if necessary.
            if(post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
            {
               ctx.block.addStatementArgs(opMove, IR::Arg_Stk(), irArgL);
               GenStmnt_MovePart(exp, ctx, dst, false, true);
            }

            ctx.block.addStatementArgs(op, irArgL, irArgL, irArgR);

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
               GenStmnt_ArithEqT<IR::Arg_##addr>( \
                  exp, op, ctx, dst, evalT, post, arg); \
               break;
            #include "IR/AddrList.hpp"
         }
      }
   }
}

// EOF

