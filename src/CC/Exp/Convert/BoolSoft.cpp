//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C soft boolean conversion expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Convert/BoolSoft.hpp"

#include "CC/Type.hpp"

#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"

#include "Platform/Platform.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_ConvertBoolSoft_Fixed::v_genStmnt
      //
      void Exp_ConvertBoolSoft_Fixed::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         if(GenStmntNul(this, ctx, dst)) return;

         exp->genStmntStk(ctx);

         for(auto n = exp->getType()->getSizeWords(); --n;)
            ctx.block.addStatementArgs(IR::Code::OrIU_W,
               IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         GenStmnt_MovePart(this, ctx, dst, false, true);
      }

      //
      // Exp_ConvertBoolSoft_Float::v_genStmnt
      //
      void Exp_ConvertBoolSoft_Float::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         if(GenStmntNul(this, ctx, dst)) return;

         exp->genStmntStk(ctx);

         auto expType = exp->getType();
         auto bitsIF = expType->getSizeBitsI() + expType->getSizeBitsF();

         ctx.block.addStatementArgs(IR::Code::AndU_W,
            IR::Arg_Stk(), IR::Arg_Stk(),
            (Core::FastU(1) << (bitsIF % Platform::GetWordBits())) - 1);

         for(auto n = exp->getType()->getSizeWords(); --n;)
            ctx.block.addStatementArgs(IR::Code::OrIU_W,
               IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         GenStmnt_MovePart(this, ctx, dst, false, true);
      }

      //
      // Exp_ConvertBoolSoft_PtrInv::v_genStmnt
      //
      void Exp_ConvertBoolSoft_PtrInv::v_genStmnt(AST::GenStmntCtx const &ctx,
         AST::Arg const &dst) const
      {
         if(GenStmntNul(this, ctx, dst)) return;

         exp->genStmntStk(ctx);

         for(auto n = exp->getType()->getSizeWords(); --n;)
            ctx.block.addStatementArgs(IR::Code::AndU_W,
               IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Stk());

         ctx.block.addStatementArgs(IR::Code::InvU_W,
            IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         GenStmnt_MovePart(this, ctx, dst, false, true);
      }

      //
      // ExpConvert_BoolSoft
      //
      AST::Exp::CRef ExpConvert_BoolSoft(AST::Exp const *e, Core::Origin pos)
      {
         auto type = e->getType();

         if(type->isCTypeFixed() || type->isCTypeInteg())
         {
            if(type->getSizeWords() == 1)
               return static_cast<AST::Exp::CRef>(e);

            return Exp_ConvertBoolSoft_Fixed::Create(TypeBoolSoft, e, pos);
         }

         if(type->isCTypeRealFlt())
            return Exp_ConvertBoolSoft_Float::Create(TypeBoolSoft, e, pos);

         if(type->isTypePointer())
         {
            if(Platform::IsZeroNull_Point(type->getBaseType()->getQualAddr().base))
            {
               if(type->getSizeWords() == 1)
                  return static_cast<AST::Exp::CRef>(e);

               return Exp_ConvertBoolSoft_Fixed::Create(TypeBoolSoft, e, pos);
            }
            else
               return Exp_ConvertBoolSoft_PtrInv::Create(TypeBoolSoft, e, pos);
         }

         throw Core::ExceptStr(pos, "soft bool stub");
      }
   }
}

// EOF

