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

//
// GenStmnt_PointIdx
//
static void GenStmnt_PointIdx(GDCC::AST::Exp const *idx,
   GDCC::Core::FastU point, GDCC::AST::GenStmntCtx const &ctx)
{
   using namespace GDCC;

   if(point > 1)
   {
      if(idx->isIRExp())
      {
         auto pointV = IR::Value_Fixed(point, idx->getType()->getIRType().tFixed);
         auto pointE = IR::ExpCreate_Value(std::move(pointV), idx->pos);

         ctx.block.addStatementArgs(IR::Code::Move_W, IR::Arg_Stk(),
            IR::ExpCreate_Mul(idx->getIRExp(), pointE, idx->pos));
      }
      else
      {
         idx->genStmntStk(ctx);
         ctx.block.addStatementArgs(IR::Code::Move_W, IR::Arg_Stk(), point);
         ctx.block.addStatementArgs(IR::Code::MulU_W,
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
static void GenStmnt_PointEqIdx(GDCC::AST::Exp_Binary const *exp,
   GDCC::IR::Code code, GDCC::AST::GenStmntCtx const &ctx,
   GDCC::AST::Arg const &dst, bool post, GDCC::AST::Arg const &arg,
   IdxT const &idx)
{
   using namespace GDCC;

   // Duplicate to destination, if necessary.
   if(post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
   {
      // Push l.
      for(Core::FastU n = 0, e = arg.type->getSizeWords(); n != e; ++n)
         AST::GenStmnt_MoveWordGetT<ArgT>(exp, ctx, arg, idx, n);

      // Assign dst.
      GenStmnt_MovePart(exp, ctx, dst, false, true);
   }

   // Push l.
   for(Core::FastU n = 0, e = arg.type->getSizeWords(); n != e; ++n)
      AST::GenStmnt_MoveWordGetT<ArgT>(exp, ctx, arg, idx, n);

   // Push r.
   GenStmnt_PointIdx(exp->expR, exp->type->getBaseType()->getSizePoint(), ctx);

   // Operate on stack.
   ctx.block.addStatementArgs(code,
      IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

   // Assign l.
   for(Core::FastU n = arg.type->getSizeWords(); n--;)
      AST::GenStmnt_MoveWordSetT<ArgT>(exp, ctx, arg, idx, n);

   // Duplicate to destination, if necessary.
   if(!post && dst.type->getQualAddr().base != IR::AddrBase::Nul)
   {
      // Push l.
      for(Core::FastU n = 0, e = arg.type->getSizeWords(); n != e; ++n)
         AST::GenStmnt_MoveWordGetT<ArgT>(exp, ctx, arg, idx, n);

      // Assign dst.
      GenStmnt_MovePart(exp, ctx, dst, false, true);
   }
}

//
// GenStmnt_PointEqT
//
template<typename ArgT>
static void GenStmnt_PointEqT(GDCC::AST::Exp_Binary const *exp,
   GDCC::IR::Code code, GDCC::AST::GenStmntCtx const &ctx,
   GDCC::AST::Arg const &dst, bool post, GDCC::AST::Arg const &arg)
{
   using namespace GDCC;

   // If arg address is a constant, then use Arg_Lit address.
   if(arg.data->isIRExp())
   {
      // Evaluate arg's data for side effects.
      arg.data->genStmnt(ctx);

      // Use literal as index.
      GenStmnt_PointEqIdx<ArgT>(exp, code, ctx, dst, post, arg,
         IR::Arg_Lit(arg.data->getIRExp()));

      return;
   }

   // As a fallback, just evaluate the pointer and store in a temporary.
   {
      // Evaluate arg's data.
      arg.data->genStmntStk(ctx);

      // Move to temporary.
      AST::Temporary tmp{ctx, exp->pos, arg.data->getType()->getSizeWords()};
      for(Core::FastU n = tmp.size(); n--;)
         ctx.block.addStatementArgs(IR::Code::Move_W,
            tmp.getArg(n), IR::Arg_Stk());

      // Use temporary as index.
      GenStmnt_PointEqIdx<ArgT>(exp, code, ctx, dst, post, arg, tmp.getArg());

      return;
   }
}

//
// GenStmnt_PointEqT<IR::Arg_Cpy>
//
template<> void GenStmnt_PointEqT<GDCC::IR::Arg_Cpy>(
   GDCC::AST::Exp_Binary const *exp, GDCC::IR::Code,
   GDCC::AST::GenStmntCtx const &, GDCC::AST::Arg const &, bool,
   GDCC::AST::Arg const &)
{
   throw GDCC::Core::ExceptStr(exp->pos, "AddrBase::Cpy op=");
}

//
// GenStmnt_PointEqT<IR::Arg_Lit>
//
template<> void GenStmnt_PointEqT<GDCC::IR::Arg_Lit>(
   GDCC::AST::Exp_Binary const *exp, GDCC::IR::Code,
   GDCC::AST::GenStmntCtx const &, GDCC::AST::Arg const &, bool,
   GDCC::AST::Arg const &)
{
   throw GDCC::Core::ExceptStr(exp->pos, "AddrBase::Lit op=");
}

//
// GenStmnt_PointEqT<IR::Arg_Nul>
//
template<> void GenStmnt_PointEqT<GDCC::IR::Arg_Nul>(
   GDCC::AST::Exp_Binary const *exp, GDCC::IR::Code,
   GDCC::AST::GenStmntCtx const &, GDCC::AST::Arg const &, bool,
   GDCC::AST::Arg const &)
{
   throw GDCC::Core::ExceptStr(exp->pos, "AddrBase::Nul op=");
}

//
// GenStmnt_PointEqT<IR::Arg_Stk>
//
template<> void GenStmnt_PointEqT<GDCC::IR::Arg_Stk>(
   GDCC::AST::Exp_Binary const *exp, GDCC::IR::Code,
   GDCC::AST::GenStmntCtx const &, GDCC::AST::Arg const &, bool,
   GDCC::AST::Arg const &)
{
   throw GDCC::Core::ExceptStr(exp->pos, "AddrBase::Stk op=");
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
      void GenStmnt_Point(Exp_Binary const *exp, IR::Code code,
         GenStmntCtx const &ctx, Arg const &dst)
      {
         if(GenStmntNul(exp, ctx, dst)) return;

         // Evaluate pointer to stack.
         exp->expL->genStmntStk(ctx);

         // Evaluate index, adjusting if necessary.
         GenStmnt_PointIdx(exp->expR, exp->type->getBaseType()->getSizePoint(), ctx);

         // Add on stack.
         ctx.block.addStatementArgs(code,
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

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
}

// EOF
