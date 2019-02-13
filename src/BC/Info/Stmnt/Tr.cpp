//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Translate statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "BC/AddFunc.hpp"

#include "IR/Function.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC
{
   //
   // Info::addFunc_Tr_W
   //
   void Info::addFunc_Tr_W(IR::CodeType type, Core::FastU dstN, Core::FastU srcN)
   {
      if(!type[0]) type[0] = 'U';
      if(!type[1]) type[1] = type[0];

      if(type[0] == 'E' || type[0] == 'F')
      {
         if(type[1] == 'E' || type[1] == 'F')
            return addFunc_Tr_W(type, getFloatInfo(dstN, type[0]), getFloatInfo(srcN, type[1]));
         else
            return addFunc_Tr_W(type, getFloatInfo(dstN, type[0]), getFixedInfo(srcN, type[1]));
      }
      else
      {
         if(type[1] == 'E' || type[1] == 'F')
            return addFunc_Tr_W(type, getFixedInfo(dstN, type[0]), getFloatInfo(srcN, type[1]));
         else
            return addFunc_Tr_W(type, getFixedInfo(dstN, type[0]), getFixedInfo(srcN, type[1]));
      }
   }

   //
   // Info::addFunc_Tr_W (Fixed-Fixed)
   //
   void Info::addFunc_Tr_W(IR::CodeType type, FixedInfo dstFI, FixedInfo srcFI)
   {
      GDCC_BC_AddFuncPre((Code::Tr+type, dstFI.words, srcFI.words),
         dstFI.words, srcFI.words, std::max(dstFI.words, srcFI.words), __FILE__);
      GDCC_BC_AddFuncObjUna(srcFI.words);

      GDCC_BC_AddFuncObjReg(dst, dstFI.words, 0);

      auto diffWords = static_cast<Core::FastI>(dstFI.words) -
                       static_cast<Core::FastI>(srcFI.words);

      auto diffBitsF = static_cast<Core::FastI>(dstFI.bitsF) -
                       static_cast<Core::FastI>(srcFI.bitsF);

      // Expand value.
      if(diffWords > 0)
      {
         // Signed source requires sign-extending.
         if(srcFI.bitsS)
         {
            GDCC_BC_AddStmnt(Code::ShR+I, 1, dst[srcFI.words], src.hi, 31);
            for(auto i = srcFI.words + 1; i != dstFI.words; ++i)
               GDCC_BC_AddStmnt(Code::Move, 1, dst[i], dst[srcFI.words]);
         }
         // Otherwise, expand with zeroes.
         else
         {
            for(auto i = srcFI.words; i != dstFI.words; ++i)
               GDCC_BC_AddStmnt(Code::Move, 1, dst[i], 0);
         }
      }

      // Shift value.
      if(diffBitsF)
      {
         auto &tmp = diffWords > 0 ? dst : src;

         char t = dstFI.bitsS || srcFI.bitsS ? 'I' : 'U';

         if(diffBitsF > 0)
            GDCC_BC_AddStmnt(Code::ShL+t, 1, tmp, tmp,  diffBitsF);
         else
            GDCC_BC_AddStmnt(Code::ShR+t, 1, tmp, tmp, -diffBitsF);
      }

      // Shrink value.
      GDCC_BC_AddStmnt(Code::Retn, dstFI.words, dst);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_Tr_W (Fixed-Float)
   //
   // Unfortunate assumptions:
   //    Everything from Float-Float.
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
   void Info::addFunc_Tr_W(IR::CodeType type, FixedInfo dstFI, FloatInfo srcFI)
   {
      GDCC_BC_AddFuncPre((Code::Tr+type, dstFI.words, srcFI.words),
         dstFI.words, srcFI.words, srcFI.words, __FILE__);
      GDCC_BC_AddFuncObjUna(srcFI.words);

      auto diffWords = static_cast<Core::FastI>(dstFI.words) -
                       static_cast<Core::FastI>(srcFI.words);

      auto expMax = srcFI.offExp + dstFI.bitsI;
      auto expMid = srcFI.offExp + srcFI.bitsManFull - dstFI.bitsF;
      auto expMin = srcFI.offExp - dstFI.bitsF;

      IR::Glyph label0   = {prog, name + "$0"};
      IR::Glyph labelGT  = {prog, name + "$gt"};
      IR::Glyph labelINF = {prog, name + "$inf"};
      IR::Glyph labelLT  = {prog, name + "$lt"};
      IR::Glyph labelMan = {prog, name + "$man"};
      IR::Glyph labelNI  = {prog, name + "$ni"};
      IR::Glyph labelPos = {prog, name + "$pos"};
      IR::Glyph labelSig = {prog, name + "$sig"};

      // Check if result is INF.
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, src.hi, srcFI.maskExp);
      GDCC_BC_AddStmnt(Code::CmpGE+I,  1, stk, stk,    expMax << srcFI.bitsMan);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelINF);

      // Check if exponent more than mid.
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, src.hi, srcFI.maskExp);
      GDCC_BC_AddStmnt(Code::CmpGT+I,  1, stk, stk,    expMid << srcFI.bitsMan);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelGT);

      // Check if exponent less than mid.
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, src.hi, srcFI.maskExp);
      GDCC_BC_AddStmnt(Code::CmpLT+I,  1, stk, stk,    expMid << srcFI.bitsMan);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelLT);

      // Exponent is equal to mid.
      GDCC_BC_AddStmnt(Code::Move, srcFI.words, stk, src);
      GDCC_BC_AddStmnt(Code::BAnd, 1,           stk, stk, srcFI.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1,           stk, stk, srcFI.maskMan + 1);
      GDCC_BC_AddStmnt(Code::Jump, 1,           labelMan);

      // Exponent is more than mid.
      GDCC_BC_AddLabel(labelGT);

      // Shift mantissa left.
      GDCC_BC_AddStmnt(Code::Move, srcFI.words, stk, src);
      GDCC_BC_AddStmnt(Code::BAnd, 1,           stk, stk, srcFI.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1,           stk, stk, srcFI.maskMan + 1);

      // If expanding value, do so before shifting.
      if(diffWords > 0) for(auto i = diffWords; i--;)
         GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);

      GDCC_BC_AddStmnt(Code::BAnd,  1, stk, src.hi, srcFI.maskExp);
      GDCC_BC_AddStmnt(Code::ShR+I, 1, stk, stk,    srcFI.bitsMan);
      GDCC_BC_AddStmnt(Code::Sub+U, 1, stk, stk,    expMid);

      if(diffWords > 0)
      {
         GDCC_BC_AddStmnt(Code::ShL+U, 1, Stk(dstFI.words), Stk(dstFI.words), stk);
         GDCC_BC_AddStmnt(Code::Jump,  1, labelSig);
      }
      else
      {
         GDCC_BC_AddStmnt(Code::ShL+U, 1, Stk(srcFI.words), Stk(srcFI.words), stk);
         GDCC_BC_AddStmnt(Code::Jump,  1, labelMan);
      }

      // Exponent if less than mid.
      GDCC_BC_AddLabel(labelLT);

      // Check if result is 0.
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, src.hi, srcFI.maskExp);
      GDCC_BC_AddStmnt(Code::CmpLT+I,  1, stk, stk,    expMin << srcFI.bitsMan);
      GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, label0);

      // Shift mantissa right.
      GDCC_BC_AddStmnt(Code::Move, srcFI.words, stk, src);
      GDCC_BC_AddStmnt(Code::BAnd, 1,           stk, stk, srcFI.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1,           stk, stk, srcFI.maskMan + 1);

      GDCC_BC_AddStmnt(Code::Move,  1, stk, expMid);
      GDCC_BC_AddStmnt(Code::BAnd,  1, stk, src.hi, srcFI.maskExp);
      GDCC_BC_AddStmnt(Code::ShR+I, 1, stk, stk,    srcFI.bitsMan);
      GDCC_BC_AddStmnt(Code::Sub+U, 1, stk, stk,    stk);

      GDCC_BC_AddStmnt(Code::ShR+U, 1, Stk(srcFI.words), Stk(srcFI.words), stk);

      // Shrink or expand value.
      GDCC_BC_AddLabel(labelMan);
      if(diffWords > 0)
      {
         // Value is currently positive, so no sign extend.
         for(auto i = diffWords; i--;)
            GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);
      }
      else
         GDCC_BC_AddStmnt(Code::Move, -diffWords, nul, stk);

      // Convert sign.
      GDCC_BC_AddLabel(labelSig);

      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, src.hi, srcFI.maskSig);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelPos);

      GDCC_BC_AddStmnt(Code::Neg+I, dstFI.words, stk, stk);

      // Return result.
      GDCC_BC_AddLabel(labelPos);
      GDCC_BC_AddStmnt(Code::Retn, dstFI.words, stk);

      // Generate INF.
      GDCC_BC_AddLabel(labelINF);
      if(dstFI.bitsS)
      {
         GDCC_BC_AddStmnt(Code::BAnd,     1, stk, src.hi, srcFI.maskSig);
         GDCC_BC_AddStmnt(Code::Jcnd_Tru, 1, stk, labelNI);

         // Positive infinity.
         for(auto i = dstFI.words; --i;)
            GDCC_BC_AddStmnt(Code::Move, 1, stk, 0xFFFFFFFF);
         GDCC_BC_AddStmnt(Code::Move,    1, stk, 0x7FFFFFFF);
         GDCC_BC_AddStmnt(Code::Jump,    1, labelPos);

         // Negative infinity.
         GDCC_BC_AddLabel(labelNI);
         for(auto i = dstFI.words; --i;)
            GDCC_BC_AddStmnt(Code::Move, 1, stk, 0x00000000);
         GDCC_BC_AddStmnt(Code::Move,    1, stk, 0x80000000);
         GDCC_BC_AddStmnt(Code::Jump,    1, labelPos);
      }
      else
      {
         for(auto i = dstFI.words; i--;)
            GDCC_BC_AddStmnt(Code::Move, 1, stk, 0xFFFFFFFF);
      GDCC_BC_AddStmnt(Code::Retn, dstFI.words, stk);
      }

      // Generate 0.
      GDCC_BC_AddLabel(label0);
      for(auto i = dstFI.words; i--;)
         GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);
      GDCC_BC_AddStmnt(Code::Retn, dstFI.words, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_Tr_W (Float-Fixed)
   //
   // Unfortunate assumptions:
   //    Everything from Float-Float.
   //    The destination type can represent the source value, even if
   //       inaccurately. Out of range values will silently produce invalid
   //       floating values rather than infinity or 0.
   //
   void Info::addFunc_Tr_W(IR::CodeType type, FloatInfo dstFI, FixedInfo srcFI)
   {
      GDCC_BC_AddFuncPre((Code::Tr+type, dstFI.words, srcFI.words),
         dstFI.words, srcFI.words, srcFI.words + 2, __FILE__);
      GDCC_BC_AddFuncObjUna(srcFI.words);

      // Only used if dst is smaller than src.
      GDCC_BC_AddFuncObjReg(tmp, dstFI.words, 0);

      GDCC_BC_AddFuncObjReg(dst, 1, srcFI.words + 0);
      GDCC_BC_AddFuncObjReg(clz, 1, srcFI.words + 1);

      auto nm = std::max(dstFI.words, srcFI.words);

      auto diffWords = static_cast<Core::FastI>(dstFI.words) -
                       static_cast<Core::FastI>(srcFI.words);

      auto expMid = dstFI.offExp - dstFI.bitsSig - dstFI.bitsExp
         + srcFI.bitsI + srcFI.bitsS;

      auto bitsHead = dstFI.bitsSig + dstFI.bitsExp;

      IR::Glyph label0    = {prog, name + "$0"};
      IR::Glyph labelShL  = {prog, name + "$shL"};
      IR::Glyph labelShLB = {prog, name + "$shLB"};

      // Set initial exponent value.
      GDCC_BC_AddStmnt(Code::Move, 1, dst, expMid << dstFI.bitsMan);

      // Sign handling, if source type is signed.
      // Must do this before counting leading zeros.
      if(srcFI.bitsS)
      {
         IR::Glyph labelPos = {prog, name + "$pos"};

         // Check sign of source.
         GDCC_BC_AddStmnt(Code::BAnd,     1, stk, src.hi, 0x80000000);
         GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelPos);

         // Set sign bit and negate source.
         GDCC_BC_AddStmnt(Code::BOrI,  1, dst, dst, dstFI.maskSig);
         GDCC_BC_AddStmnt(Code::Neg+I, 1, src, src);

         GDCC_BC_AddLabel(labelPos);
      }

      // Count leading zeroes. Return zero if source is zero.
      GDCC_BC_AddStmnt(Code::Bclz,     1, stk, src);
      GDCC_BC_AddStmnt(Code::Jcnd_Tab, 1, stk, srcFI.words * 32, label0);
      GDCC_BC_AddStmnt(Code::Move,     1, clz, stk);

      // Expand word count.
      if(diffWords > 0) for(auto i = diffWords; i--;)
         GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);
      GDCC_BC_AddStmnt(Code::Move, srcFI.words, stk, src);

      // If too few leading zeroes, shift right and increase exponent.
      GDCC_BC_AddStmnt(Code::CmpLT+I,  1, stk, clz, bitsHead - 1);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelShL);

      GDCC_BC_AddStmnt(Code::Sub+U,  1, clz, bitsHead - 1, clz);
      GDCC_BC_AddStmnt(Code::ShR+U, nm, stk, stk,          clz);

      GDCC_BC_AddStmnt(Code::ShL+U, 1, stk, clz, dstFI.bitsMan);
      GDCC_BC_AddStmnt(Code::Add+U, 1, dst, dst, stk);

      GDCC_BC_AddStmnt(Code::Jump, 1, labelShLB);

      // If too many leading zeroes, shift left and decrease exponent.
      GDCC_BC_AddLabel(labelShL);

      GDCC_BC_AddStmnt(Code::CmpGT+I,  1, stk, clz, bitsHead - 1);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelShLB);

      GDCC_BC_AddStmnt(Code::Sub+U,  1, clz, clz, bitsHead - 1);
      GDCC_BC_AddStmnt(Code::ShL+U, nm, stk, stk, clz);

      GDCC_BC_AddStmnt(Code::ShL+U, 1, stk, clz, dstFI.bitsMan);
      GDCC_BC_AddStmnt(Code::Sub+U, 1, dst, dst, stk);

      GDCC_BC_AddLabel(labelShLB);

      // Shrink word count.
      if(diffWords < 0)
      {
         GDCC_BC_AddStmnt(Code::Move, dstFI.words, tmp, stk);
         GDCC_BC_AddStmnt(Code::Move, -diffWords,  nul, stk);
         GDCC_BC_AddStmnt(Code::Move, dstFI.words, stk, tmp);
      }

      // Add sign and exponent.
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, stk, dstFI.maskMan);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, dst);

      GDCC_BC_AddStmnt(Code::Retn, dstFI.words, stk);

      // Return 0.
      GDCC_BC_AddLabel(label0);
      for(auto i = dstFI.words; i--;)
         GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);
      GDCC_BC_AddStmnt(Code::Retn, dstFI.words, stk);

      GDCC_BC_AddFuncEnd();
   }

   //
   // Info::addFunc_Tr_W (Float-Float)
   //
   // Unfortunate assumptions:
   //    All floats are signed.
   //    Sign and exponent fit in highest word.
   //    NaNs have mantissa bits in the highest word set. (This assumption
   //       exists elsewhere as well and serves as an optimization.)
   //    Words are ordered little-endian, another widespread assumption.
   //
   void Info::addFunc_Tr_W(IR::CodeType type, FloatInfo dstFI, FloatInfo srcFI)
   {
      GDCC_BC_AddFuncPre((Code::Tr+type, dstFI.words, srcFI.words),
         dstFI.words, srcFI.words, srcFI.words, __FILE__);
      GDCC_BC_AddFuncObjUna(srcFI.words);

      // Only used if dst is smaller than src.
      GDCC_BC_AddFuncObjReg(dst, dstFI.words, 0);

      auto diffWords = static_cast<Core::FastI>(dstFI.words) -
                       static_cast<Core::FastI>(srcFI.words);

      auto diffBitsExp = static_cast<Core::FastI>(dstFI.bitsExp) -
                         static_cast<Core::FastI>(srcFI.bitsExp);

      auto diffBitsMan = static_cast<Core::FastI>(dstFI.bitsMan) -
                         static_cast<Core::FastI>(srcFI.bitsMan);

      auto diffOffExp = static_cast<Core::FastI>(dstFI.offExp) -
                        static_cast<Core::FastI>(srcFI.offExp);

      Core::FastU expMax, expMin;
      if(diffBitsExp < 0)
      {
         expMax = srcFI.offExp + dstFI.offExp + 1;
         expMin = srcFI.offExp - dstFI.offExp;
      }
      else
      {
         expMax = srcFI.maxExp;
         expMin = 0;
      }

      IR::Glyph labelChk0   = {prog, name + "$chk0"};
      IR::Glyph labelChkINF = {prog, name + "$chkINF"};
      IR::Glyph labelMan    = {prog, name + "$man"};
      IR::Glyph labelSig    = {prog, name + "$sig"};

      // Check if source is NaN.
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, src.hi, srcFI.maskExp | srcFI.maskMan);
      GDCC_BC_AddStmnt(Code::CmpGT+I,  1, stk, stk,    srcFI.maskExp);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelChkINF);

      // Generate NaN.
      for(auto i = dstFI.words; --i;)
         GDCC_BC_AddStmnt(Code::Move, 1, stk, 0xFFFFFFFF);
      GDCC_BC_AddStmnt(Code::Move,    1, stk, srcFI.maskExp | srcFI.maskMan);
      GDCC_BC_AddStmnt(Code::Jump,    0, labelSig);

      // Check if result if INF.
      GDCC_BC_AddLabel(labelChkINF);
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, src.hi, srcFI.maskExp);
      GDCC_BC_AddStmnt(Code::CmpGE+I,  1, stk, stk,    expMax << srcFI.bitsMan);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelChk0);

      // Generate INF.
      for(auto i = dstFI.words; --i;)
         GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);
      GDCC_BC_AddStmnt(Code::Move,    1, stk, dstFI.maskExp);
      GDCC_BC_AddStmnt(Code::Jump,    1, labelSig);

      // Check if result is 0.
      GDCC_BC_AddLabel(labelChk0);
      GDCC_BC_AddStmnt(Code::BAnd,     1, stk, src.hi, srcFI.maskExp);
      GDCC_BC_AddStmnt(Code::CmpLE+I,  1, stk, stk,    expMin << srcFI.bitsMan);
      GDCC_BC_AddStmnt(Code::Jcnd_Nil, 1, stk, labelMan);

      // Generate 0.
      for(auto i = dstFI.words; i--;)
         GDCC_BC_AddStmnt(Code::Move, 1, stk, 0);
      GDCC_BC_AddStmnt(Code::Jump,    1, labelSig);

      // Convert mantissa.
      GDCC_BC_AddLabel(labelMan);

      if(diffWords > 0) for(auto i = diffWords; i--;)
         GDCC_BC_AddStmnt(Code::Move, 1,        stk, 0);
      GDCC_BC_AddStmnt(Code::Move, srcFI.words, stk, src);
      GDCC_BC_AddStmnt(Code::BAnd, 1,           stk, stk, srcFI.maskMan);

      if(diffBitsMan)
      {
         auto shN = std::max(dstFI.words, srcFI.words);
         if(diffBitsMan > 0)
            GDCC_BC_AddStmnt(Code::ShL+U, 1, Stk(shN), Stk(shN),  diffBitsMan);
         else
            GDCC_BC_AddStmnt(Code::ShR+U, 1, Stk(shN), Stk(shN), -diffBitsMan);
      }

      if(diffWords < 0)
      {
         GDCC_BC_AddStmnt(Code::Move, dstFI.words, dst, stk);
         GDCC_BC_AddStmnt(Code::Move, -diffWords,  nul, stk);
         GDCC_BC_AddStmnt(Code::Move, dstFI.words, stk, dst);
      }

      // Convert exponent.
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, src.hi, srcFI.maskExp);

      if(diffBitsExp > 0)
      {
         GDCC_BC_AddStmnt(Code::ShR+I, 1, stk, stk, diffBitsExp);
         GDCC_BC_AddStmnt(Code::Add+U, 1, stk, stk, diffOffExp << dstFI.bitsMan);
      }
      else if(diffBitsExp < 0)
      {
         GDCC_BC_AddStmnt(Code::Sub+U, 1, stk, stk, -diffOffExp << srcFI.bitsMan);
         GDCC_BC_AddStmnt(Code::ShL+U, 1, stk, stk, -diffBitsExp);
      }

      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk, stk);

      // Convert sign.
      GDCC_BC_AddLabel(labelSig);
      GDCC_BC_AddStmnt(Code::BAnd, 1, stk, src.hi, srcFI.maskSig);
      GDCC_BC_AddStmnt(Code::BOrI, 1, stk, stk,    stk);

      // Return result.
      GDCC_BC_AddStmnt(Code::Retn, dstFI.words, stk);

      GDCC_BC_AddFuncEnd();
   }
}

// EOF

