//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C pointer conversion expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Convert/Pointer.hpp"

#include "CC/Factory.hpp"
#include "CC/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"

#include "SR/Exp/IRExp.hpp"
#include "SR/Type.hpp"

#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // ExpConvert_PtrSpace
   //
   static SR::Exp::CRef ExpConvert_PtrSpace(SR::Type const *typeL,
      SR::Exp const *exp, Core::Origin pos)
   {
      auto typeR = exp->getType();

      auto baseL = typeL->getBaseType();
      auto baseR = typeR->getBaseType();

      auto addrL = baseL->getQualAddr();
      auto addrR = baseR->getQualAddr();

      // To far.
      if(addrL.base == IR::AddrBase::Far)
         Core::Error(pos, "convert to far pointer stub");

      // From far.
      if(addrR.base == IR::AddrBase::Far)
         Core::Error(pos, "convert from far pointer stub");

      // As a last resort, same size pointers can fall back to direct bitwise copy.
      if(typeL->getSizeWords() == typeR->getSizeWords())
      {
         baseR = baseR->getTypeQualAddr(addrL);
         typeR = baseR->getTypePointer();

         // If null representation is different, perform inversion.
         if(Target::IsZeroNull_Point(addrL.base) != Target::IsZeroNull_Point(addrR.base))
            return Exp_ConvertPtrInv::Create(typeR, exp, pos);
         else
            return Exp_ConvertPtr::Create(typeR, exp, pos);
      }

      Core::Error(pos, "unsupported pointer conversion");
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_ConvertPtr::v_genStmnt
   //
   void Exp_ConvertPtr::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      return exp->genStmnt(ctx, dst);
   }

   //
   // Exp_ConvertPtrInt::v_genStmnt
   //
   void Exp_ConvertPtrInt::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      auto dstW = type->getSizeWords();
      auto srcW = exp->getType()->getSizeWords();

      GenStmnt_MoveDstPre(exp, ctx, dst);

      // Evaluate sub-expression to stack.
      exp->genStmntStk(ctx);

      // Expand/contract on stack.
      ctx.block.setArgSize();
      if(srcW < dstW)
      {
         for(auto i = srcW - dstW; i--;)
            ctx.block.addStmnt(IR::CodeBase::Move, IR::Block::Stk(), 0);
      }
      else if(srcW > dstW)
      {
         for(auto i = srcW - dstW; i--;)
            ctx.block.addStmnt(IR::CodeBase::Move,
               IR::Block::Nul(), IR::Block::Stk());
      }

      // Move to destination.
      GenStmnt_MoveDstSuf(exp, ctx, dst);
   }

   //
   // Exp_ConvertPtrInt::v_isIRExp
   //
   bool Exp_ConvertPtrInt::v_isIRExp() const
   {
      // Do not allow conversion to __str at compile-time.
      if(type->isTypePointer() && type->getBaseType()->isTypeStrEnt())
         return false;

      auto expT = exp->getType();

      // Do not allow conversion from __str at compile-time.
      if(expT->isTypePointer() && expT->getBaseType()->isTypeStrEnt())
         return false;

      return exp->isIRExp();
   }

   //
   // Exp_ConvertPtrInv::v_genStmnt
   //
   void Exp_ConvertPtrInv::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      if(GenStmntNul(this, ctx, dst)) return;

      GenStmnt_MoveDstPre(exp, ctx, dst);

      // Evaluate sub-expression to stack.
      exp->genStmntStk(ctx);

      // Operate on stack.
      ctx.block.setArgSize().addStmnt(IR::CodeBase::BNot,
         IR::Block::Stk(), IR::Block::Stk());

      // Move to destination.
      GenStmnt_MoveDstSuf(exp, ctx, dst);
   }

   //
   // Exp_ConvertPtrLoc::v_genStmnt
   //
   void Exp_ConvertPtrLoc::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      if(GenStmntNul(this, ctx, dst)) return;

      GenStmnt_MoveDstPre(exp, ctx, dst);

      // Evaluate sub-expression to stack.
      exp->genStmntStk(ctx);

      // Operate on stack.
      ctx.block.setArgSize().addStmnt(IR::CodeBase::Pltn,
         IR::Block::Stk(), IR::Block::Stk());

      // Move to destination.
      GenStmnt_MoveDstSuf(exp, ctx, dst);
   }

   //
   // Exp_ConvertPtrSh::v_genStmnt
   //
   void Exp_ConvertPtrSh::v_genStmnt(SR::GenStmntCtx const &ctx,
      SR::Arg const &dst) const
   {
      if(GenStmntNul(this, ctx, dst)) return;

      GenStmnt_MoveDstPre(exp, ctx, dst);

      // Evaluate sub-expression to stack.
      exp->genStmntStk(ctx);

      // Operate on stack.
      auto shiftL = type->getBaseType()->getSizeShift();
      auto shiftR = exp->getType()->getBaseType()->getSizeShift();
      ctx.block.setArgSize();
      if(shiftL > shiftR)
      {
         auto lit = SR::Exp_IRExp::Create_Size(shiftL / shiftR)->getIRExp();
         ctx.block.addStmnt(IR::CodeBase::Div+'U', IR::Block::Stk(), IR::Block::Stk(), lit);
      }
      else
      {
         auto lit = SR::Exp_IRExp::Create_Size(shiftR / shiftL)->getIRExp();
         ctx.block.addStmnt(IR::CodeBase::Mul+'U', IR::Block::Stk(), IR::Block::Stk(), lit);
      }

      // Move to destination.
      GenStmnt_MoveDstSuf(exp, ctx, dst);
   }

   //
   // Factory::expConvert_ArithPtr
   //
   SR::Exp::CRef Factory::expConvert_ArithPtr(SR::Type const *typeL,
      SR::Exp const *e, Core::Origin pos)
   {
      SR::Exp::CRef exp{e};

      auto typeR = exp->getType();

      // Convert Aut pointer to Sta before converting to arithmetic type.
      if(typeR->getBaseType()->getQualAddr().base == IR::AddrBase::Aut)
      {
         typeR = typeR->getBaseType()
            ->getTypeQualAddr({IR::AddrBase::Sta, Core::STR_})->getTypePointer();
         typeR = (exp = Exp_ConvertPtrLoc::Create(typeR, exp, pos))->getType();
      }

      if(typeL->isCTypeFloat())
         Core::Error(pos, "pointer to float stub");

      if(typeL->isCTypeFixed())
         Core::Error(pos, "pointer to fixed stub");

      if(typeL->isCTypeInteg())
         return Exp_ConvertPtrInt::Create(typeL, exp, pos);

      Core::Error(pos, "unsupported pointer to arithmetic");
   }

   //
   // Factory::expConvert_Pointer
   //
   SR::Exp::CRef Factory::expConvert_Pointer(SR::Type const *typeL,
      SR::Exp const *e, Core::Origin pos)
   {
      SR::Exp::CRef exp{e};

      auto typeR = exp->getType();

      // Return early if already same type.
      if(typeL->getTypeQual() == typeR->getTypeQual())
         return exp;

      auto baseL = typeL->getBaseType();
      auto baseR = typeR->getBaseType();

      // Special handling for pointer-to-function.
      if(baseL->isCTypeFunction() && baseR->isCTypeFunction())
         return Exp_ConvertPtr::Create(typeL, exp, pos);

      // If converting from __str_ent* to T __str_ars*, convert to char
      // const __str_ars* first.
      if(baseR->isTypeStrEnt() && baseL->getQualAddr().base == IR::AddrBase::StrArs)
      {
         exp   = expCreate_Add(e, expCreate_LitInt(TypeIntegPrS, 0, pos), pos);
         typeR = exp->getType();
         baseR = typeR->getBaseType();
      }

      // Remainder of function concerns pointer-to-object.
      if(!baseL->isCTypeObject() || !baseR->isCTypeObject())
         Core::Error(e->pos, "expected pointer to object");

      // Special step for conversion from pointer-to-Aut.
      if(baseR->getQualAddr().base == IR::AddrBase::Aut &&
         baseL->getQualAddr().base != IR::AddrBase::Aut)
      {
         baseR = baseR->getTypeQualAddr(IR::AddrBase::Sta);
         typeR = baseR->getTypePointer();
         exp   = Exp_ConvertPtrLoc::Create(typeR, exp, pos);
      }

      // Adjust address space.
      if(baseL->getQualAddr().base != baseR->getQualAddr().base)
      {
         exp   = ExpConvert_PtrSpace(typeL, exp, pos);
         typeR = exp->getType();
         baseR = typeR->getBaseType();
      }

      // Adjust pointer shift.
      if(baseL->getSizeShift() != baseR->getSizeShift())
      {
         exp   = Exp_ConvertPtrSh::Create(typeL, exp, pos);
         typeR = exp->getType();
         baseR = typeR->getBaseType();
      }

      // If type is still not an exact match, use a wrapper to say it is.
      if(typeL->getTypeQual() != typeR->getTypeQual())
      {
         exp   = Exp_ConvertPtr::Create(typeL, exp, pos);
         typeR = exp->getType();
         baseR = typeR->getBaseType();
      }

      return exp;
   }

   //
   // Factory::expConvert_PtrArith
   //
   SR::Exp::CRef Factory::expConvert_PtrArith(SR::Type const *typeL,
      SR::Exp const *e, Core::Origin pos)
   {
      SR::Exp::CRef exp{e};

      auto typeR = exp->getType();

      if(typeR->isCTypeFloat())
         Core::Error(pos, "float to pointer stub");

      if(typeR->isCTypeFixed())
         Core::Error(pos, "fixed to pointer stub");

      if(typeR->isCTypeInteg())
      {
         if(exp->isZero())
            return expCreate_Comma(exp, expCreate_LitNul(typeL, pos), pos);

         return Exp_ConvertPtrInt::Create(typeL, exp, pos);
      }

      Core::Error(pos, "unsupported arithmetic to pointer");
   }
}

// EOF

