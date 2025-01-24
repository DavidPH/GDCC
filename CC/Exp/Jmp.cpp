//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2025 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C setjmp/longjmp expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp/Jmp.hpp"

#include "CC/Factory.hpp"
#include "CC/Scope/Function.hpp"
#include "CC/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Program.hpp"

#include "SR/Arg.hpp"
#include "SR/Function.hpp"
#include "SR/Temporary.hpp"
#include "SR/Type.hpp"

#include "Target/Info.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Exp_JmpLng::v_genStmnt
   //
   void Exp_JmpLng::v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &) const
   {
      // Generate IR arg for env.
      IR::Arg envArg;
      if(env->getArg().isIRArg())
         envArg = IR::Arg_Sta(Target::GetWordBytes(),
            env->getArg().getIRArg(ctx.prog));
      else
         Core::Error(pos, "non-IRArg env stub");

      // Generate IR arg for val.
      IR::Arg valArg;
      if(val->getArg().isIRArg())
         valArg = val->getArg().getIRArg(ctx.prog);
      else
         Core::Error(pos, "non-IRArg val stub");

      ctx.block.setArgSize().addStmnt(IR::CodeBase::Jfar_Sta,
         IR::Glyph(ctx.prog, scope.fn.getLabelLJR()),
         std::move(envArg), std::move(valArg));
   }

   //
   // Exp_JmpLng::v_getType
   //
   SR::Type::CRef Exp_JmpLng::v_getType() const
   {
      return SR::Type::Void;
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
   void Exp_JmpSet::v_genStmnt(SR::GenStmntCtx const &ctx, SR::Arg const &dst) const
   {
      // Generate IR arg for env.
      SR::Temporary envTmp{ctx, pos};
      IR::Arg       envArg;
      if(env->getArg().isIRArg())
         envArg = IR::Arg_Sta(Target::GetWordBytes(),
            env->getArg().getIRArg(ctx.prog));
      else
      {
         SR::Type::CRef envType = env->getType();
         envTmp.alloc(envType->getSizeWords());
         envArg = IR::Arg_Sta(Target::GetWordBytes(), envTmp.getArg());

         // TODO: Convert envTmp to an SR::Arg to avoid stack op.
         env->genStmntStk(ctx);
         ctx.block.addStmnt(IR::CodeBase::Move, envTmp.getArg(), envTmp.getArgStk());
      }

      // Generate dynamic jump target for addr.
      Core::String addrLabel = ctx.fn->genLabel();
      IR::DJump   &addrDJump = ctx.prog.getDJump(ctx.fn->genLabel());
      addrDJump.label = addrLabel;
      addrDJump.alloc = true;
      addrDJump.defin = true;

      GenStmnt_MoveDstPre(this, ctx, dst);

      ctx.block.setArgSize().addStmnt(IR::CodeBase::Jfar_Set,
         IR::Glyph(ctx.prog, addrDJump.glyph), std::move(envArg));

      ctx.block.addLabel(addrLabel);

      // Move to destination from stack.
      GenStmnt_MoveDstSuf(this, ctx, dst);
   }

   //
   // Exp_JmpSet::v_getType
   //
   SR::Type::CRef Exp_JmpSet::v_getType() const
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
   // Factory::expCreate_JmpLng
   //
   SR::Exp::CRef Factory::expCreate_JmpLng(Scope &scope, SR::Exp const *env_,
      SR::Exp const *val_, Core::Origin pos)
   {
      auto env = expPromo_Assign(TypeIntegPrS->getTypePointer(), env_, pos);
      auto val = expPromo_Assign(TypeIntegPrS, val_, pos);

      if(auto scopeLocal = dynamic_cast<Scope_Local *>(&scope))
         return Exp_JmpLng::Create(*scopeLocal, env, val, pos);
      else
         Core::Error(pos, "invalid scope for longjmp");

   }

   //
   // Factory::expCreate_JmpSet
   //
   SR::Exp::CRef Factory::expCreate_JmpSet(SR::Exp const *env_, Core::Origin pos)
   {
      auto env = expPromo_Assign(TypeIntegPrS->getTypePointer(), env_, pos);

      return Exp_JmpSet::Create(env, pos);
   }
}

// EOF

