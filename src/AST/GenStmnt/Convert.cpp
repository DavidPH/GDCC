//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Type-conversion codegen.
//
//-----------------------------------------------------------------------------

#include "AST/Exp/Convert.hpp"

#include "AST/ExpCode.hpp"
#include "AST/Function.hpp"
#include "AST/Temporary.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "Platform/Platform.hpp"

#include "IR/Block.hpp"
#include "IR/CodeSet/Bitwise.hpp"
#include "IR/CodeSet/Unary.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
      static Core::FastU GetMaskExp(Type const *t);
      static Core::FastU GetMaskMan(Type const *t);

      //
      // CheckExp
      //
      static void CheckExp(Exp const *, GenStmntCtx const &ctx,
         Type const *srcT, Temporary &src, IR::OpCode opCmp,
         Core::FastU exp, IR::OpCode opJcnd, IR::Glyph label)
      {
         ctx.block.addStatementArgs({IR::Code::AndU_W, 1},
            IR::Arg_Stk(), src.getArg(src.size() - 1), GetMaskExp(srcT));

         ctx.block.addStatementArgs(opCmp, IR::Arg_Stk(), IR::Arg_Stk(),
            exp << (srcT->getSizeBitsI() % Platform::GetWordBits()));

         ctx.block.addStatementArgs(opJcnd, IR::Arg_Stk(), label);
      }

      //
      // DropLow
      //
      static void DropLow(Exp const *exp, GenStmntCtx const &ctx,
         Type const *dstT, Core::FastI diffWords)
      {
         // Store desired words in a temporary...
         Temporary tmpSwap{ctx, exp->pos, dstT->getSizeWords()};
         for(Core::FastU i = tmpSwap.size(); i--;)
            ctx.block.addStatementArgs({IR::Code::Move_W, 1},
               tmpSwap.getArg(i), IR::Arg_Stk());

         // ... drop unneeded words...
         for(auto i = diffWords; i++;)
            ctx.block.addStatementArgs({IR::Code::Move_W, 1},
               IR::Arg_Nul(), IR::Arg_Stk());

         // ... and reclaim desired words.
         for(Core::FastU i = 0, e = tmpSwap.size(); i != e; ++i)
            ctx.block.addStatementArgs({IR::Code::Move_W, 1},
               IR::Arg_Stk(), tmpSwap.getArg(i));
      }

      //
      // ExtendSign
      //
      static void ExtendSign(Exp const *, GenStmntCtx const &ctx, Core::FastI diffWords)
      {
         // Duplicate leading word and signed shift.
         ctx.block.addStatementArgs({IR::Code::Copy_W, 1},
            IR::Arg_Stk(), IR::Arg_Stk());
         ctx.block.addStatementArgs({IR::Code::ShRI_W, 1},
            IR::Arg_Stk(), IR::Arg_Stk(), Platform::GetWordBits() - 1);

         // Duplicate that result for any additional words.
         for(auto i = diffWords; --i;)
            ctx.block.addStatementArgs({IR::Code::Copy_W, 1},
               IR::Arg_Stk(), IR::Arg_Stk());
      }

      //
      // FillLow
      //
      static void FillLow(Exp const *exp, GenStmntCtx const &ctx,
         Type const *srcT, Core::FastI diffWords)
      {
         // Store current words in a temporary...
         Temporary tmp{ctx, exp->pos, srcT->getSizeWords()};
         ctx.block.addStatementArgs({IR::Code::Move_W, tmp.size()},
            tmp.getArg(), IR::Arg_Stk());

         // ... push needed words...
         for(auto i = diffWords; i--;)
            ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), 0);

         // ... and reclaim original words.
         ctx.block.addStatementArgs({IR::Code::Move_W, tmp.size()},
            IR::Arg_Stk(), tmp.getArg());
      }

      //
      // GetExp
      //
      static void GetExp(Exp const *, GenStmntCtx const &ctx,
         Type const *srcT, Temporary &src)
      {
         ctx.block.addStatementArgs({IR::Code::AndU_W, 1},
            IR::Arg_Stk(), src.getArg(src.size() - 1), GetMaskExp(srcT));

         ctx.block.addStatementArgs({IR::Code::ShRI_W, 1}, IR::Arg_Stk(),
            IR::Arg_Stk(), srcT->getSizeBitsI() % Platform::GetWordBits());
      }

      //
      // GetMan
      //
      static void GetMan(Exp const *, GenStmntCtx const &ctx,
         Type const *srcT, Temporary &src, bool full)
      {
         ctx.block.addStatementArgs({IR::Code::Move_W, src.size()},
            IR::Arg_Stk(), src.getArg());

         ctx.block.addStatementArgs({IR::Code::AndU_W, 1},
            IR::Arg_Stk(), IR::Arg_Stk(), GetMaskMan(srcT));

         if(full) ctx.block.addStatementArgs({IR::Code::OrIU_W, 1},
            IR::Arg_Stk(), IR::Arg_Stk(), GetMaskMan(srcT) + 1);
      }

      //
      // GetMaskExp
      //
      static Core::FastU GetMaskExp(Type const *t)
      {
         Core::FastU bitsF = t->getSizeBitsF();
         Core::FastU bitsI = t->getSizeBitsI() % Platform::GetWordBits();

         return ((Core::FastU(1) << bitsF) - 1) << bitsI;
      }

      //
      // GetMaskMan
      //
      static Core::FastU GetMaskMan(Type const *t)
      {
         Core::FastU bitsI = t->getSizeBitsI() % Platform::GetWordBits();

         return (Core::FastU(1) << bitsI) - 1;
      }

      //
      // GetMaskSig
      //
      static Core::FastU GetMaskSig(Type const *t)
      {
         Core::FastU bitsF = t->getSizeBitsF();
         Core::FastU bitsI = t->getSizeBitsI();

         return Core::FastU(1) << ((bitsI + bitsF) % Platform::GetWordBits());
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace AST
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

         throw Core::ExceptStr(exp->pos, "convert arith stub");
      }

      //
      // GenStmnt_ConvertBoolFix
      //
      void GenStmnt_ConvertBoolFix(Exp const *, Type const *,
         Type const *srcT, GenStmntCtx const &ctx)
      {
         ctx.block.addStatementArgs({IR::Code::NotU_W, srcT->getSizeWords()},
            IR::Arg_Stk(), IR::Arg_Stk());
         ctx.block.addStatementArgs({IR::Code::NotU_W, 1},
            IR::Arg_Stk(), IR::Arg_Stk());
      }

      //
      // GenStmnt_ConvertBoolFlt
      //
      void GenStmnt_ConvertBoolFlt(Exp const *, Type const *,
         Type const *srcT, GenStmntCtx const &ctx)
      {
         ctx.block.addStatementArgs({IR::Code::AndU_W, 1},
            IR::Arg_Stk(), IR::Arg_Stk(), ~GetMaskSig(srcT));
         ctx.block.addStatementArgs({IR::Code::NotU_W, srcT->getSizeWords()},
            IR::Arg_Stk(), IR::Arg_Stk());
         ctx.block.addStatementArgs({IR::Code::NotU_W, 1},
            IR::Arg_Stk(), IR::Arg_Stk());
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
         auto wordBits  = Platform::GetWordBits();
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
         ctx.block.addStatementArgs({IR::Code::Move_W, src.size()},
            src.getArg(), IR::Arg_Stk());

         // Check if result is INF.
         CheckExp(exp, ctx, srcT, src, {IR::Code::CmpI_GE_W, 1}, expMax,
            {IR::Code::Jcnd_Tru, 1}, labelINF);

         // Check if exponent more than mid.
         CheckExp(exp, ctx, srcT, src, {IR::Code::CmpI_GT_W, 1}, expMid,
            {IR::Code::Jcnd_Tru, 1}, labelGT);

         // Check if exponent less than mid.
         CheckExp(exp, ctx, srcT, src, {IR::Code::CmpI_LT_W, 1}, expMid,
            {IR::Code::Jcnd_Tru, 1}, labelLT);

         // Exponent is equal to mid.
         GetMan(exp, ctx, srcT, src, true);
         ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelMan);

         // Generate INF.
         ctx.block.addLabel(labelINF);
         if(dstT->getSizeBitsS())
         {
            ctx.block.addStatementArgs({IR::Code::AndU_W, 1},
               IR::Arg_Stk(), src.getArg(src.size() - 1), GetMaskSig(srcT));
            ctx.block.addStatementArgs({IR::Code::Jcnd_Tru, 1},
               IR::Arg_Stk(), labelNI);

            // Positive infinity.
            for(auto i = dstT->getSizeWords(); --i;)
               ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), -1);
            ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(),
               (Core::FastU(1) << (dstT->getSizeBitsI() % wordBits)) - 1);

            ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelPos);

            // Negative infinity.
            ctx.block.addLabel(labelNI);

            for(auto i = dstT->getSizeWords(); --i;)
               ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), 0);
            ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(),
               Core::FastU(1) << (dstT->getSizeBitsI() % wordBits));

            ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelPos);
         }
         else
         {
            for(auto i = dstT->getSizeWords(); i--;)
               ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), -1);

            ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelPos);
         }

         // Generate 0.
         ctx.block.addLabel(label0);
         for(auto i = dstT->getSizeWords(); i--;)
            ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), 0);
         ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelPos);

         // Exponent is more than mid.
         ctx.block.addLabel(labelGT);

         // Shift mantissa left.
         GetMan(exp, ctx, srcT, src, true);

         GetExp(exp, ctx, srcT, src);
         ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), expMid);
         ctx.block.addStatementArgs({IR::Code::SubU_W, 1},
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         if(diffWords > 0)
         {
            for(auto i = diffWords; i--;)
               ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), 0);

            ctx.block.addStatementArgs({IR::Code::ShLU_W, dstT->getSizeWords()},
               IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

            ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelSig);
         }
         else
         {
            ctx.block.addStatementArgs({IR::Code::ShLU_W, srcT->getSizeWords()},
               IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

            ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelMan);
         }

         // Exponent is less than mid.
         ctx.block.addLabel(labelLT);

         // Check if result is 0.
         CheckExp(exp, ctx, srcT, src, {IR::Code::CmpI_LT_W, 1}, expMin,
            {IR::Code::Jcnd_Tru, 1}, label0);

         // Shift mantissa right.
         GetMan(exp, ctx, srcT, src, true);

         ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), expMid);
         GetExp(exp, ctx, srcT, src);
         ctx.block.addStatementArgs({IR::Code::SubU_W, 1},
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         ctx.block.addStatementArgs({IR::Code::ShRU_W, srcT->getSizeWords()},
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Shrink or expand value.
         ctx.block.addLabel(labelMan);
         if(diffWords > 0)
         {
            // Value is currently positive, so no sign extend.
            for(auto i = diffWords; i--;)
               ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), 0);
         }
         else if(diffWords < 0)
         {
            for(auto i = diffWords; i++;)
               ctx.block.addStatementArgs({IR::Code::Move_W, 1},
                  IR::Arg_Nul(), IR::Arg_Stk());
         }

         // Convert sign.
         ctx.block.addLabel(labelSig);

         ctx.block.addStatementArgs({IR::Code::AndU_W, 1},
            IR::Arg_Stk(), src.getArg(src.size() - 1), GetMaskSig(srcT));
         ctx.block.addStatementArgs({IR::Code::Jcnd_Nil, 1},
            IR::Arg_Stk(), labelPos);

         ctx.block.addStatementArgs({IR::Code::NegI_W, dstT->getSizeWords()},
            IR::Arg_Stk(), IR::Arg_Stk());

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

         // Expand value.
         if(diffWords > 0)
         {
            // Signed source may require sign-extending.
            if(srcT->getSizeBitsS())
               ExtendSign(exp, ctx, diffWords);

            // Otherwise, expand with zeroes.
            else for(auto i = diffWords; i--;)
               ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), 0);
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
            IR::OpCode  shiftOp;
            Core::FastU shiftVal;

            if(diffBitsF > 0)
            {
               shiftOp  = ExpCode_ArithInteg<IR::CodeSet_ShL>(shiftType);
               shiftVal = +diffBitsF;
            }
            else
            {
               shiftOp  = ExpCode_ArithInteg<IR::CodeSet_ShR>(shiftType);
               shiftVal = -diffBitsF;
            }

            if(shiftOp.code == IR::Code::None)
               throw Core::ExceptStr(exp->pos, "no shift code for conversion");

            // Generate shift.
            ctx.block.addStatementArgs(shiftOp,
               IR::Arg_Stk(), IR::Arg_Stk(), shiftVal);
         }

         // Shrink value.
         if(diffWords < 0)
         {
            for(auto i = diffWords; i++;)
               ctx.block.addStatementArgs({IR::Code::Move_W, 1},
                  IR::Arg_Nul(), IR::Arg_Stk());
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
         Core::FastU wordBits = Platform::GetWordBits();

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
         ctx.block.addStatementArgs({IR::Code::Move_W, src.size()},
            src.getArg(), IR::Arg_Stk());

         // Check if source is NaN.
         ctx.block.addStatementArgs({IR::Code::AndU_W, 1},
            IR::Arg_Stk(), src.getArg(src.size() - 1),
            GetMaskMan(srcT) | GetMaskExp(srcT));
         ctx.block.addStatementArgs({IR::Code::CmpI_GT_W, 1},
            IR::Arg_Stk(), IR::Arg_Stk(), GetMaskExp(srcT));
         ctx.block.addStatementArgs({IR::Code::Jcnd_Nil, 1},
            IR::Arg_Stk(), labelChkINF);

         // Generate NaN.
         for(auto i = dstT->getSizeWords(); --i;)
            ctx.block.addStatementArgs({IR::Code::Move_W, 1},
               IR::Arg_Stk(), (1 << Platform::GetWordBits()) - 1);
         ctx.block.addStatementArgs({IR::Code::Move_W, 1},
            IR::Arg_Stk(), (1 << (Platform::GetWordBits() - 1)) - 1);
         ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelSig);

         // Check if result is INF.
         ctx.block.addLabel(labelChkINF);
         CheckExp(exp, ctx, srcT, src, {IR::Code::CmpI_GE_W, 1}, expMax,
            {IR::Code::Jcnd_Nil, 1}, labelChk0);

         // Generate INF.
         for(auto i = dstT->getSizeWords(); --i;)
            ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), 0);
         ctx.block.addStatementArgs({IR::Code::Move_W, 1},
            IR::Arg_Stk(), GetMaskExp(dstT));
         ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelSig);

         // Check if result is 0.
         ctx.block.addLabel(labelChk0);
         CheckExp(exp, ctx, srcT, src, {IR::Code::CmpI_LE_W, 1}, expMin,
            {IR::Code::Jcnd_Nil, 1}, labelMan);

         // Generate 0.
         for(auto i = dstT->getSizeWords(); i--;)
            ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), 0);
         ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelSig);

         // Convert mantissa.
         ctx.block.addLabel(labelMan);

         if(diffWords > 0)
         {
            for(auto i = diffWords; i--;)
               ctx.block.addStatementArgs({IR::Code::Move_W, 1}, IR::Arg_Stk(), 0);
         }

         GetMan(exp, ctx, srcT, src, false);

         if(diffBitsI)
         {
            // Determine shift code and create amount expression.
            IR::OpCode  shiftOp;
            Core::FastU shiftVal;
            Core::FastU shiftWords =
               std::max(dstT->getSizeWords(), srcT->getSizeWords());

            if(diffBitsI > 0)
            {
               shiftOp  = {IR::Code::ShLU_W, shiftWords};
               shiftVal = +diffBitsI;
            }
            else
            {
               shiftOp  = {IR::Code::ShRU_W, shiftWords};
               shiftVal = -diffBitsI;
            }

            // Generate shift.
            ctx.block.addStatementArgs(shiftOp,
               IR::Arg_Stk(), IR::Arg_Stk(), shiftVal);
         }

         if(diffWords < 0)
            DropLow(exp, ctx, dstT, diffWords);

         // Convert exponent.
         ctx.block.addStatementArgs({IR::Code::AndU_W, 1},
            IR::Arg_Stk(), src.getArg(src.size() - 1), GetMaskExp(srcT));

         if(diffBitsF > 0)
         {
            Core::FastU bitsI    = dstT->getSizeBitsI() % wordBits;
            Core::FastU diffBias = dstT->getBias() - srcT->getBias();

            ctx.block.addStatementArgs({IR::Code::ShRI_W, 1},
               IR::Arg_Stk(), IR::Arg_Stk(), diffBitsF);
            ctx.block.addStatementArgs({IR::Code::AddU_W, 1},
               IR::Arg_Stk(), IR::Arg_Stk(), diffBias << bitsI);
         }
         else if(diffBitsF < 0)
         {
            Core::FastU bitsI    = srcT->getSizeBitsI() % wordBits;
            Core::FastU diffBias = srcT->getBias() - dstT->getBias();

            ctx.block.addStatementArgs({IR::Code::SubU_W, 1},
               IR::Arg_Stk(), IR::Arg_Stk(), diffBias << bitsI);
            ctx.block.addStatementArgs({IR::Code::ShLU_W, 1},
               IR::Arg_Stk(), IR::Arg_Stk(), -diffBitsF);
         }

         ctx.block.addStatementArgs({IR::Code::OrIU_W, 1},
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Convert sign.
         ctx.block.addLabel(labelSig);
         ctx.block.addStatementArgs({IR::Code::AndU_W, 1},
            IR::Arg_Stk(), src.getArg(src.size() - 1), GetMaskSig(srcT));
         ctx.block.addStatementArgs({IR::Code::OrIU_W, 1},
            IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());
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

         auto codeWords = std::max(dstT->getSizeWords(), srcT->getSizeWords());

         auto expMid = dstT->getBias() - dstT->getSizeBitsS() - dstT->getSizeBitsF()
            + srcT->getSizeBitsI() + srcT->getSizeBitsS();

         auto headBits = dstT->getSizeBitsS() + dstT->getSizeBitsF();

         IR::Glyph labelEnd  = {ctx.prog, ctx.fn->genLabel()};
         IR::Glyph labelShL  = {ctx.prog, ctx.fn->genLabel()};
         IR::Glyph labelShLB = {ctx.prog, ctx.fn->genLabel()};

         // Temporary to store the sign and exponent of the result. The
         // mantissa will remain on the stack throughout.
         AST::Temporary dst{ctx, exp->pos, 1};

         // Set initial exponent value.
         ctx.block.addStatementArgs({IR::Code::Move_W, 1}, dst.getArg(),
            expMid << (dstT->getSizeBitsI() % Platform::GetWordBits()));

         // Sign handling, if source type is signed.
         // Must do this before counting leading zeros.
         if(srcT->getSizeBitsS())
         {
            IR::Glyph labelPos = {ctx.prog, ctx.fn->genLabel()};

            // Check sign of source.
            ctx.block.addStatementArgs({IR::Code::Copy_W, 1},
               IR::Arg_Stk(), IR::Arg_Stk());
            ctx.block.addStatementArgs({IR::Code::AndU_W, 1},
               IR::Arg_Stk(), IR::Arg_Stk(), GetMaskSig(srcT));
            ctx.block.addStatementArgs({IR::Code::Jcnd_Nil, 1},
               IR::Arg_Stk(), labelPos);

            // Set sign bit and negate source.
            ctx.block.addStatementArgs({IR::Code::OrIU_W, 1},
               dst.getArg(), dst.getArg(), GetMaskSig(dstT));

            ctx.block.addStatementArgs({IR::Code::NegI_W, srcT->getSizeWords()},
               IR::Arg_Stk(), IR::Arg_Stk());

            ctx.block.addLabel(labelPos);
         }

         // Leading zero count of the input.
         Temporary clz = {ctx, exp->pos, 1};

         // Check if source is 0.
         {
            IR::Glyph labelTru = {ctx.prog, ctx.fn->genLabel()};

            Temporary tmp{ctx, exp->pos, srcT->getSizeWords()};
            ctx.block.addStatementArgs({IR::Code::Move_W, tmp.size()},
               tmp.getArg(), IR::Arg_Stk());

            // Test for 0.
            ctx.block.addStatementArgs({IR::Code::Jcnd_Tru, tmp.size()},
               tmp.getArg(), labelTru);

            // Push 0 and end.
            ctx.block.addStatementArgs({IR::Code::Move_W, dstT->getSizeWords()},
               IR::Arg_Stk(), 0);
            ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelEnd);

            // Otherwise, put source back on stack and continue conversion.
            // Also use the opportunity to count leading zeros.
            ctx.block.addLabel(labelTru);
            ctx.block.addStatementArgs({IR::Code::Bclz_W, tmp.size()},
               clz.getArg(), tmp.getArg());
            ctx.block.addStatementArgs({IR::Code::Move_W, tmp.size()},
               IR::Arg_Stk(), tmp.getArg());
         }

         // Expand word count.
         if(diffWords > 0)
            FillLow(exp, ctx, srcT, diffWords);

         // If too few leading zeroes, shift right and increase exponent.

         ctx.block.addStatementArgs({IR::Code::CmpI_LT_W, 1},
            IR::Arg_Stk(), clz.getArg(), headBits - 1);
         ctx.block.addStatementArgs({IR::Code::Jcnd_Nil, 1},
            IR::Arg_Stk(), labelShL);

         ctx.block.addStatementArgs({IR::Code::SubU_W, 1},
            clz.getArg(), headBits - 1, clz.getArg());

         ctx.block.addStatementArgs({IR::Code::ShRU_W, codeWords},
            IR::Arg_Stk(), IR::Arg_Stk(), clz.getArg());

         ctx.block.addStatementArgs({IR::Code::ShLU_W, 1},
            IR::Arg_Stk(), clz.getArg(), Platform::GetWordBits() - headBits);
         ctx.block.addStatementArgs({IR::Code::AddU_W, 1},
            dst.getArg(), dst.getArg(), IR::Arg_Stk());

         ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelShLB);

         // If too many leading zeroes, shift left and decrease exponent.

         ctx.block.addLabel(labelShL);

         ctx.block.addStatementArgs({IR::Code::CmpI_GT_W, 1},
            IR::Arg_Stk(), clz.getArg(), headBits - 1);
         ctx.block.addStatementArgs({IR::Code::Jcnd_Nil, 1},
            IR::Arg_Stk(), labelShLB);

         ctx.block.addStatementArgs({IR::Code::SubU_W, 1},
            clz.getArg(), clz.getArg(), headBits - 1);

         ctx.block.addStatementArgs({IR::Code::ShLU_W, codeWords},
            IR::Arg_Stk(), IR::Arg_Stk(), clz.getArg());

         ctx.block.addStatementArgs({IR::Code::ShLU_W, 1},
            IR::Arg_Stk(), clz.getArg(), Platform::GetWordBits() - headBits);
         ctx.block.addStatementArgs({IR::Code::SubU_W, 1},
            dst.getArg(), dst.getArg(), IR::Arg_Stk());

         ctx.block.addLabel(labelShLB);

         // Shrink word count.
         if(diffWords < 0)
            DropLow(exp, ctx, dstT, diffWords);

         // Add sign and exponent.
         ctx.block.addStatementArgs({IR::Code::AndU_W, 1},
            IR::Arg_Stk(), IR::Arg_Stk(), GetMaskMan(dstT));
         ctx.block.addStatementArgs({IR::Code::OrIU_W, 1},
            IR::Arg_Stk(), IR::Arg_Stk(), dst.getArg());

         ctx.block.addLabel(labelEnd);
      }
   }
}

// EOF

