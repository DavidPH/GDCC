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


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_ConverBoolSoft_Fixed::v_genStmnt
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

         throw Core::ExceptStr(pos, "soft bool stub");
      }
   }
}

// EOF

