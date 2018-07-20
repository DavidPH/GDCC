//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C return statements.
//
//-----------------------------------------------------------------------------

#include "CC/Statement/Return.hpp"

#include "CC/Exp.hpp"
#include "CC/Scope/Function.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"
#include "IR/CallType.hpp"
#include "IR/Glyph.hpp"

#include "SR/Arg.hpp"
#include "SR/Exp.hpp"
#include "SR/Function.hpp"
#include "SR/Type.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::CC
{
   //
   // CheckConstraint
   //
   static SR::Exp::CPtr CheckConstraint(Core::Origin pos,
      CC::Scope_Function &scope, SR::Exp const *exp)
   {
      bool isVoid = scope.fn->retrn->isTypeVoid();

      // Sanity check. This should have been caught by now.
      if(!isVoid && !scope.fn->retrn->isTypeComplete())
         throw Core::ExceptStr(pos, "complete type required for return");

      if(exp)
      {
         if(isVoid)
            throw Core::ExceptStr(pos, "return expression forbidden");

         return CC::ExpPromo_Assign(scope.fn->retrn, exp, pos);
      }
      else
      {
         if(!isVoid)
            throw Core::ExceptStr(pos, "return expression required");

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
   Statement_ReturnExp::Statement_ReturnExp(Labels const &labels_,
      Core::Origin pos_, SR::Exp const *exp_, Scope_Function &scope_) :
      Super{labels_, pos_}, exp{exp_}, scope(scope_)
   {
   }

   //
   // Statement_ReturnExp constructor
   //
   Statement_ReturnExp::Statement_ReturnExp(Labels &&labels_,
      Core::Origin pos_, SR::Exp const *exp_, Scope_Function &scope_) :
      Super{std::move(labels_), pos_}, exp{exp_}, scope(scope_)
   {
   }

   //
   // Statement_ReturnExp constructor
   //
   Statement_ReturnExp::Statement_ReturnExp(Core::Origin pos_,
      SR::Exp const *exp_, Scope_Function &scope_) :
      Super{pos_}, exp{exp_}, scope(scope_)
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
      auto ctype = IR::GetCallTypeIR(scope.fn->ctype);

      exp->genStmntStk(ctx);

      if(ctype == IR::CallType::ScriptI || ctype == IR::CallType::ScriptS)
      {
         ctx.block.setArgSize().addStmnt(IR::Code::Jump,
            IR::Glyph(ctx.prog, scope.fn->getLabelEnd()));
      }
      else
         ctx.block.addStmnt(IR::Code::Retn, IR::Arg_Stk(exp->getType()->getSizeBytes()));
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
      auto ctype = IR::GetCallTypeIR(scope.fn->ctype);

      if(ctype == IR::CallType::ScriptI || ctype == IR::CallType::ScriptS)
      {
         ctx.block.setArgSize().addStmnt(IR::Code::Jump,
            IR::Glyph(ctx.prog, scope.fn->getLabelEnd()));
      }
      else
         ctx.block.addStmnt(IR::Code::Retn);
   }

   //
   // StatementCreate_Return
   //
   SR::Statement::CRef StatementCreate_Return(
      SR::Statement::Labels const &labels, Core::Origin pos,
      Scope_Function &scope, SR::Exp const *e)
   {
      if(auto exp = CheckConstraint(pos, scope, e))
         return Statement_ReturnExp::Create(labels, pos, exp, scope);
      else
         return Statement_ReturnNul::Create(labels, pos, scope);
   }

   //
   // StatementCreate_Return
   //
   SR::Statement::CRef StatementCreate_Return(
      SR::Statement::Labels &&labels, Core::Origin pos,
      Scope_Function &scope, SR::Exp const *e)
   {
      if(auto exp = CheckConstraint(pos, scope, e))
         return Statement_ReturnExp::Create(std::move(labels), pos, exp, scope);
      else
         return Statement_ReturnNul::Create(std::move(labels), pos, scope);
   }

   //
   // StatementCreate_Return
   //
   SR::Statement::CRef StatementCreate_Return(Core::Origin pos,
      Scope_Function &scope, SR::Exp const *e)
   {
      if(auto exp = CheckConstraint(pos, scope, e))
         return Statement_ReturnExp::Create(pos, exp, scope);
      else
         return Statement_ReturnNul::Create(pos, scope);
   }
}

// EOF

