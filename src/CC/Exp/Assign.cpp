//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator =" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Assign.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"
#include "IR/Exp.hpp"

#include "SR/Arg.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
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

            if(dst.type->getQualAddr().base == IR::AddrBase::Nul)
            {
               // TODO: Do not require an IR Arg.
               if(!expL->getArg().isIRArg())
                  throw Core::ExceptStr(pos, "bitfield write must be IR arg");
               IR::Arg argL = expL->getArg().getIRArg(ctx.prog);

               // Generate IR arg for right operand.
               IR::Arg argR;
               if(!expR->getArg().isIRArg())
               {
                  expR->genStmntStk(ctx);
                  argR = IR::Arg_Stk();
               }
               else
                  argR = expR->getArg().getIRArg(ctx.prog);

               ctx.block.addStatementArgs({IR::Code::Bset_W, typeL->getSizeWords()},
                  argL, argR, bits, offs);
            }
            else
               throw Core::ExceptStr(pos, "bitfield write-and-read unimplemented");
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
      // ExpCreate_Assign
      //
      SR::Exp::CRef ExpCreate_Assign(SR::Exp const *l, SR::Exp const *r,
         Core::Origin pos)
      {
         if(!IsModLValue(l))
            throw Core::ExceptStr(l->pos, "expected modifiable lvalue");

         auto type = l->getType();
         if(type->isTypeBitfield())
            type = type->getBaseType();

         auto expR = ExpPromo_Assign(type, r, pos);

         return Exp_Assign::Create(l, expR, pos);
      }
   }
}

// EOF

