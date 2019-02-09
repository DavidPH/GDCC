//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Type-conversion codegen.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/Convert.hpp"

#include "SR/ExpCode.hpp"
#include "SR/Function.hpp"
#include "SR/Temporary.hpp"
#include "SR/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"
#include "IR/Glyph.hpp"

#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::SR
{
   static Core::FastU GetMaskExp(Type const *t);
   static Core::FastU GetMaskMan(Type const *t);

   //
   // CheckExp
   //
   static void CheckExp(Exp const *, GenStmntCtx const &ctx,
      Type const *srcT, Temporary &src, IR::Code codeCmp,
      Core::FastU exp, IR::Code codeJcnd, IR::Glyph label)
   {
      ctx.block.setArgSize();

      ctx.block.addStmnt(IR::CodeBase::BAnd,
         IR::Block::Stk(), src.getArg(src.size() - 1), GetMaskExp(srcT));

      ctx.block.addStmnt(codeCmp, IR::Block::Stk(), IR::Block::Stk(),
         exp << (srcT->getSizeBitsI() % Target::GetWordBits()));

      ctx.block.addStmnt(codeJcnd, IR::Block::Stk(), label);
   }

   //
   // DropLow
   //
   static void DropLow(Exp const *exp, GenStmntCtx const &ctx,
      Type const *dstT, Core::FastI diffWords)
   {
      Temporary tmpSwap{ctx, exp->pos, dstT->getSizeWords()};

      // Store desired words in a temporary...
      ctx.block.addStmnt(IR::CodeBase::Move,
         tmpSwap.getArg(), IR::Arg_Stk(dstT->getSizeBytes()));

      // ... drop unneeded words...
      ctx.block.addStmnt(IR::CodeBase::Move,
         IR::Arg_Nul(-diffWords), IR::Arg_Stk(-diffWords));

      // ... and reclaim desired words.
      ctx.block.addStmnt(IR::CodeBase::Move,
         IR::Arg_Stk(dstT->getSizeBytes()), tmpSwap.getArg());
   }

   //
   // ExtendSign
   //
   static void ExtendSign(Exp const *, GenStmntCtx const &ctx, Core::FastI diffWords)
   {
      ctx.block.setArgSize();

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
      ctx.block.setArgSize();

      for(auto i = diffWords; i--;)
         ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), 0);
   }

   //
   // FillLow
   //
   static void FillLow(Exp const *exp, GenStmntCtx const &ctx,
      Type const *srcT, Core::FastI diffWords)
   {
      Temporary tmp{ctx, exp->pos, srcT->getSizeWords()};

      // Store current words in a temporary...
      ctx.block.addStmnt(IR::CodeBase::Move,
         tmp.getArg(), IR::Arg_Stk(srcT->getSizeBytes()));

      // ... push needed words...
      ctx.block.setArgSize();
      for(auto i = diffWords; i--;)
         ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), 0);

      // ... and reclaim original words.
      ctx.block.addStmnt(IR::CodeBase::Move,
         IR::Arg_Stk(srcT->getSizeBytes()), tmp.getArg());
   }

   //
   // GetExp
   //
   static void GetExp(Exp const *, GenStmntCtx const &ctx,
      Type const *srcT, Temporary &src)
   {
      ctx.block.setArgSize();

      ctx.block.addStmnt(IR::CodeBase::BAnd,
         IR::Block::Stk(), src.getArg(src.size() - 1), GetMaskExp(srcT));

      ctx.block.addStmnt(IR::CodeBase::ShR+'I', IR::Block::Stk(),
         IR::Block::Stk(), srcT->getSizeBitsI() % Target::GetWordBits());
   }

   //
   // GetMan
   //
   static void GetMan(Exp const *, GenStmntCtx const &ctx,
      Type const *srcT, Temporary &src, bool full)
   {
      ctx.block.addStmnt(IR::CodeBase::Move, src.getArgStk(), src.getArg());

      ctx.block.setArgSize().addStmnt(IR::CodeBase::BAnd,
         IR::Block::Stk(), IR::Block::Stk(), GetMaskMan(srcT));

      if(full) ctx.block.addStmnt(IR::CodeBase::BOrI,
         IR::Block::Stk(), IR::Block::Stk(), GetMaskMan(srcT) + 1);
   }

   //
   // GetMaskExp
   //
   static Core::FastU GetMaskExp(Type const *t)
   {
      Core::FastU bitsF = t->getSizeBitsF();
      Core::FastU bitsI = t->getSizeBitsI() % Target::GetWordBits();

      return ((Core::FastU(1) << bitsF) - 1) << bitsI;
   }

   //
   // GetMaskMan
   //
   static Core::FastU GetMaskMan(Type const *t)
   {
      Core::FastU bitsI = t->getSizeBitsI() % Target::GetWordBits();

      return (Core::FastU(1) << bitsI) - 1;
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
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   void GenStmnt_ConvertBoolFix(Exp const *exp, Type const *dstT,
      Type const *srcT, GenStmntCtx const &ctx);
   void GenStmnt_ConvertBoolFlt(Exp const *exp, Type const *dstT,
      Type const *srcT, GenStmntCtx const &ctx);

   //
   // GenStmnt_ConvertArith
   //
   void GenStmnt_ConvertArith(Exp const *exp, Type const *dstT,
      Type const *srcT, GenStmntCtx const &ctx)
   {
      if(dstT == srcT)
         return;

      // Fixed-point source.
      if(srcT->isCTypeFixed() || srcT->isCTypeInteg())
      {
         // Boolean output.
         if(dstT->isTypeBoolean())
            return GenStmnt_ConvertBoolFix(exp, dstT, srcT, ctx);

         // Fixed-point output.
         if(dstT->isCTypeFixed() || dstT->isCTypeInteg())
            return GenStmnt_ConvertFixed(exp, dstT, srcT, ctx);

         // Floating-point output.
         if(dstT->isCTypeRealFlt())
            return GenStmnt_ConvertFltFix(exp, dstT, srcT, ctx);
      }

      // Floating-point source
      if(srcT->isCTypeRealFlt())
      {
         // Boolean output.
         if(dstT->isTypeBoolean())
            return GenStmnt_ConvertBoolFlt(exp, dstT, srcT, ctx);

         // Fixed-point output.
         if(dstT->isCTypeFixed() || dstT->isCTypeInteg())
            return GenStmnt_ConvertFixFlt(exp, dstT, srcT, ctx);

         // Floating-point output.
         if(dstT->isCTypeRealFlt())
            return GenStmnt_ConvertFloat(exp, dstT, srcT, ctx);
      }

      Core::Error(exp->pos, "convert arith stub");
   }

   //
   // GenStmnt_ConvertBoolFix
   //
   void GenStmnt_ConvertBoolFix(Exp const *, Type const *,
      Type const *srcT, GenStmntCtx const &ctx)
   {
      ctx.block.setArgSize();
      ctx.block.addStmnt(IR::CodeBase::LNot,
         IR::Block::Stk(), IR::Arg_Stk(srcT->getSizeBytes()));
      ctx.block.addStmnt(IR::CodeBase::LNot, IR::Block::Stk(), IR::Block::Stk());
   }

   //
   // GenStmnt_ConvertBoolFlt
   //
   void GenStmnt_ConvertBoolFlt(Exp const *, Type const *,
      Type const *srcT, GenStmntCtx const &ctx)
   {
      ctx.block.setArgSize();
      ctx.block.addStmnt(IR::CodeBase::BAnd,
         IR::Block::Stk(), IR::Block::Stk(), ~GetMaskSig(srcT));
      ctx.block.addStmnt(IR::CodeBase::LNot,
         IR::Block::Stk(), IR::Arg_Stk(srcT->getSizeBytes()));
      ctx.block.addStmnt(IR::CodeBase::LNot, IR::Block::Stk(), IR::Block::Stk());
   }

   //
   // GenStmnt_ConvertFixFlt
   //
   // Unfortunate assumptions:
   //    Everything from GenStmnt_ConvertFloat.
   //    The destination cannot represent all values of the source type.
   //       This would manifest as incorrect calculation of min and max
   //       exponent.
   //    Two's complement MIN for -INF.
   //
   // Values too large produce MAX/MIN for positive/negative values.
   // Values too small produce 0.
   // NaN produces MAX or MIN.
   //
   // Codegen for expMid > expMax is not optimal.
   //
   void GenStmnt_ConvertFixFlt(Exp const *exp, Type const *dstT,
      Type const *srcT, GenStmntCtx const &ctx)
   {
      auto wordBits  = Target::GetWordBits();
      auto diffWords = static_cast<Core::FastI>(dstT->getSizeWords()) -
                       static_cast<Core::FastI>(srcT->getSizeWords());

      auto expMax = srcT->getBias() + dstT->getSizeBitsI();
      auto expMid = srcT->getBias() + srcT->getSizeBitsI() - dstT->getSizeBitsF();
      auto expMin = srcT->getBias() - dstT->getSizeBitsF();

      IR::Glyph label0   = {ctx.prog, ctx.fn->genLabel()};
      IR::Glyph labelGT  = {ctx.prog, ctx.fn->genLabel()};
      IR::Glyph labelINF = {ctx.prog, ctx.fn->genLabel()};
      IR::Glyph labelLT  = {ctx.prog, ctx.fn->genLabel()};
      IR::Glyph labelMan = {ctx.prog, ctx.fn->genLabel()};
      IR::Glyph labelNI  = {ctx.prog, ctx.fn->genLabel()};
      IR::Glyph labelPos = {ctx.prog, ctx.fn->genLabel()};
      IR::Glyph labelSig = {ctx.prog, ctx.fn->genLabel()};

      // Store source in a temporary.
      Temporary src{ctx, exp->pos, srcT->getSizeWords()};
      ctx.block.addStmnt(IR::CodeBase::Move, src.getArg(), src.getArgStk());

      // Check if result is INF.
      CheckExp(exp, ctx, srcT, src, IR::CodeBase::CmpGE+'I', expMax,
         IR::CodeBase::Jcnd_Tru, labelINF);

      // Check if exponent more than mid.
      CheckExp(exp, ctx, srcT, src, IR::CodeBase::CmpGT+'I', expMid,
         IR::CodeBase::Jcnd_Tru, labelGT);

      // Check if exponent less than mid.
      CheckExp(exp, ctx, srcT, src, IR::CodeBase::CmpLT+'I', expMid,
         IR::CodeBase::Jcnd_Tru, labelLT);

      // Exponent is equal to mid.
      GetMan(exp, ctx, srcT, src, true);
      ctx.block.setArgSize().addStmnt(IR::CodeBase::Jump, labelMan);

      // Generate INF.
      ctx.block.addLabel(labelINF);
      if(dstT->getSizeBitsS())
      {
         ctx.block.setArgSize();

         ctx.block.addStmnt(IR::CodeBase::BAnd,
            IR::Block::Stk(), src.getArg(src.size() - 1), GetMaskSig(srcT));
         ctx.block.addStmnt(IR::CodeBase::Jcnd_Tru, IR::Block::Stk(), labelNI);

         // Positive infinity.
         for(auto i = dstT->getSizeWords(); --i;)
            ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), -1);
         ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(),
            (Core::FastU(1) << (dstT->getSizeBitsI() % wordBits)) - 1);

         ctx.block.addStmnt(IR::CodeBase::Jump, labelPos);

         // Negative infinity.
         ctx.block.addLabel(labelNI);

         for(auto i = dstT->getSizeWords(); --i;)
            ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), 0);
         ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(),
            Core::FastU(1) << (dstT->getSizeBitsI() % wordBits));

         ctx.block.addStmnt(IR::CodeBase::Jump, labelPos);
      }
      else
      {
         ctx.block.setArgSize();

         for(auto i = dstT->getSizeWords(); i--;)
            ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), -1);

         ctx.block.addStmnt(IR::CodeBase::Jump, labelPos);
      }

      // Generate 0.
      ctx.block.addLabel(label0);
      for(auto i = dstT->getSizeWords(); i--;)
         ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), 0);
      ctx.block.addStmnt(IR::CodeBase::Jump, labelPos);

      // Exponent is more than mid.
      ctx.block.addLabel(labelGT);

      // Shift mantissa left.
      GetMan(exp, ctx, srcT, src, true);

      if(diffWords > 0)
      {
         ctx.block.setArgSize();

         // If expanding value, do so before left shifting.
         for(auto i = diffWords; i--;)
            ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), 0);
      }

      GetExp(exp, ctx, srcT, src);
      ctx.block.setArgSize();
      ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), expMid);
      ctx.block.addStmnt(IR::CodeBase::Sub+'U',
         IR::Block::Stk(), IR::Block::Stk(), IR::Block::Stk());

      if(diffWords > 0)
      {
         ctx.block.addStmnt(IR::CodeBase::ShL+'U',
            IR::Arg_Stk(dstT->getSizeBytes()),
            IR::Arg_Stk(dstT->getSizeBytes()), IR::Block::Stk());

         ctx.block.addStmnt(IR::CodeBase::Jump, labelSig);
      }
      else
      {
         ctx.block.addStmnt(IR::CodeBase::ShL+'U',
            IR::Arg_Stk(srcT->getSizeBytes()),
            IR::Arg_Stk(srcT->getSizeBytes()), IR::Block::Stk());

         ctx.block.addStmnt(IR::CodeBase::Jump, labelMan);
      }

      // Exponent is less than mid.
      ctx.block.addLabel(labelLT);

      // Check if result is 0.
      CheckExp(exp, ctx, srcT, src, IR::CodeBase::CmpLT+'I', expMin,
         IR::CodeBase::Jcnd_Tru, label0);

      // Shift mantissa right.
      GetMan(exp, ctx, srcT, src, true);

      ctx.block.setArgSize().addStmnt(IR::CodeBase::Move, IR::Block::Stk(), expMid);
      GetExp(exp, ctx, srcT, src);
      ctx.block.setArgSize().addStmnt(IR::CodeBase::Sub+'U',
         IR::Block::Stk(), IR::Block::Stk(), IR::Block::Stk());

      ctx.block.addStmnt(IR::CodeBase::ShR+'U',
         IR::Arg_Stk(srcT->getSizeBytes()),
         IR::Arg_Stk(srcT->getSizeBytes()), IR::Block::Stk());

      // Shrink or expand value.
      ctx.block.addLabel(labelMan);
      if(diffWords > 0)
      {
         // Value is currently positive, so no sign extend.
         for(auto i = diffWords; i--;)
            ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), 0);
      }
      else if(diffWords < 0)
      {
         ctx.block.addStmnt(IR::CodeBase::Move,
            IR::Arg_Nul(-diffWords), IR::Arg_Stk(-diffWords));
      }

      // Convert sign.
      ctx.block.addLabel(labelSig);

      ctx.block.setArgSize();

      ctx.block.addStmnt(IR::CodeBase::BAnd,
         IR::Block::Stk(), src.getArg(src.size() - 1), GetMaskSig(srcT));
      ctx.block.addStmnt(IR::CodeBase::Jcnd_Nil,
         IR::Block::Stk(), labelPos);

      ctx.block.addStmnt(IR::CodeBase::Neg+'I',
         IR::Arg_Stk(dstT->getSizeBytes()), IR::Arg_Stk(dstT->getSizeBytes()));

      ctx.block.addLabel(labelPos);
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

      // If source is signed subword, do subword sign extension.
      if(srcT->isTypeSubWord() && srcT->getSizeBitsS())
      {
         auto srcBits = srcT->getSizeBitsF() + srcT->getSizeBitsI() + srcT->getSizeBitsS();
         auto shift   = Target::GetWordBits() - srcBits;

         ctx.block.setArgSize();
         ctx.block.addStmnt(IR::CodeBase::ShL+'U', IR::Block::Stk(), IR::Block::Stk(), shift);
         ctx.block.addStmnt(IR::CodeBase::ShR+'I', IR::Block::Stk(), IR::Block::Stk(), shift);
      }

      // Expand value.
      if(diffWords > 0)
      {
         // Signed source may require sign-extending.
         if(srcT->getSizeBitsS())
            ExtendSign(exp, ctx, diffWords);

         // Otherwise, expand with zeroes.
         else
            ExtendZero(exp, ctx, diffWords);
      }

      // Shift value.
      if(diffBitsF)
      {
         // Determine a shift type that has the larger size, but the
         // signedness of the source.
         Type::CPtr shiftType;
         if(diffWords > 0)
         {
            if(srcT->getSizeBitsS())
               shiftType = dstT->getSignType();
            else
               shiftType = dstT->getUnsiType();
         }
         else
            shiftType = srcT;

         // Determine shift code and create amount expression.
         IR::Code    shiftCode;
         Core::FastU shiftSize = shiftType->getSizeBytes();
         Core::FastU shiftVal;

         if(diffBitsF > 0)
         {
            shiftCode = ExpCode_ArithInteg(IR::CodeBase::ShL, shiftType);
            shiftVal  = +diffBitsF;
         }
         else
         {
            shiftCode = ExpCode_ArithInteg(IR::CodeBase::ShR, shiftType);
            shiftVal  = -diffBitsF;
         }

         if(shiftCode.base == IR::CodeBase::None)
            Core::Error(exp->pos, "no shift code for conversion");

         // Generate shift.
         ctx.block.setArgSize().addStmnt(shiftCode,
            IR::Arg_Stk(shiftSize), IR::Arg_Stk(shiftSize), shiftVal);
      }

      // Shrink value.
      if(diffWords < 0)
      {
         ctx.block.setArgSize();
         for(auto i = diffWords; i++;)
            ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Nul(), IR::Block::Stk());
      }

      // If destination is subword, mask.
      if(dstT->isTypeSubWord())
      {
         auto dstBits = dstT->getSizeBitsF() + dstT->getSizeBitsI() + dstT->getSizeBitsS();

         // If signed, also do sign extension.
         ctx.block.setArgSize();
         if(dstT->getSizeBitsS())
         {
            auto shift = Target::GetWordBits() - dstBits;

            ctx.block.addStmnt(IR::CodeBase::ShL+'U',
               IR::Block::Stk(), IR::Block::Stk(), shift);
            ctx.block.addStmnt(IR::CodeBase::ShR+'I',
               IR::Block::Stk(), IR::Block::Stk(), shift);
         }
         else
         {
            auto mask = (Core::FastU{1} << dstBits) - 1;

            ctx.block.addStmnt(IR::CodeBase::BAnd, IR::Block::Stk(), IR::Block::Stk(), mask);
         }
      }
   }

   //
   // GenStmnt_ConvertFloat
   //
   // Unfortunate assumptions:
   //    All floats are signed.
   //    Sign and exponent fit in highest word.
   //    NaNs have mantissa bits in the highest word set. (This assumption
   //       exists elsewhere as well and serves as an optimization.)
   //    Words are ordered little-endian, another widespread assumption.
   //
   void GenStmnt_ConvertFloat(Exp const *exp, Type const *dstT,
      Type const *srcT, GenStmntCtx const &ctx)
   {
      Core::FastU wordBits = Target::GetWordBits();

      auto diffWords = static_cast<Core::FastI>(dstT->getSizeWords()) -
                       static_cast<Core::FastI>(srcT->getSizeWords());

      auto diffBitsF = static_cast<Core::FastI>(dstT->getSizeBitsF()) -
                       static_cast<Core::FastI>(srcT->getSizeBitsF());

      auto diffBitsI = static_cast<Core::FastI>(dstT->getSizeBitsI() % wordBits) -
                       static_cast<Core::FastI>(srcT->getSizeBitsI() % wordBits);

      if(!diffWords && !diffBitsF) return;

      Core::FastU expMax, expMin;
      if(diffBitsF < 0)
      {
         expMax = srcT->getBias() + dstT->getBias() + 1;
         expMin = srcT->getBias() - dstT->getBias();
      }
      else
      {
         expMax = (1 << srcT->getSizeBitsF()) - 1;
         expMin = 0;
      }

      IR::Glyph labelChk0   = {ctx.prog, ctx.fn->genLabel()};
      IR::Glyph labelChkINF = {ctx.prog, ctx.fn->genLabel()};
      IR::Glyph labelMan    = {ctx.prog, ctx.fn->genLabel()};
      IR::Glyph labelSig    = {ctx.prog, ctx.fn->genLabel()};

      // Store source in a temporary.
      Temporary src{ctx, exp->pos, srcT->getSizeWords()};
      ctx.block.addStmnt(IR::CodeBase::Move, src.getArg(), src.getArgStk());

      // Check if source is NaN.
      ctx.block.setArgSize();
      ctx.block.addStmnt(IR::CodeBase::BAnd,
         IR::Block::Stk(), src.getArg(src.size() - 1),
         GetMaskMan(srcT) | GetMaskExp(srcT));
      ctx.block.addStmnt(IR::CodeBase::CmpGT+'I',
         IR::Block::Stk(), IR::Block::Stk(), GetMaskExp(srcT));
      ctx.block.addStmnt(IR::CodeBase::Jcnd_Nil,
         IR::Block::Stk(), labelChkINF);

      // Generate NaN.
      ctx.block.setArgSize();
      for(auto i = dstT->getSizeWords(); --i;)
         ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(),
            (Core::FastU(1) << Target::GetWordBits()) - 1);
      ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(),
         (Core::FastU(1) << (Target::GetWordBits() - 1)) - 1);
      ctx.block.addStmnt(IR::CodeBase::Jump, labelSig);

      // Check if result is INF.
      ctx.block.addLabel(labelChkINF);
      CheckExp(exp, ctx, srcT, src, IR::CodeBase::CmpGE+'I', expMax,
         IR::CodeBase::Jcnd_Nil, labelChk0);

      // Generate INF.
      ctx.block.setArgSize();
      for(auto i = dstT->getSizeWords(); --i;)
         ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), 0);
      ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), GetMaskExp(dstT));
      ctx.block.addStmnt(IR::CodeBase::Jump, labelSig);

      // Check if result is 0.
      ctx.block.addLabel(labelChk0);
      CheckExp(exp, ctx, srcT, src, IR::CodeBase::CmpLE+'I', expMin,
         IR::CodeBase::Jcnd_Nil, labelMan);

      // Generate 0.
      ctx.block.setArgSize();
      for(auto i = dstT->getSizeWords(); i--;)
         ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), 0);
      ctx.block.addStmnt(IR::CodeBase::Jump, labelSig);

      // Convert mantissa.
      ctx.block.addLabel(labelMan);

      if(diffWords > 0)
      {
         for(auto i = diffWords; i--;)
            ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), 0);
      }

      GetMan(exp, ctx, srcT, src, false);

      if(diffBitsI)
      {
         // Determine shift code and create amount expression.
         IR::Code    shiftCode;
         Core::FastU shiftVal;
         Core::FastU shiftBytes =
            std::max(dstT->getSizeBytes(), srcT->getSizeBytes());

         if(diffBitsI > 0)
         {
            shiftCode = IR::CodeBase::ShL+'U';
            shiftVal  = +diffBitsI;
         }
         else
         {
            shiftCode = IR::CodeBase::ShR+'U';
            shiftVal  = -diffBitsI;
         }

         // Generate shift.
         ctx.block.setArgSize().addStmnt(shiftCode,
            IR::Arg_Stk(shiftBytes), IR::Arg_Stk(shiftBytes), shiftVal);
      }

      if(diffWords < 0)
         DropLow(exp, ctx, dstT, diffWords);

      // Convert exponent.
      ctx.block.setArgSize().addStmnt(IR::CodeBase::BAnd,
         IR::Block::Stk(), src.getArg(src.size() - 1), GetMaskExp(srcT));

      if(diffBitsF > 0)
      {
         Core::FastU bitsI    = dstT->getSizeBitsI() % wordBits;
         Core::FastU diffBias = dstT->getBias() - srcT->getBias();

         ctx.block.addStmnt(IR::CodeBase::ShR+'I',
            IR::Block::Stk(), IR::Block::Stk(), diffBitsF);
         ctx.block.addStmnt(IR::CodeBase::Add+'U',
            IR::Block::Stk(), IR::Block::Stk(), diffBias << bitsI);
      }
      else if(diffBitsF < 0)
      {
         Core::FastU bitsI    = srcT->getSizeBitsI() % wordBits;
         Core::FastU diffBias = srcT->getBias() - dstT->getBias();

         ctx.block.addStmnt(IR::CodeBase::Sub+'U',
            IR::Block::Stk(), IR::Block::Stk(), diffBias << bitsI);
         ctx.block.addStmnt(IR::CodeBase::ShL+'U',
            IR::Block::Stk(), IR::Block::Stk(), -diffBitsF);
      }

      ctx.block.addStmnt(IR::CodeBase::BOrI,
         IR::Block::Stk(), IR::Block::Stk(), IR::Block::Stk());

      // Convert sign.
      ctx.block.addLabel(labelSig);
      ctx.block.addStmnt(IR::CodeBase::BAnd,
         IR::Block::Stk(), src.getArg(src.size() - 1), GetMaskSig(srcT));
      ctx.block.addStmnt(IR::CodeBase::BOrI,
         IR::Block::Stk(), IR::Block::Stk(), IR::Block::Stk());
   }

   //
   // GenStmnt_ConvertFltFix
   //
   // Unfortunate assumptions:
   //    Everything from GenStmnt_ConvertFloat.
   //    The destination type can represent the source value, even if
   //       inaccurately. Out of range values will silently produce invalid
   //       floating values rather than infinity or 0.
   //
   void GenStmnt_ConvertFltFix(Exp const *exp, Type const *dstT,
      Type const *srcT, GenStmntCtx const &ctx)
   {
      auto diffWords = static_cast<Core::FastI>(dstT->getSizeWords()) -
                       static_cast<Core::FastI>(srcT->getSizeWords());

      auto codeBytes = std::max(dstT->getSizeBytes(), srcT->getSizeBytes());

      auto expMid = dstT->getBias() - dstT->getSizeBitsS() - dstT->getSizeBitsF()
         + srcT->getSizeBitsI() + srcT->getSizeBitsS();

      auto headBits = dstT->getSizeBitsS() + dstT->getSizeBitsF();

      IR::Glyph labelEnd  = {ctx.prog, ctx.fn->genLabel()};
      IR::Glyph labelShL  = {ctx.prog, ctx.fn->genLabel()};
      IR::Glyph labelShLB = {ctx.prog, ctx.fn->genLabel()};

      // Temporary to store the sign and exponent of the result. The
      // mantissa will remain on the stack throughout.
      Temporary dst{ctx, exp->pos, 1};

      // Set initial exponent value.
      ctx.block.setArgSize().addStmnt(IR::CodeBase::Move, dst.getArg(),
         expMid << (dstT->getSizeBitsI() % Target::GetWordBits()));

      // Sign handling, if source type is signed.
      // Must do this before counting leading zeros.
      if(srcT->getSizeBitsS())
      {
         IR::Glyph labelPos = {ctx.prog, ctx.fn->genLabel()};

         // Check sign of source.
         ctx.block.addStmnt(IR::CodeBase::Copy,
            IR::Block::Stk(), IR::Block::Stk());
         ctx.block.addStmnt(IR::CodeBase::BAnd,
            IR::Block::Stk(), IR::Block::Stk(), GetMaskSig(srcT));
         ctx.block.addStmnt(IR::CodeBase::Jcnd_Nil,
            IR::Block::Stk(), labelPos);

         // Set sign bit and negate source.
         ctx.block.addStmnt(IR::CodeBase::BOrI,
            dst.getArg(), dst.getArg(), GetMaskSig(dstT));

         ctx.block.addStmnt(IR::CodeBase::Neg+'I',
            IR::Arg_Stk(srcT->getSizeBytes()), IR::Arg_Stk(srcT->getSizeBytes()));

         ctx.block.addLabel(labelPos);
      }

      // Leading zero count of the input.
      Temporary clz = {ctx, exp->pos, 1};

      // Check if source is 0.
      {
         IR::Glyph labelTru = {ctx.prog, ctx.fn->genLabel()};

         Temporary tmp{ctx, exp->pos, srcT->getSizeWords()};
         ctx.block.addStmnt(IR::CodeBase::Move,
            tmp.getArg(), tmp.getArgStk());

         // Test for 0.
         ctx.block.addStmnt(IR::CodeBase::Jcnd_Tru, tmp.getArg(), labelTru);

         // Push 0 and end.
         for(auto i = dstT->getSizeWords(); i--;)
         ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), 0);
         ctx.block.addStmnt(IR::CodeBase::Jump, labelEnd);

         // Otherwise, put source back on stack and continue conversion.
         // Also use the opportunity to count leading zeros.
         ctx.block.addLabel(labelTru);
         ctx.block.addStmnt(IR::CodeBase::Bclz, clz.getArg(), tmp.getArg());
         ctx.block.addStmnt(IR::CodeBase::Move, tmp.getArgStk(), tmp.getArg());
      }

      // Expand word count.
      if(diffWords > 0)
         FillLow(exp, ctx, srcT, diffWords);

      // If too few leading zeroes, shift right and increase exponent.

      ctx.block.setArgSize();

      ctx.block.addStmnt(IR::CodeBase::CmpLT+'I',
         IR::Block::Stk(), clz.getArg(), headBits - 1);
      ctx.block.addStmnt(IR::CodeBase::Jcnd_Nil,
         IR::Block::Stk(), labelShL);

      ctx.block.addStmnt(IR::CodeBase::Sub+'U',
         clz.getArg(), headBits - 1, clz.getArg());

      ctx.block.addStmnt(IR::CodeBase::ShR+'U',
         IR::Arg_Stk(codeBytes), IR::Arg_Stk(codeBytes), clz.getArg());

      ctx.block.addStmnt(IR::CodeBase::ShL+'U',
         IR::Block::Stk(), clz.getArg(), Target::GetWordBits() - headBits);
      ctx.block.addStmnt(IR::CodeBase::Add+'U',
         dst.getArg(), dst.getArg(), IR::Block::Stk());

      ctx.block.addStmnt(IR::CodeBase::Jump, labelShLB);

      // If too many leading zeroes, shift left and decrease exponent.

      ctx.block.addLabel(labelShL);

      ctx.block.addStmnt(IR::CodeBase::CmpGT+'I',
         IR::Block::Stk(), clz.getArg(), headBits - 1);
      ctx.block.addStmnt(IR::CodeBase::Jcnd_Nil,
         IR::Block::Stk(), labelShLB);

      ctx.block.addStmnt(IR::CodeBase::Sub+'U',
         clz.getArg(), clz.getArg(), headBits - 1);

      ctx.block.addStmnt(IR::CodeBase::ShL+'U',
         IR::Arg_Stk(codeBytes), IR::Arg_Stk(codeBytes), clz.getArg());

      ctx.block.addStmnt(IR::CodeBase::ShL+'U',
         IR::Block::Stk(), clz.getArg(), Target::GetWordBits() - headBits);
      ctx.block.addStmnt(IR::CodeBase::Sub+'U',
         dst.getArg(), dst.getArg(), IR::Block::Stk());

      ctx.block.addLabel(labelShLB);

      // Shrink word count.
      if(diffWords < 0)
         DropLow(exp, ctx, dstT, diffWords);

      // Add sign and exponent.
      ctx.block.setArgSize();
      ctx.block.addStmnt(IR::CodeBase::BAnd,
         IR::Block::Stk(), IR::Block::Stk(), GetMaskMan(dstT));
      ctx.block.addStmnt(IR::CodeBase::BOrI,
         IR::Block::Stk(), IR::Block::Stk(), dst.getArg());

      ctx.block.addLabel(labelEnd);
   }
}

// EOF

