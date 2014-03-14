//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C pointer conversion expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Convert/Pointer.hpp"

#include "AST/Type.hpp"

#include "Bytecode/Platform.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// ExpConvert_PtrSpace
//
static GDCC::AST::Exp::CRef ExpConvert_PtrSpace(GDCC::AST::Type const *typeL,
   GDCC::AST::Exp const *exp, GDCC::Core::Origin pos)
{
   using namespace GDCC;

   auto typeR = exp->getType();

   auto baseL = typeL->getBaseType();
   auto baseR = typeR->getBaseType();

   auto addrL = baseL->getQualAddr();
   auto addrR = baseR->getQualAddr();

   // To far.
   if(addrL.base == IR::AddrBase::Far)
   {
      throw Core::ExceptStr(pos, "convert to far pointer stub");
   }

   // From far.
   if(addrR.base == IR::AddrBase::Far)
   {
      throw Core::ExceptStr(pos, "convert from far pointer stub");
   }

   // As a last resort, same size pointers can fall back to direct bitwise copy.
   if(typeL->getSizeWords() == typeR->getSizeWords())
   {
      baseR = baseR->getTypeQualAddr(addrL);
      typeR = baseR->getTypePointer();

      // If null representation is different, perform inversion.
      if(Bytecode::IsZeroNull_Point(addrL.base) != Bytecode::IsZeroNull_Point(addrR.base))
         return CC::Exp_ConvertPtrInv::Create(typeR, exp, pos);
      else
         return CC::Exp_ConvertPtr::Create(typeR, exp, pos);
   }

   throw Core::ExceptStr(pos, "unsupported pointer conversion");
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_ConvertPtr::v_genStmnt
      //
      void Exp_ConvertPtr::v_genStmnt(IR::Block &block, AST::Function *fn,
         AST::Arg const &dst) const
      {
         return exp->genStmnt(block, fn, dst);
      }

      //
      // Exp_ConvertPtrInv::v_genStmnt
      //
      void Exp_ConvertPtrInv::v_genStmnt(IR::Block &block, AST::Function *fn,
         AST::Arg const &dst) const
      {
         if(GenStmntNul(this, block, fn, dst)) return;

         // Evaluate sub-expression to stack.
         exp->genStmntStk(block, fn);

         // Operate on stack.
         block.addStatementArgs(IR::Code::InvU_W,
            IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         exp->genStmntMovePart(block, fn, dst, false, true);
      }

      //
      // Exp_ConvertPtrLoc::v_genStmnt
      //
      void Exp_ConvertPtrLoc::v_genStmnt(IR::Block &block, AST::Function *fn,
         AST::Arg const &dst) const
      {
         if(GenStmntNul(this, block, fn, dst)) return;

         // Evaluate sub-expression to stack.
         exp->genStmntStk(block, fn);

         // Operate on stack.
         block.addStatementArgs(IR::Code::Pltn, IR::Arg_Stk(), IR::Arg_Stk());

         // Move to destination.
         exp->genStmntMovePart(block, fn, dst, false, true);
      }

      //
      // Exp_ConvertPtrSh::v_genStmnt
      //
      void Exp_ConvertPtrSh::v_genStmnt(IR::Block &block, AST::Function *fn,
         AST::Arg const &dst) const
      {
         if(GenStmntNul(this, block, fn, dst)) return;

         // Evaluate sub-expression to stack.
         exp->genStmntStk(block, fn);

         // Operate on stack.
         auto shiftL = type->getBaseType()->getSizeShift();
         auto shiftR = exp->getType()->getBaseType()->getSizeShift();
         if(shiftL > shiftR)
         {
            auto lit = AST::ExpCreate_Size(shiftL / shiftR)->getIRExp();
            block.addStatementArgs(IR::Code::DivU_W,
               IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Lit(lit));
         }
         else
         {
            auto lit = AST::ExpCreate_Size(shiftR / shiftL)->getIRExp();
            block.addStatementArgs(IR::Code::MulU_W,
               IR::Arg_Stk(), IR::Arg_Stk(), IR::Arg_Lit(lit));
         }

         // Move to destination.
         exp->genStmntMovePart(block, fn, dst, false, true);
      }

      //
      // ExpConvert_Pointer
      //
      AST::Exp::CRef ExpConvert_Pointer(AST::Type const *typeL,
         AST::Exp const *e, Core::Origin pos)
      {
         AST::Exp::CRef exp{e};

         auto typeR = exp->getType();

         // Return early if already same type.
         if(typeL->getTypeQual() == typeR->getTypeQual())
            return exp;

         auto baseL = typeL->getBaseType();
         auto baseR = typeR->getBaseType();

         // Special handling for pointer-to-function.
         if(baseL->isCTypeFunction() && baseR->isCTypeFunction())
            return Exp_ConvertPtr::Create(typeL, exp, pos);

         // Special step for conversion from pointer-to-loc.
         if(baseR->getQualAddr().base == IR::AddrBase::Loc &&
            baseL->getQualAddr().base != IR::AddrBase::Loc)
         {
            baseR = baseR->getTypeQualAddr(IR::AddrBase::LocArs);
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
      // ExpConvert_PtrArith
      //
      AST::Exp::CRef ExpConvert_PtrArith(AST::Type const *typeL,
         AST::Exp const *e, Core::Origin pos)
      {
         AST::Exp::CRef exp{e};

         auto typeR = exp->getType();

         if(typeR->isCTypeFloat())
         {
            throw Core::ExceptStr(pos, "float to pointer stub");
         }

         if(typeR->isCTypeFixed())
         {
            throw Core::ExceptStr(pos, "fixed to pointer stub");
         }

         if(typeR->isCTypeInteg())
         {
            if(exp->isZero())
               return ExpCreate_Comma(exp, ExpCreate_LitNul(typeL, pos), pos);

            throw Core::ExceptStr(pos, "integ to pointer stub");
         }

         throw Core::ExceptStr(pos, "unsupported arithmetic to pointer");
      }
   }
}

// EOF

