//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C return statements.
//
//-----------------------------------------------------------------------------

#include "CC/Statement/Return.hpp"

#include "CC/Factory.hpp"
#include "CC/Scope/Function.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"
#include "IR/Glyph.hpp"

#include "SR/Arg.hpp"
#include "SR/Exp.hpp"
#include "SR/Function.hpp"
#include "SR/Type.hpp"

#include "Target/CallType.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // CheckConstraint
   //
   static SR::Exp::CPtr CheckConstraint(Factory &fact, Core::Origin pos,
      CC::Scope_Function &scope, SR::Exp const *exp)
   {
      bool isVoid = scope.fn->retrn->isTypeVoid();

      // Sanity check. This should have been caught by now.
      if(!isVoid && !scope.fn->retrn->isTypeComplete())
         Core::Error(pos, "complete type required for return");

      if(exp)
      {
         if(isVoid)
            Core::Error(pos, "return expression forbidden");

         return fact.expPromo_Assign(scope.fn->retrn, exp, pos);
      }
      else
      {
         if(!isVoid)
            Core::Error(pos, "return expression required");

         return nullptr;
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::CC
{
   //
   // Statement_ReturnExp constructor
   //
   Statement_ReturnExp::Statement_ReturnExp(Labels &&labels_,
      Core::Origin pos_, SR::Exp const *exp_, Scope_Function &scope_) :
      Super{std::move(labels_), pos_}, exp{exp_}, scope(scope_)
   {
   }

   //
   // Statement_ReturnExp destructor
   //
   Statement_ReturnExp::~Statement_ReturnExp()
   {
   }

   //
   // Statement_ReturnExp::v_genStmnt
   //
   void Statement_ReturnExp::v_genStmnt(SR::GenStmntCtx const &ctx) const
   {
      auto ctype = Target::GetCallTypeIR(scope.fn->ctype);

      exp->genStmntStk(ctx);

      if(ctype == IR::CallType::ScriptI || ctype == IR::CallType::ScriptS)
      {
         ctx.block.setArgSize().addStmnt(IR::CodeBase::Jump,
            IR::Glyph(ctx.prog, scope.fn->getLabelEnd()));
      }
      else
         ctx.block.addStmnt(IR::CodeBase::Retn,
            IR::Arg_Stk(exp->getType()->getSizeBytes()));
   }

   //
   // Statement_ReturnExp::v_isEffect
   //
   bool Statement_ReturnExp::v_isEffect() const
   {
      return exp->isEffect();
   }

   //
   // Statement_ReturnExp::v_isNoAuto
   //
   bool Statement_ReturnExp::v_isNoAuto() const
   {
      return exp->isNoAuto();
   }

   //
   // Statement_ReturnNul::v_genStmnt
   //
   void Statement_ReturnNul::v_genStmnt(SR::GenStmntCtx const &ctx) const
   {
      auto ctype = Target::GetCallTypeIR(scope.fn->ctype);

      if(ctype == IR::CallType::ScriptI || ctype == IR::CallType::ScriptS)
      {
         ctx.block.setArgSize().addStmnt(IR::CodeBase::Jump,
            IR::Glyph(ctx.prog, scope.fn->getLabelEnd()));
      }
      else
         ctx.block.addStmnt(IR::CodeBase::Retn);
   }

   //
   // Factory::stCreate_Return
   //
   SR::Statement::CRef Factory::stCreate_Return(
      SR::Statement::Labels &&labels, Core::Origin pos,
      Scope_Function &scope, SR::Exp const *e)
   {
      if(auto exp = CheckConstraint(*this, pos, scope, e))
         return Statement_ReturnExp::Create(std::move(labels), pos, exp, scope);
      else
         return Statement_ReturnNul::Create(std::move(labels), pos, scope);
   }
}

// EOF

