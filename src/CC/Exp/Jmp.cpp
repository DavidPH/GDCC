//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C setjmp/longjmp expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Jmp.hpp"

#include "CC/Scope/Function.hpp"
#include "CC/Type.hpp"

#include "AST/Arg.hpp"
#include "AST/Function.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Program.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Exp_JmpLng::v_genStmnt
      //
      void Exp_JmpLng::v_genStmnt(AST::GenStmntCtx const &ctx, AST::Arg const &) const
      {
         // Generate IR arg for env.
         IR::Arg envArg;
         if(env->getArg().isIRArg())
            envArg = IR::Arg_Sta(env->getArg().getIRArg(ctx.prog));
         else
            throw Core::ExceptStr(pos, "non-IRArg env stub");

         // Generate IR arg for val.
         IR::Arg valArg;
         if(val->getArg().isIRArg())
            valArg = val->getArg().getIRArg(ctx.prog);
         else
            throw Core::ExceptStr(pos, "non-IRArg val stub");

         ctx.block.addStatementArgs({IR::Code::Jfar, 0},
            IR::Glyph(ctx.prog, scope.fn.getLabelLJR()), envArg, valArg);
      }

      //
      // Exp_JmpLng::v_getType
      //
      AST::Type::CRef Exp_JmpLng::v_getType() const
      {
         return AST::Type::Void;
      }

      //
      // Exp_JmpLng::v_isNoAuto
      //
      bool Exp_JmpLng::v_isNoAuto() const
      {
         return env->isNoAuto() && val->isNoAuto();
      }

      //
      // Exp_JmpSet::v_genStmnt
      //
      void Exp_JmpSet::v_genStmnt(AST::GenStmntCtx const &ctx, AST::Arg const &dst) const
      {
         // Generate IR arg for env.
         IR::Arg envArg;
         if(env->getArg().isIRArg())
            envArg = IR::Arg_Sta(env->getArg().getIRArg(ctx.prog));
         else
            throw Core::ExceptStr(pos, "non-IRArg env stub");

         // Generate dynamic jump target for addr.
         Core::String addrLabel = ctx.fn->genLabel();
         IR::DJump   &addrDJump = ctx.prog.getDJump(ctx.fn->genLabel());
         addrDJump.label = addrLabel;
         addrDJump.alloc = true;
         addrDJump.defin = true;

         ctx.block.addStatementArgs({IR::Code::Jset, 0},
            envArg, IR::Glyph(ctx.prog, addrDJump.glyph));

         ctx.block.addLabel(addrLabel);

         // Move to destination from stack.
         GenStmnt_MovePart(this, ctx, dst, false, true);
      }

      //
      // Exp_JmpSet::v_getType
      //
      AST::Type::CRef Exp_JmpSet::v_getType() const
      {
         return TypeIntegPrS;
      }

      //
      // Exp_JmpSet::v_isNoAuto
      //
      bool Exp_JmpSet::v_isNoAuto() const
      {
         return env->isNoAuto();
      }

      //
      // ExpCreate_JmpLng
      //
      AST::Exp::CRef ExpCreate_JmpLng(Scope &scope, AST::Exp const *env_,
         AST::Exp const *val_, Core::Origin pos)
      {
         auto env = ExpPromo_Assign(TypeIntegPrS->getTypePointer(), env_, pos);
         auto val = ExpPromo_Assign(TypeIntegPrS, val_, pos);

         if(auto scopeLocal = dynamic_cast<Scope_Local *>(&scope))
            return Exp_JmpLng::Create(*scopeLocal, env, val, pos);
         else
            throw Core::ExceptStr(pos, "invalid scope for longjmp");

      }

      //
      // ExpCreate_JmpSet
      //
      AST::Exp::CRef ExpCreate_JmpSet(AST::Exp const *env_, Core::Origin pos)
      {
         auto env = ExpPromo_Assign(TypeIntegPrS->getTypePointer(), env_, pos);

         return Exp_JmpSet::Create(env, pos);
      }
   }
}

// EOF

