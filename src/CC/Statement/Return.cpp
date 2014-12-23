//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

#include "AST/Arg.hpp"
#include "AST/Exp.hpp"
#include "AST/Function.hpp"
#include "AST/Type.hpp"

#include "Core/Exception.hpp"

#include "IR/Block.hpp"
#include "IR/CallType.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// CheckConstraint
//
static GDCC::AST::Exp::CPtr CheckConstraint(GDCC::Core::Origin pos,
   GDCC::CC::Scope_Function &scope, GDCC::AST::Exp const *exp)
{
   using namespace GDCC;

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


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // Statement_ReturnExp constructor
      //
      Statement_ReturnExp::Statement_ReturnExp(Labels const &labels_,
         Core::Origin pos_, AST::Exp const *exp_, Scope_Function &scope_) :
         Super{labels_, pos_}, exp{exp_}, scope(scope_)
      {
      }

      //
      // Statement_ReturnExp constructor
      //
      Statement_ReturnExp::Statement_ReturnExp(Labels &&labels_,
         Core::Origin pos_, AST::Exp const *exp_, Scope_Function &scope_) :
         Super{std::move(labels_), pos_}, exp{exp_}, scope(scope_)
      {
      }

      //
      // Statement_ReturnExp constructor
      //
      Statement_ReturnExp::Statement_ReturnExp(Core::Origin pos_,
         AST::Exp const *exp_, Scope_Function &scope_) :
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
      void Statement_ReturnExp::v_genStmnt(AST::GenStmntCtx const &ctx) const
      {
         auto ctype = IR::GetCallTypeIR(scope.fn->ctype);

         exp->genStmntStk(ctx);

         if(ctype == IR::CallType::ScriptI || ctype == IR::CallType::ScriptS)
         {
            ctx.block.addStatementArgs({IR::Code::Jump, 0},
               IR::Glyph(ctx.prog, scope.fn->getLabelEnd()));
         }
         else
         {
            ctx.block.setArgs({exp->getType()->getSizeWords(), IR::Arg_Stk()});
            ctx.block.addStatement({IR::Code::Retn, 0});
         }
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
      void Statement_ReturnNul::v_genStmnt(AST::GenStmntCtx const &ctx) const
      {
         auto ctype = IR::GetCallTypeIR(scope.fn->ctype);

         if(ctype == IR::CallType::ScriptI || ctype == IR::CallType::ScriptS)
         {
            ctx.block.addStatementArgs({IR::Code::Jump, 0},
               IR::Glyph(ctx.prog, scope.fn->getLabelEnd()));
         }
         else
         {
            ctx.block.addStatementArgs({IR::Code::Retn, 0});
         }
      }

      //
      // StatementCreate_Return
      //
      AST::Statement::CRef StatementCreate_Return(
         AST::Statement::Labels const &labels, Core::Origin pos,
         Scope_Function &scope, AST::Exp const *e)
      {
         if(auto exp = CheckConstraint(pos, scope, e))
            return Statement_ReturnExp::Create(labels, pos, exp, scope);
         else
            return Statement_ReturnNul::Create(labels, pos, scope);
      }

      //
      // StatementCreate_Return
      //
      AST::Statement::CRef StatementCreate_Return(
         AST::Statement::Labels &&labels, Core::Origin pos,
         Scope_Function &scope, AST::Exp const *e)
      {
         if(auto exp = CheckConstraint(pos, scope, e))
            return Statement_ReturnExp::Create(std::move(labels), pos, exp, scope);
         else
            return Statement_ReturnNul::Create(std::move(labels), pos, scope);
      }

      //
      // StatementCreate_Return
      //
      AST::Statement::CRef StatementCreate_Return(Core::Origin pos,
         Scope_Function &scope, AST::Exp const *e)
      {
         if(auto exp = CheckConstraint(pos, scope, e))
            return Statement_ReturnExp::Create(pos, exp, scope);
         else
            return Statement_ReturnNul::Create(pos, scope);
      }
   }
}

// EOF

