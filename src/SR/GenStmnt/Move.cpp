//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Memory movement statement generation.
//
//-----------------------------------------------------------------------------

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
      // GenStmnt_MovePartIdx
      //
      template<typename ArgT, typename IdxT>
      static void GenStmnt_MovePartIdx(Exp const *exp, GenStmntCtx const &ctx,
         Arg const &arg, IdxT const &idx, bool get, bool set)
      {
         IR::OpCode op;
         if(arg.type->isTypeSubWord())
            op = {IR::Code::Move_B, arg.type->getSizeBytes()};
         else
            op = {IR::Code::Move_W, arg.type->getSizeWords()};

         if(set)
            ctx.block.addStatementArgs(op,
               GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0), IR::Arg_Stk());

         if(get)
            ctx.block.addStatementArgs(op,
               IR::Arg_Stk(), GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, idx, 0));
      }

      //
      // GenStmnt_MovePartT
      //
      template<typename ArgT>
      static void GenStmnt_MovePartT(Exp const *exp, GenStmntCtx const &ctx,
         Arg const &arg, bool get, bool set)
      {
         // If arg address is a constant, then use Arg_Lit address.
         if(arg.data->isIRExp())
         {
            // Evaluate arg's data for side effects.
            arg.data->genStmnt(ctx);

            // Use literal as index.
            GenStmnt_MovePartIdx<ArgT>(exp, ctx, arg,
               IR::Arg_Lit(arg.data->getIRExp()), get, set);

            return;
         }

         // If arg address is an IR arg, use it.
         // Note that isIRArg implies a lack of side effects.
         if(arg.data->getArgSrc().isIRArg())
         {
            GenStmnt_MovePartIdx<ArgT>(exp, ctx, arg,
               arg.data->getArgSrc().getIRArg(ctx.prog), get, set);

            return;
         }

         // If fetching or setting a single word, use address on stack.
         if(arg.type->getSizeWords() == 1 && get ^ set)
         {
            // Evaluate arg's data.
            arg.data->genStmntStk(ctx);

            // Use Stk as index.
            GenStmnt_MovePartIdx<ArgT>(exp, ctx, arg, IR::Arg_Stk(), get, set);

            return;
         }

         // As a last resort, evaluate the pointer and store in a temporary.
         {
            // Evaluate arg's data.
            arg.data->genStmntStk(ctx);

            // Move to temporary.
            Temporary tmp{ctx, exp->pos, arg.data->getType()->getSizeWords()};
            ctx.block.addStatementArgs({IR::Code::Move_W, tmp.size()},
               tmp.getArg(), IR::Arg_Stk());

            // Use temporary as index.
            GenStmnt_MovePartIdx<ArgT>(exp, ctx, arg, tmp.getArg(), get, set);

            return;
         }
      }

      //
      // GenStmnt_MovePartT<IR::Arg_Cpy>
      //
      template<> void GenStmnt_MovePartT<IR::Arg_Cpy>(Exp const *exp,
         GenStmntCtx const &, Arg const &, bool get, bool set)
      {
         if(set) throw Core::ExceptStr(exp->pos, "AddrBase::Cpy set");
         if(get) throw Core::ExceptStr(exp->pos, "AddrBase::Cpy get");
      }

      //
      // GenStmnt_MovePartT<IR::Arg_Lit>
      //
      template<> void GenStmnt_MovePartT<IR::Arg_Lit>(Exp const *exp,
         GenStmntCtx const &ctx, Arg const &arg, bool get, bool set)
      {
         if(set) throw Core::ExceptStr(exp->pos, "AddrBase::Lit set");

         if(get)
         {
            if(arg.data->isIRExp())
            {
               arg.data->genStmnt(ctx);

               ctx.block.addStatementArgs(
                  {IR::Code::Move_W, arg.type->getSizeWords()},
                  IR::Arg_Stk(), arg.data->getIRExp());
            }
            else
               arg.data->genStmnt(ctx, Arg(arg.type, IR::AddrBase::Stk));
         }
      }

      //
      // GenStmnt_MovePartT<IR::Arg_Nul>
      //
      template<> void GenStmnt_MovePartT<IR::Arg_Nul>(Exp const *exp,
         GenStmntCtx const &ctx, Arg const &arg, bool get, bool set)
      {
         if(set)
         {
            ctx.block.addStatementArgs(
               {IR::Code::Move_W, arg.type->getSizeWords()},
               IR::Arg_Nul(), IR::Arg_Stk());
         }

         if(get) throw Core::ExceptStr(exp->pos, "AddrBase::Nul get");
      }

      //
      // GenStmnt_MovePartT<IR::Arg_Stk>
      //
      template<> void GenStmnt_MovePartT<IR::Arg_Stk>(
         Exp const *exp, GenStmntCtx const &,
         Arg const &, bool get, bool set)
      {
         if(get && set)
            throw Core::ExceptStr(exp->pos, "AddrBase::Stk get && set");
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
      // GenStmnt_Move
      //
      void GenStmnt_Move(Exp const *exp, GenStmntCtx const &ctx,
         Arg const &dst, Arg const &src)
      {
         // Try to use IR args.
         if(dst.isIRArg() && src.isIRArg())
         {
            if(src.type->isTypeSubWord())
               ctx.block.addStatementArgs(
                  {IR::Code::Move_B, src.type->getSizeBytes()},
                  dst.getIRArg(ctx.prog), src.getIRArg(ctx.prog));
            else
               ctx.block.addStatementArgs(
                  {IR::Code::Move_W, src.type->getSizeWords()},
                  dst.getIRArg(ctx.prog), src.getIRArg(ctx.prog));

            return;
         }

         // Fall back to dumping full objects to stack.
         GenStmnt_MovePart(exp, ctx, src, true, false);
         GenStmnt_MovePart(exp, ctx, dst, false, true);
      }

      //
      // GenStmnt_Move
      //
      void GenStmnt_Move(Exp const *exp, GenStmntCtx const &ctx,
         Arg const &dst, Arg const &dup, Arg const &src)
      {
         // Try to use IR args.
         if(dst.isIRArg() && dup.isIRArg() && src.isIRArg())
         {
            auto dupIR = dup.getIRArg(ctx.prog);

            if(src.type->isTypeSubWord())
            {
               ctx.block.addStatementArgs(
                  {IR::Code::Move_B, src.type->getSizeBytes()},
                  dupIR, src.getIRArg(ctx.prog));

               ctx.block.addStatementArgs(
                  {IR::Code::Move_B, src.type->getSizeBytes()},
                  dst.getIRArg(ctx.prog), dupIR);
            }
            else
            {
               ctx.block.addStatementArgs(
                  {IR::Code::Move_W, src.type->getSizeWords()},
                  dupIR, src.getIRArg(ctx.prog));

               ctx.block.addStatementArgs(
                  {IR::Code::Move_W, src.type->getSizeWords()},
                  dst.getIRArg(ctx.prog), dupIR);
            }

            return;
         }

         // Fall back to dumping full objects to stack.
         GenStmnt_MovePart(exp, ctx, src, true, false);
         GenStmnt_MovePart(exp, ctx, dup, true, true);
         GenStmnt_MovePart(exp, ctx, dst, false, true);
      }

      //
      // GenStmnt_MovePart
      //
      void GenStmnt_MovePart(Exp const *exp, GenStmntCtx const &ctx,
         Arg const &arg_, bool get, bool set)
      {
         auto arg = arg_;

         // Special handling of void arg.
         if(arg.type->isTypeVoid())
         {
            // A void src is an error.
            if(get) throw Core::ExceptStr(exp->pos, "void src");

            // A void dst is a no-op.
            if(set) return;
         }

         // Map from generic address space for codegen.
         if(arg.type->getQualAddr().base == IR::AddrBase::Gen)
            arg.type = arg.type->getTypeQual(IR::GetAddrGen());

         switch(arg.type->getQualAddr().base)
         {
            #define GDCC_IR_AddrList(addr) \
            case IR::AddrBase::addr: \
               GenStmnt_MovePartT<IR::Arg_##addr>(exp, ctx, arg, get, set); \
               break;
            #include "IR/AddrList.hpp"
         }
      }
   }
}

// EOF

