//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C arithmetic conversion expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Convert/Arith.hpp"

#include "CC/Exp/Arith.hpp"
#include "CC/Type.hpp"

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
   namespace CC
   {
      //
      // Exp_ConvertFixed::v_genStmnt
      //
      void Exp_ConvertFixed::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         if(GenStmntNul(this, ctx, dst)) return;

         auto typeE = exp->getType();

         auto diffWords = static_cast<Core::FastI>(type ->getSizeWords()) -
                          static_cast<Core::FastI>(typeE->getSizeWords());

         auto diffBitsF = static_cast<Core::FastI>(type ->getSizeBitsF()) -
                          static_cast<Core::FastI>(typeE->getSizeBitsF());

         // Evaluate expression to stack.
         exp->genStmntStk(ctx);

         // Expand value.
         if(diffWords > 0)
         {
            // Signed source may require sign-extending.
            if(typeE->getSizeBitsS())
               ExtendSign(this, ctx, diffWords);

            // Otherwise, expand with zeroes.
            else for(auto i = diffWords; i--;)
               ctx.block.addStatementArgs(IR::Code::Move_W, IR::Arg_Stk(), 0);
         }

         // Shift value.
         if(diffBitsF)
         {
            // Determine a shift type that has the larger size, but the
            // signedness of the source.
            AST::Type::CPtr shiftType;
            if(diffWords > 0)
            {
               if(typeE->getSizeBitsS())
                  shiftType = type->getSignType();
               else
                  shiftType = type->getUnsiType();
            }
            else
               shiftType = typeE;

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
               throw Core::ExceptStr(pos, "no shift code for conversion");

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

         // Move to destination.
         GenStmnt_MovePart(exp, ctx, dst, false, true);
      }

      //
      // ExpConvert_Arith
      //
      AST::Exp::CRef ExpConvert_Arith(AST::Type const *t, AST::Exp const *e,
         Core::Origin pos)
      {
         auto type  = t->getTypeQual();
         auto typeE = e->getType()->getTypeQual();

         if(type == typeE)
            return static_cast<AST::Exp::CRef>(e);

         // Fixed-point source.
         if(typeE->isCTypeFixed() || typeE->isCTypeInteg())
         {
            // Fixed-point output.
            if(type->isCTypeFixed() || type->isCTypeInteg())
               return Exp_ConvertFixed::Create(type, e, pos);
         }

         throw Core::ExceptStr(pos, "convert arith stub");
      }
   }
}

// EOF

