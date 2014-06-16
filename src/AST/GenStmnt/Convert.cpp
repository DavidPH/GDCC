//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"
#include "IR/CodeSet/Bitwise.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// ExtendSign
//
static void ExtendSign(GDCC::AST::Exp const *exp,
   GDCC::AST::GenStmntCtx const &ctx, GDCC::Core::FastI diffWords)
{
   using namespace GDCC;

   auto labelEnd = ctx.fn->genLabel();
   auto labelPos = ctx.fn->genLabel();

   // Duplicate leading word and check if negative.
   ctx.block.addStatementArgs(IR::Code::Copy_W, IR::Arg_Stk(), IR::Arg_Stk());
   ctx.block.addStatementArgs(IR::Code::Move_W, IR::Arg_Stk(), 0);
   ctx.block.addStatementArgs(IR::Code::CmpI_LT_W,
      IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

   ctx.block.addStatementArgs(IR::Code::Cjmp_Nil, IR::Arg_Stk(),
      IR::ExpCreate_ValueGlyph({ctx.prog, labelPos}, exp->pos));

   // Extend sign.
   for(auto i = diffWords; i--;)
      ctx.block.addStatementArgs(IR::Code::Move_W, IR::Arg_Stk(), -1);

   ctx.block.addStatementArgs(IR::Code::Jump,
      IR::ExpCreate_ValueGlyph({ctx.prog, labelEnd}, exp->pos));

   // Positive?
   ctx.block.addLabel(labelPos);

   // Extend zero.
   for(auto i = diffWords; i--;)
      ctx.block.addStatementArgs(IR::Code::Move_W, IR::Arg_Stk(), 0);

   // Done.
   ctx.block.addLabel(labelEnd);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
   {
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
            // Fixed-point output.
            if(dstT->isCTypeFixed() || dstT->isCTypeInteg())
               return GenStmnt_ConvertFixed(exp, dstT, srcT, ctx);
         }

         throw Core::ExceptStr(exp->pos, "convert arith stub");
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
               ctx.block.addStatementArgs(IR::Code::Move_W, IR::Arg_Stk(), 0);
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
            Core::FastU shiftVal;

            if(diffBitsF > 0)
            {
               shiftCode = ExpCode_ArithFixed<IR::CodeSet_ShL>(shiftType);
               shiftVal  = +diffBitsF;
            }
            else
            {
               shiftCode = ExpCode_ArithFixed<IR::CodeSet_ShR>(shiftType);
               shiftVal  = -diffBitsF;
            }

            if(shiftCode == IR::Code::None)
               throw Core::ExceptStr(exp->pos, "no shift code for conversion");

            // Generate shift.
            ctx.block.addStatementArgs(shiftCode,
               IR::Arg_Stk(), IR::Arg_Stk(), shiftVal);
         }

         // Shrink value.
         if(diffWords < 0)
         {
            for(auto i = diffWords; i++;)
               ctx.block.addStatementArgs(IR::Code::Move_W,
                  IR::Arg_Nul(), IR::Arg_Stk());
         }
      }
   }
}

// EOF

