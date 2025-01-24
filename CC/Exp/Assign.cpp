//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator =" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Assign.hpp"

#include "CC/Exp/Mem.hpp"
#include "CC/Factory.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"
#include "IR/Exp.hpp"

#include "SR/Arg.hpp"
#include "SR/GenStmnt/Move.hpp"
#include "SR/Temporary.hpp"
#include "SR/Type.hpp"

#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // GenStmnt_SetBitsPartIdx
   //
   template<typename ArgT, typename IdxT>
   static void GenStmnt_SetBitsPartIdx(SR::Exp const *exp,
      SR::GenStmntCtx const &ctx, SR::Arg const &arg, IdxT &&idx,
      Core::FastU bits, Core::FastU offs)
   {
      ctx.block.addStmnt(IR::CodeBase::Bset,
         SR::GenStmnt_Move_GenArg<ArgT>(exp, ctx, arg, std::forward<IdxT>(idx), 0),
         IR::Arg_Stk(arg.type->getSizeBytes()), bits, offs);
   }

   //
   // GenStmnt_SetBitsPartT
   //
   template<typename ArgT>
   static void GenStmnt_SetBitsPartT(SR::Exp const *exp,
      SR::GenStmntCtx const &ctx, SR::Arg const &arg, Core::FastU bits,
      Core::FastU offs)
   {
      // If arg address is a constant, then use Arg_Lit address.
      if(arg.data->isIRExp())
      {
         // Evaluate arg's data for side effects.
         arg.data->genStmnt(ctx);

         // Use literal as index.
         GenStmnt_SetBitsPartIdx<ArgT>(exp, ctx, arg,
            IR::Arg_Lit(Target::GetWordBytes(), arg.data->getIRExp()), bits, offs);

         return;
      }

      // If arg address is an IR arg, use it.
      // Note that isIRArg implies a lack of side effects.
      if(arg.data->getArgSrc().isIRArg())
      {
         GenStmnt_SetBitsPartIdx<ArgT>(exp, ctx, arg,
            arg.data->getArgSrc().getIRArg(ctx.prog), bits, offs);

         return;
      }

      // As a last resort, evaluate the pointer and store in a temporary.
      {
         // Evaluate arg's data.
         arg.data->genStmntStk(ctx);

         // Move to temporary.
         SR::Temporary tmp{ctx, exp->pos, arg.data->getType()->getSizeWords()};
         ctx.block.addStmnt(IR::CodeBase::Move, tmp.getArg(), tmp.getArgStk());

         // Use temporary as index.
         GenStmnt_SetBitsPartIdx<ArgT>(exp, ctx, arg, tmp.getArg(), bits, offs);

         return;
      }
   }

   //
   // GenStmnt_SetBitsPartT<IR::Arg_Cpy>
   //
   template<> void GenStmnt_SetBitsPartT<IR::Arg_Cpy>(SR::Exp const *exp,
      SR::GenStmntCtx const &, SR::Arg const &, Core::FastU, Core::FastU)
   {
      Core::Error(exp->pos, "AddrBase::Cpy set");
   }

   //
   // GenStmnt_SetBitsPartT<IR::Arg_Lit>
   //
   template<> void GenStmnt_SetBitsPartT<IR::Arg_Lit>(SR::Exp const *exp,
      SR::GenStmntCtx const &, SR::Arg const &, Core::FastU, Core::FastU)
   {
      Core::Error(exp->pos, "AddrBase::Lit set");
   }

   //
   // GenStmnt_SetBitsPartT<IR::Arg_Nul>
   //
   template<> void GenStmnt_SetBitsPartT<IR::Arg_Nul>(SR::Exp const *,
      SR::GenStmntCtx const &ctx, SR::Arg const &arg, Core::FastU, Core::FastU)
   {
      ctx.block.setArgSize(arg.type->getSizeBytes()).addStmnt(
         IR::CodeBase::Move, IR::Block::Nul(), IR::Block::Stk());
   }

   //
   // GenStmnt_SetBitsPartT<IR::Arg_Stk>
   //
   template<> void GenStmnt_SetBitsPartT<IR::Arg_Stk>(SR::Exp const *exp,
      SR::GenStmntCtx const &, SR::Arg const &, Core::FastU, Core::FastU)
   {
      Core::Error(exp->pos, "AddrBase::Stk set");
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_Assign constructor
   //
   Exp_Assign::Exp_Assign(SR::Exp const *l, SR::Exp const *r, Core::Origin pos_) :
      Super{l->getType(), l, r, pos_}
   {
   }

   //
   // Exp_Assign::v_genStmnt
   //
   void Exp_Assign::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      auto typeL = expL->getType();

      if(typeL->isTypeBitfield())
      {
         auto bits = typeL->getSizeBitsF() + typeL->getSizeBitsI() + typeL->getSizeBitsS();
         auto offs = typeL->getSizeBitsO();

         if(dst.type->getQualAddr().base != IR::AddrBase::Nul)
            GenStmnt_MoveDstPre(this, ctx, dst);

         // Evaluate source to stack.
         expR->genStmntStk(ctx);

         // If duplicating the result, just duplicate it on the stack.
         if(dst.type->getQualAddr().base != IR::AddrBase::Nul)
            ctx.block.setArgSize(expR->getType()->getSizeBytes()).addStmnt(
               IR::CodeBase::Copy, IR::Block::Stk(), IR::Block::Stk());

         auto argL = expL->getArg();

         // Map from generic address space for codegen.
         if(argL.type->getQualAddr().base == IR::AddrBase::Gen)
            argL.type = argL.type->getTypeQual(Target::GetAddrGen());

         switch(argL.type->getQualAddr().base)
         {
            #define GDCC_Target_AddrList(addr) \
            case IR::AddrBase::addr: \
               GenStmnt_SetBitsPartT<IR::Arg_##addr>(this, ctx, argL, bits, offs); \
               break;
            #include "Target/AddrList.hpp"
         }

         // Move duplicated result, if needed.
         if(dst.type->getQualAddr().base != IR::AddrBase::Nul)
            GenStmnt_MoveDstSuf(this, ctx, dst);
      }
      else
      {
         if(dst.type->getQualAddr().base == IR::AddrBase::Nul)
            expR->genStmnt(ctx, expL->getArgDst());
         else
            GenStmnt_Move(this, ctx, dst, expL->getArgDup(), expR->getArgSrc());
      }
   }

   //
   // Exp_Assign::v_isEffect
   //
   bool Exp_Assign::v_isEffect() const
   {
      // Assignment is the most basic form of side-effect.
      return true;
   }

   //
   // Exp_Assign:v_getIRExp
   //
   IR::Exp::CRef Exp_Assign::v_getIRExp() const
   {
      return expR->getIRExp();
   }

   //
   // Exp_Assign::v_isIRExp
   //
   bool Exp_Assign::v_isIRExp() const
   {
      return expR->isIRExp();
   }

   //
   // Factory::expCreate_Assign
   //
   SR::Exp::CRef Factory::expCreate_Assign(SR::Exp const *l, SR::Exp const *r,
      Core::Origin pos)
   {
      // Special check for structure property.
      if(auto expL = dynamic_cast<Exp_MemProp const *>(l))
         return expL->createExp_Equal(SR::Exp::CRef{r});

      if(!IsModLValue(l))
         Core::Error(l->pos, "expected modifiable lvalue");

      auto type = l->getType();
      if(type->isTypeBitfield())
         type = type->getBaseType();

      auto expR = expPromo_Assign(type, r, pos);

      return Exp_Assign::Create(l, expR, pos);
   }
}

// EOF

