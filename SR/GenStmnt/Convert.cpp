//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Type-conversion codegen.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/Convert.hpp"

#include "SR/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"

#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::SR
{
   //
   // ExtendSign
   //
   static void ExtendSign(Exp const *, GenStmntCtx const &ctx, Core::FastI diffWords)
   {
      // Duplicate leading word and signed shift.
      ctx.block.addStmnt(IR::CodeBase::Copy, IR::Block::Stk(), IR::Block::Stk());
      ctx.block.addStmnt(IR::CodeBase::ShR+'I',
         IR::Block::Stk(), IR::Block::Stk(), Target::GetWordBits() - 1);

      // Duplicate that result for any additional words.
      for(auto i = diffWords; --i;)
         ctx.block.addStmnt(IR::CodeBase::Copy, IR::Block::Stk(), IR::Block::Stk());
   }

   //
   // ExtendZero
   //
   static void ExtendZero(Exp const *, GenStmntCtx const &ctx, Core::FastI diffWords)
   {
      for(auto i = diffWords; i--;)
         ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), 0);
   }

   //
   // GetMaskSig
   //
   static Core::FastU GetMaskSig(Type const *t)
   {
      Core::FastU bitsF = t->getSizeBitsF();
      Core::FastU bitsI = t->getSizeBitsI();

      return Core::FastU(1) << ((bitsI + bitsF) % Target::GetWordBits());
   }

   //
   // MaskSubWord
   //
   static void MaskSubWord(Exp const *, GenStmntCtx const &ctx, SR::Type const *type)
   {
      auto bits = type->getSizeBitsF() + type->getSizeBitsI();

      // If signed, also do sign extension.
      if(type->getSizeBitsS())
      {
         auto shift = Target::GetWordBits() - bits - 1;

         ctx.block.addStmnt(IR::CodeBase::ShL+'I',
            IR::Block::Stk(), IR::Block::Stk(), shift);
         ctx.block.addStmnt(IR::CodeBase::ShR+'I',
            IR::Block::Stk(), IR::Block::Stk(), shift);
      }
      else
      {
         auto mask = (Core::FastU{1} << bits) - 1;

         ctx.block.addStmnt(IR::CodeBase::BAnd,
            IR::Block::Stk(), IR::Block::Stk(), mask);
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // GenStmnt_ConvertArith
   //
   void GenStmnt_ConvertArith(Exp const *exp, Type const *dstT,
      Type const *srcT, GenStmntCtx const &ctx)
   {
      if(dstT == srcT)
         return;

      // Boolean conversion.
      if(dstT->isTypeBoolean())
         return GenStmnt_ConvertBool(exp, dstT, srcT, ctx);

      // Fixed-Fixed conversion.
      // TODO: Use Tr when it has good codegen for this.
      if((srcT->isCTypeFixed() || srcT->isCTypeInteg()) &&
         (dstT->isCTypeFixed() || dstT->isCTypeInteg()))
         return GenStmnt_ConvertFixed(exp, dstT, srcT, ctx);

      // Use Tr.
      char t[2];

      if(dstT->isCTypeRealFlt())
         t[0] = dstT->getSizeBitsS() ? 'F' : 'E';
      else if(dstT->isCTypeAccum())
         t[0] = dstT->getSizeBitsS() ? 'X' : 'K';
      else if(srcT->isCTypeFract())
         t[0] = dstT->getSizeBitsS() ? 'R' : 'A';
      else if(dstT->isCTypeInteg())
         t[0] = dstT->getSizeBitsS() ? 'I' : 'U';
      else
         Core::Error(exp->pos, "convert arith dst stub");

      if(srcT->isCTypeRealFlt())
         t[1] = srcT->getSizeBitsS() ? 'F' : 'E';
      else if(srcT->isCTypeAccum())
         t[1] = srcT->getSizeBitsS() ? 'X' : 'K';
      else if(srcT->isCTypeFract())
         t[1] = srcT->getSizeBitsS() ? 'R' : 'A';
      else if(srcT->isCTypeInteg())
         t[1] = srcT->getSizeBitsS() ? 'I' : 'U';
      else
         Core::Error(exp->pos, "convert arith src stub");

      ctx.block.setOrigin(exp->pos).addStmnt(IR::CodeBase::Tr+t,
         IR::Arg_Stk(dstT->getSizeBytes()), IR::Arg_Stk(srcT->getSizeBytes()));
   }

   //
   // GenStmnt_ConvertBool
   //
   void GenStmnt_ConvertBool(Exp const *, Type const *,
      Type const *srcT, GenStmntCtx const &ctx)
   {
      ctx.block.setArgSize();

      if(srcT->isCTypeRealFlt())
         ctx.block.addStmnt(IR::CodeBase::BAnd,
            IR::Block::Stk(), IR::Block::Stk(), ~GetMaskSig(srcT));

      ctx.block.addStmnt(IR::CodeBase::LNot,
         IR::Block::Stk(), IR::Arg_Stk(srcT->getSizeBytes()));
      ctx.block.addStmnt(IR::CodeBase::LNot, IR::Block::Stk(), IR::Block::Stk());
   }

   //
   // GenStmnt_ConvertFixed
   //
   void GenStmnt_ConvertFixed(Exp const *exp, Type const *dstT,
      Type const *srcT, GenStmntCtx const &ctx)
   {
      auto diffWords = static_cast<Core::FastI>(dstT->getSizeWords()) -
                       static_cast<Core::FastI>(srcT->getSizeWords());

      auto diffBitsF = static_cast<Core::FastI>(dstT->getSizeBitsF()) -
                       static_cast<Core::FastI>(srcT->getSizeBitsF());

      ctx.block.setArgSize();

      // If source is signed subword, do subword sign extension.
      if(srcT->isTypeSubWord() && srcT->getSizeBitsS())
         MaskSubWord(exp, ctx, srcT);

      // Expand value.
      if(diffWords > 0)
         (srcT->getSizeBitsS() ? ExtendSign : ExtendZero)(exp, ctx, diffWords);

      // Shift value.
      if(diffBitsF)
      {
         IR::CodeBase shiftCode = diffBitsF > 0 ? IR::CodeBase::ShL : IR::CodeBase::ShR;
         Core::FastU  shiftSize = std::max(srcT->getSizeBytes(), dstT->getSizeBytes());

         ctx.block.addStmnt(shiftCode + (srcT->getSizeBitsS() ? 'I' : 'U'),
            IR::Arg_Stk(shiftSize), IR::Arg_Stk(shiftSize), std::abs(diffBitsF));
      }

      // Shrink value.
      if(diffWords < 0)
         ctx.block.addStmnt(IR::CodeBase::Move,
            IR::Arg_Nul(-diffWords), IR::Arg_Stk(-diffWords));

      // If destination is subword, mask.
      if(dstT->isTypeSubWord())
         MaskSubWord(exp, ctx, srcT);
   }
}

// EOF

